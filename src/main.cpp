#include "main.h"

using namespace std::chrono_literals;
const char *FilePath="/home/zeroxcorbin/file.pcd";

/* Output formate */
INT32 Tof_output_format = 0;

int ExitApp = 0;

clsTCPSocket Listener;

bool SaveImageFile(){
	std::cout<< "Reading sensor and saving to file."<< std::endl;

	pcl::PointCloud<pcl::PointXYZI> cloud_;
	if (CTOFSample::Run(&cloud_) == 0)
	{
		pcl::PCDWriter w;
		w.writeBinaryCompressed (FilePath, cloud_);

		std::cout<< "File saved."<< std::endl;
		return true;
	}
	return false;
}

void SendImageFile(clsTCPSocket *client){
	std::cout<< "Reading sensor and sending data."<< std::endl;

	if(SaveImageFile()){
		std::ifstream infile;
		infile.open(FilePath, std::ios::binary);
		infile.seekg(0, std::ios::end);
		size_t file_size_in_byte = infile.tellg();
		std::vector<char> data; // used to store text data
		data.resize(file_size_in_byte);
		infile.seekg(0, std::ios::beg);
		infile.read(&data[0], file_size_in_byte);
		infile.close();

		client->Write(&data[0]);

	}
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
				ExitApp = 1;
				break;
			}
		}else if(len < 0){
			break;
		}
	}
	client->Close();
}

void ListenWaitThread(){

	Listener.NameIP = "";
	Listener.Port = 8890;
	
	if(!Listener.Configure()){
		ExitApp = 1;
		return;
	}

	std::cout<< "Listening..."<< std::endl;

	while(true){

		clsTCPSocket *client = new (clsTCPSocket);

		if(Listener.Listen(client)){
			ClientConnectedThread(client);

			delete(client);

			if(ExitApp == 1){
				break;
			}
		}else{
			std::cout<< "Listen Error..."<< std::endl;
			ExitApp = 1;

			client->Close();
			Listener.Close();

			delete(client);

			break;
		}	
	}
}

int main()
{
	std::string config_file = SOURCE_DIR_PREFIX;

	/* Load USB serial module */
#ifdef PI
	system("sudo modprobe usbserial vendor=0x0590 product=0x00ca");
	system("sudo chmod 666 /dev/ttyUSB0");
#endif

	// Listener.NameIP = "192.168.0.123";
	// Listener.Port = 8890;
	// if(!Listener.Configure()) return -1;

	config_file.append(CONFIG_FILE_PATH);

	std::cout<< "B5L Testing version 0.2 starting"<< std::endl;
	int Ret = CTOFSample::Init(config_file, Tof_output_format); // Init camera and UART
	/* Here we are supporting output format 257 || 258 || 1 || 2 */
	/* Format validation done in Init function and if fails then it returns non zero value */

    std::this_thread::sleep_for(250ms);

	if (Ret == 0)
	{	
		std::thread listen (ListenWaitThread);

		while(ExitApp == 0){
			std::this_thread::sleep_for(10ms);
		}
	}

	std::cout<< "B5L Testing version 0.2 exiting"<< std::endl;
	
	Listener.Close();

	CTOFSample::Stop();

	return 0;
}