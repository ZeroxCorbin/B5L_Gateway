#include <unistd.h>
#include <iostream>
#include <ostream>
#include <chrono>
#include <thread>

#if _WIN32

#else
	#include <sys/stat.h>
#endif

#include <pcl/io/pcd_io.h>

#include "clsTCPSocket.hpp"
#include "ToF_Sample.hpp"

/* Config file string */
#define CONFIG_FILE_PATH "/src/config/ToF_Sample.prm"

bool SaveImageFile();
void SendImageFile(clsTCPSocket *);
void ClientConnectedThread(clsTCPSocket *);
void ListenWaitThread();