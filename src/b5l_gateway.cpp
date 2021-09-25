
#include "b5l_gateway.hpp"

using namespace std::chrono_literals;
const char *FilePath="/home/zeroxcorbin/file.pcd";

/* Output formate */
INT32 Tof_output_format = 0;

int ExitApp = 0;
std::mutex exitLock;

struct PointCloudBuffers {
  using Ptr = std::shared_ptr<PointCloudBuffers>;
  std::vector<short> cloud_buffers;
  std::vector<short> intensity;
};

pcl::PointCloud<pcl::PointXYZI>::Ptr cloud_ (new pcl::PointCloud<pcl::PointXYZI>()); 
PointCloudBuffers cloud_buffers;// (new std::vector<short>);

void
CopyPointCloudToBuffers()
{
  const std::size_t nr_points = cloud_->points.size();

  cloud_buffers.cloud_buffers.resize(nr_points * 3);
	cloud_buffers.intensity.resize(nr_points);

  std::size_t j = 0;
  for (std::size_t i = 0; i < nr_points; ++i) {

    const pcl::PointXYZI& point = (*cloud_)[i];


    cloud_buffers.cloud_buffers[j * 3 + 0] = static_cast<short>(point.x * 1000);
    cloud_buffers.cloud_buffers[j * 3 + 1] = static_cast<short>(point.y * 1000);
    cloud_buffers.cloud_buffers[j * 3 + 2] = static_cast<short>(point.z * 1000);

	cloud_buffers.intensity[j] = static_cast<short>(point.intensity);
    // cloud_buffers.rgb[j * 3 + 0] = point.r;
    // cloud_buffers.rgb[j * 3 + 1] = point.g;
    // cloud_buffers.rgb[j * 3 + 2] = point.b;

    j++;
  }

  cloud_buffers.cloud_buffers.resize(j * 3);
  cloud_buffers.intensity.resize(j);
}


bool ReadSensor(){
		cloud_->clear();
	if (CTOFSample::Run(cloud_) == 0)
	{
		return true;
	}else{
		return false;
	}
}

bool SaveImageFile(){
	std::cout<< "Reading sensor and saving to file."<< std::endl;

	//pcl::PointCloud<pcl::PointXYZI> cloud_; 
	if (CTOFSample::Run(cloud_) == 0)
	{
		pcl::PCDWriter w;
		w.writeBinaryCompressed (FilePath, *cloud_);

		std::cout<< "File saved."<< std::endl;
		return true;
	}
	return false;
}

void SendImageFile(clsTCPSocket *client){
	std::cout<< "Reading sensor and sending data."<< std::endl;

	if(ReadSensor()){
		//PointCloudBuffers *data (new PointCloudBuffers());
		CopyPointCloudToBuffers();

		client->Write((const char *)&cloud_buffers.cloud_buffers[0], cloud_buffers.cloud_buffers.size() * 2);
		client->Write((const char *)&cloud_buffers.intensity[0], cloud_buffers.intensity.size() * 2);
		std::cout<< "Sent: "<< (cloud_buffers.cloud_buffers.size() * 2) + cloud_buffers.intensity.size() * 2  << std::endl;
	}

	// if(SaveImageFile()){
	// 	std::ifstream infile;
	// 	infile.open(FilePath, std::ios::binary);
	// 	infile.seekg(0, std::ios::end);
	// 	size_t file_size_in_byte = infile.tellg();
	// 	std::vector<char> data; // used to store text data
	// 	data.resize(file_size_in_byte);
	// 	infile.seekg(0, std::ios::beg);
	// 	infile.read(&data[0], file_size_in_byte);
	// 	infile.close();

	// 	client->Write(&data[0], file_size_in_byte);
	// 	std::cout<< "Sending data complete."<< std::endl;

	// }
}

void ClientConnectedThread(clsTCPSocket *client){
	std::cout<< "Client connected..."<< std::endl;

	while(true){
		long len = client->Read();
		if(len > 0){
			if(strcmp(client->recBuffer, "save\r\n") == 0){
				SaveImageFile();
			}
			if(strcmp(client->recBuffer, "send\r\n") == 0){
				SendImageFile(client);
			}
			if(strcmp(client->recBuffer, "exit\r\n") == 0){
				//std::lock_guard<std::mutex> lock(exitLock);
				ExitApp = 1;
				break;
			}
		}else if(len < 0){
			break;
		}
	}
}

void ListenWaitThread(){
	clsTCPSocket listener;

	listener.NameIP = "";
	listener.Port = 8890;
	
	if(!listener.Configure()){
		return;
	}

	while(true){

		clsTCPSocket *client = new (clsTCPSocket);

		std::cout<< "Listening..."<< std::endl;
		if(listener.Listen(client)){
			std::thread connected (ClientConnectedThread, client);
			connected.join();

			delete(client);

			if(ExitApp == 1){
				listener.Close();
				break;
			}
		}else{
			std::cout<< "Listen Error..."<< std::endl;

			client->Close();
			delete(client);

			listener.Close();

			break;
		}	
	}
}


int main()
{
	

	/* Load USB serial module */
#ifdef PI
    if (system(NULL)){
       std::cout<< "Setting up serial port."<< std::endl;
		system("sudo modprobe usbserial vendor=0x0590 product=0x00ca");
		system("sudo chmod 666 /dev/ttyUSB0");		
	}else{
		std::cout<< "System commands not available."<< std::endl;
	}

#endif

	std::string config_file = SOURCE_DIR_PREFIX;
	config_file.append(CONFIG_FILE_PATH);

	std::cout<< "B5L Testing version 0.2 starting"<< std::endl;
	int Ret = CTOFSample::Init(config_file, Tof_output_format); // Init camera and UART
	/* Here we are supporting output format 257 || 258 || 1 || 2 */
	/* Format validation done in Init function and if fails then it returns non zero value */

	if (Ret == 0)
	{	
		std::thread listen (ListenWaitThread);
		listen.join();
	}

	std::cout<< "B5L Testing version 0.2 exiting"<< std::endl;

	CTOFSample::Stop();

	return 0;
}