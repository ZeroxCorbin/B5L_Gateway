/*
 * TCPSocket.cpp
 *
 *  Created on: Dec 16, 2014
 *      Author: Jack Bowling
 */
/************TCPSocket.c************************/

#include "clsTCPSocket.hpp"

bool clsTCPSocket::Configure(){

	try{
		memset(&IP, 0x00, sizeof(struct sockaddr_in));
#if WIN32
		IP.sin_family = PF_INET;
#else
		IP.sin_family = AF_INET;
#endif

		if((Port <= 0) | (Port >= 65535)) throw SocketException("Invalid port number specified", 8007);
		IP.sin_port = htons(Port);

		if(strcmp(NameIP, "") != 0 ){
			if((IP.sin_addr.s_addr = inet_addr(NameIP)) == (unsigned long)INADDR_NONE){
				HostName = gethostbyname(NameIP);
				if(HostName == (struct hostent *)NULL) throw SocketException("Invalid host name or IP address specified", 8000);

				memcpy(&IP.sin_addr, HostName->h_addr, sizeof(IP.sin_addr));
			}	
		}
		else{
			IP.sin_addr.s_addr = INADDR_ANY;
		}
		
		#if WIN32
		WSADATA wsaData;
		if(WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) throw SocketException ("Could not load Winsock", 8001);
		this->SocketNum = socket(IP.sin_family, SOCK_STREAM, IPPROTO_TCP);
#else
		this->SocketNum = socket(IP.sin_family, SOCK_STREAM, 0);
#endif
		if(this->SocketNum <= 0) throw SocketException ("Could not create a socket", 8002);

		return true;

	}catch(SocketException& ex){
		UpdateUser((char*)ex.description().c_str(), ex.code());
		return false;
	}
}

bool clsTCPSocket::ConfigureFTP(void){

	try{
		memset(&IP, 0x00, sizeof(struct sockaddr_in));

#if WIN32
		IP.sin_family = AF_INET;
#else
		IP.sin_family = AF_INET;
#endif
		if((Port <= 0) | (Port >= 65535)) throw SocketException("Invalid port number specified", 8007);
		IP.sin_port = htons(Port);

		if((IP.sin_addr.s_addr = inet_addr(NameIP)) == (unsigned long)INADDR_NONE){
			HostName = gethostbyname(NameIP);
			if(HostName == (struct hostent *)NULL) throw SocketException("Invalid host name or IP address specified", 8000);

			memcpy(&IP.sin_addr, HostName->h_addr, sizeof(IP.sin_addr));
		}

#if WIN32
		WSADATA wsaData;
		if(WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) throw SocketException ("Could not load Winsock", 8001);
		this->SocketNum = socket(IP.sin_family, SOCK_STREAM, IPPROTO_TCP);
#else
		this->SocketNum = socket(IP.sin_family, SOCK_STREAM, 0);
#endif
		if(this->SocketNum <= 0) throw SocketException ("Could not create a socket", 8002);

		return true;
	}catch(SocketException& ex){
		UpdateUser((char*)ex.description().c_str(), ex.code());
		return false;
	}
}

bool clsTCPSocket::Listen(clsTCPSocket* client){
	try {
		if(IsBound == 0)
			if(bind(this->SocketNum, (struct sockaddr *) &this->IP, sizeof(IP)) < 0) throw SocketException ("Could not bind the socket",8003);
			
		IsBound = 1;
		listen(this->SocketNum,1);

#if WIN32
		//int clilen = sizeof(sockaddr);
		client->SocketNum = accept(this->SocketNum, NULL, NULL); //(struct sockaddr *) &client->IP.sin_addr.s_addr, &clilen);
#else
		socklen_t clilen = sizeof(client->IP);
		client->SocketNum = accept(this->SocketNum, (struct sockaddr *) &client->IP, &clilen);
#endif
		if(client->SocketNum <= 0) throw SocketException ("Could not create the client listen socket", 8004);

		return true;
	}catch(SocketException& ex) {
		if(client->SocketNum > 0) Close(client->SocketNum);
		UpdateUser((char*)ex.description().c_str(), ex.code());
		return false;
	}
	catch(...) {
		if(client->SocketNum > 0) Close(client->SocketNum);
		UpdateUser((char*)"Unknown Listen() Error", 8021);
		return false;
	}
}

int clsTCPSocket::Connect()
{
	int rc = sizeof(int);
	try{

		//tightvnc
		//int flags =1;
		//setsockopt(this->SocketNum, SOL_TCP, TCP_NODELAY, (void *)&flags, sizeof(flags));

		uint32_t clilen = sizeof(this->IP);
		rc = connect(this->SocketNum, (struct sockaddr *)&this->IP, clilen);
		if(!(rc==0)) throw SocketException ("Could not connect to client", 8005);

		return true;
	}catch(SocketException& ex){
		UpdateUser((char*)ex.description().c_str(), ex.code());
		return false;
	}catch(...){
		UpdateUser((char*)"Unknown Connect() Error", 8020);
		return false;
	}
}

int clsTCPSocket::Write(const char* sendData, int dataLength)
{
	int rc = sizeof(int);
	try{
		if(dataLength == 0){
			dataLength = strlen(sendData);
		}
		rc = send(this->SocketNum, sendData, dataLength,0);

		if(rc < 0){
			int  length = sizeof(int);
			char temp;
#if WIN32
			rc = getsockopt(this->SocketNum, SOL_SOCKET, SO_ERROR, &temp, &length);
#else
			rc = getsockopt(this->SocketNum, SOL_SOCKET, SO_ERROR, &temp, (socklen_t*)&length);
#endif
			if(rc == 0){
				throw SocketException((const char*)&temp, 8025);
			}else throw SocketException("Unknown Write() Error", 8022);
		}

		return rc;
	}catch(SocketException& ex){
		UpdateUser((char*)ex.description().c_str(), ex.code());
		return false;
	}catch(...){
		UpdateUser((char*)"Unknown Write() Error", 8022);
		return false;
	}
}

bool clsTCPSocket::HasData(){
	int count;
	ioctl(this->SocketNum, FIONREAD, &count);
	if(count > 0)
		return true;
	else
		return false;
}

long clsTCPSocket::Read(long dataLen){
	int  rc, length = sizeof(int);

	if(dataLen <= 0) dataLen = REC_BUFFER_LENGTH;

	length = 0;
	try{
		while(true){

			//Peek at the receive buffer
			//rc = recv(this->SocketNum, &this->recBuffer[0], dataLen-length,MSG_PEEK);
			//if(rc > 0) 
				rc = recv(this->SocketNum, &this->recBuffer[0], dataLen-length,0);

			if(rc < 0){
				throw SocketException("Data read error", 8010);
			}
			else if (rc == 0){
				throw SocketException("Remote server closed the connection", 8006);
			}
			else{
				length += rc;
				this->recBuffer[rc] = 0;
			}

			if(length >= dataLen) break;
			if(dataLen == REC_BUFFER_LENGTH) break;
		}

		this->recBuffer[length] = 0;
		return length;
	}catch(SocketException& ex){
		Close();
		UpdateUser((char*)ex.description().c_str(), ex.code());
		return -1;
	}catch(...){
		Close();
		UpdateUser((char*)"Unknown Read() Error", 8023);
		return -1;
	}
}


void clsTCPSocket::Close(int socketNumber){
	if(socketNumber <= 0) socketNumber = this->SocketNum;
	if(socketNumber <= 0) return;
#ifdef WIN32
	::closesocket(socketNumber);
#else
	::close(socketNumber);
#endif
}





