/*
 * TCPSocket.h
 *
 *  Created on: Dec 16, 2014
 *      Author: Jack Bowling
 */

#ifndef LINCLUDES_CLSTCPSOCKET_H_
#define LINCLUDES_CLSTCPSOCKET_H_

#include "libConversion.h"
#include "libException.h"

#include <cstdio>
#include <cstdlib>
#include <memory>
#include <iostream>
#include <sstream>
#include <cstring>
#include <sys/types.h>

#if WIN32
#include <windows.h>
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#include <vector>
#include <unistd.h>

/* BufferLength is 100 bytes */
#define REC_BUFFER_LENGTH 50000
//#define SND_BUFFER_LENGTH 500
/* Default host name of server system. Change it to your default */
/* server hostname or IP.  If the user do not supply the hostname */
/* as an argument, the_server_name_or_IP will be used as default*/
//#define SERVER "127.0.0.1"
/* Server's port number */
//#define SERVPORT 10302


class clsTCPSocket{
private:
	//char sndBuffer[SND_BUFFER_LENGTH];

public:
	const char* NameIP;
	struct hostent *HostName;

	struct sockaddr_in IP;

	int Port;
	int SocketNum;
	int IsBound;
	char recBuffer[REC_BUFFER_LENGTH];
	//char sndBuffer[REC_BUFFER_LENGTH];

	int Connect(void);
	bool Listen(clsTCPSocket*);
	void Close(int = 0);

	bool Configure(void);
	bool ConfigureFTP(void);
	int Write(char*);
	long Read(long = 0);
};

#endif /* LINCLUDES_TCPSOCKET_H_ */
