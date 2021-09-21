/*
 * clsFTPClient.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: Jack Bowling
 */

#include "clsFTPClient.h"
clsFTPClient FTP;

bool clsFTPClient::Configure(){
	if(! this->ClientSocket.ConfigureFTP()) return false;
	return true;
}

bool clsFTPClient::Open(const char* user,const char* password){
//	220 Tornado-vxWorks (VxWorks5.5.1) FTP server ready
//	USER melfa
//	331 Password required
//	PASS robot
//	230 User logged in

	std::string data;

	if(! this->ClientSocket.Connect()) return false;
	if(this->ClientSocket.Read() > 0){
		if(strstr(this->ClientSocket.recBuffer, "220") != &this->ClientSocket.recBuffer[0]) return false;
	}else return false;

	data = "USER " + (std::string)user + "\n";
	if(! this->ClientSocket.Write((char*)data.c_str())) return false;
	if(this->ClientSocket.Read() > 0){
		if(strstr(this->ClientSocket.recBuffer, "331") != this->ClientSocket.recBuffer) return false;
	}else return false;

	data = "PASS " + (std::string)password + "\n";
	if(! this->ClientSocket.Write((char*)data.c_str())) return false;
	if(this->ClientSocket.Read() > 0){
		if(strstr(this->ClientSocket.recBuffer, "230") != this->ClientSocket.recBuffer) return false;
	}else return false;

	return true;
}

bool clsFTPClient::RecieveFile(char* FileName){
//	TYPE I
//	200 Type set to I, binary mode
//	PORT 10,42,50,2,15,239
//	200 Port set okay
//	SIZE /robprg/dat/1.MB5
//	213 17896
//	RETR /robprg/dat/1.MB5
//	150 Opening BINARY mode data connection
//	226 Transfer complete
//	221 You could at least say goodbye.

	std::string data;

	ListenSocket.NameIP=ClientSocket.NameIP;
	ListenSocket.Port=ClientSocket.Port;
	if(! ListenSocket.Configure()) return false;

	data = "TYPE I\n";
	if(! this->ClientSocket.Write((char*)data.c_str())) return false;
	if(this->ClientSocket.Read() > 0){
		if(strstr(this->ClientSocket.recBuffer, "200") != this->ClientSocket.recBuffer) return false;
	}else return false;

	struct sockaddr_in sin;
	socklen_t len = sizeof(sin);
	if (getsockname(ClientSocket.SocketNum, (struct sockaddr *)&sin, &len) == -1) return false;

	int port = ntohs(sin.sin_port);
	char sPort[4];
	sprintf(&sPort[0],"%02x",port);
	if(strlen(sPort) != 4) return false;

	unsigned char cHigh = val(sPort[0]) * 16 + val(sPort[1]);
	unsigned char cLow = val(sPort[2]) * 16 + val(sPort[3]);
	if(cLow == 255) cLow = 2;

	char sHigh[4];
	char sLow[4];
	sprintf(&sHigh[0], "%d", cHigh);
	sprintf(&sLow[0], "%d", cLow);

	data = "PORT " + (std::string)this->ServerSocket.NameIP;
	for(uint32_t i=0;i < data.size();i++){
		if(data[i] == '.') data[i] = ',';
	}
	data += "," + (std::string)sHigh + "," + (std::string)sLow + "\n";

	if(! this->ClientSocket.Write((char*)data.c_str())) return false;
	if(this->ClientSocket.Read() > 0){
		if(strstr(this->ClientSocket.recBuffer, "200") != this->ClientSocket.recBuffer) return false;
	}else return false;

	long FileSize = 0;
	data = "SIZE " + (std::string)FileName + "\n";
	if(! this->ClientSocket.Write((char*)data.c_str())) return false;
	if(this->ClientSocket.Read() > 0){
		if(strstr(this->ClientSocket.recBuffer, "213") != this->ClientSocket.recBuffer) return false;
		char* ptr;
		FileSize = strtol((const char*)&this->ClientSocket.recBuffer[4], &ptr ,10);
		if(FileSize <= 0) return false;
	}else return false;

	data = "RETR " + (std::string)FileName + "\n";
	if(! this->ClientSocket.Write((char*)data.c_str())) return false;


	this->ServerSocket.Port = port;
	if(! this->ServerSocket.ConfigureFTP()) return false;
	if(! this->ServerSocket.Listen(&ListenSocket)) return false;

	if(this->ClientSocket.Read() > 0){
		if(strstr(this->ClientSocket.recBuffer, "150") != this->ClientSocket.recBuffer) return false;
	}else return false;

	unsigned long ulSize = ListenSocket.Read(FileSize);
	if(ulSize <= 0) return false;

	if(this->ClientSocket.Read() > 0){
		if(strstr(this->ClientSocket.recBuffer, "226") != this->ClientSocket.recBuffer) return false;
	}else return false;

	return true;
}



