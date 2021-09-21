#include <unistd.h>
#include <iostream>
#include <chrono>
#include <thread>

#include <pcl/io/pcd_io.h>

#include "main.h"
#include "clsTCPSocket.h"

using namespace std::chrono_literals;

/* Output formate */
INT32 Tof_output_format = 0;

int ExitApp = 0;


clsTCPSocket *Listener;

int IsListening = 0;
int ClientAccept = 0;
int ClientConnected = 0;

void GetImageFile(){
	std::cout<< "Reading image and saving to file."<< std::endl;

	pcl::PointCloud<pcl::PointXYZI> cloud_;
	int Ret1 = CTOFSample::Run(&cloud_);
	if (Ret1 == 0)
	{
		const char *path="/home/zeroxcorbin/file.pcd";
		std::cout<< "File saved."<< std::endl;
		pcl::io::savePCDFileASCII (path, cloud_);
	}
}

void ClientConnectedThread(clsTCPSocket *client){
	std::cout<< "Client connected..."<< std::endl;

	while(true){
		long len = client->Read();
		if(len > 0){
			if(client->recBuffer[0] == 'g'){
				GetImageFile();
			}
			if(client->recBuffer[0] == 'exit'){
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


	std::cout<< "Listening..."<< std::endl;

	while(true){
		Listener = new(clsTCPSocket);

		Listener->NameIP = "";
		Listener->Port = 8890;
		
		if(!Listener->Configure()){
			ExitApp = 1;
			return;
		}

		clsTCPSocket *client = new (clsTCPSocket);

		if(Listener->Listen(client)){
			ClientConnectedThread(client);

			client->Close();
			Listener->Close();

			if(ExitApp == 1){
				break;
			}
		}else{
			std::cout<< "Listen Error..."<< std::endl;
			ExitApp = 1;

			client->Close();
			Listener->Close();

			delete(client);
			delete(Listener);

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
	
	Listener->Close();

	CTOFSample::Stop();

	return 0;
}