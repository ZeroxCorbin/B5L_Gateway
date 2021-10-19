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
#include "d6t_32l.hpp"

/* Config file string */
#define CONFIG_FILE_PATH "/src/config/ToF_Sample.prm"

bool SaveImageFile();
void SendImageFile(clsTCPSocket *);
void ClientConnectedThread(clsTCPSocket *);
void ListenWaitThread();
bool D6TThread(uint8_t *);
