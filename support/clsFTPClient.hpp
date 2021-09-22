/*
 * clsFTPClient.h
 *
 *  Created on: Dec 18, 2014
 *      Author: Jack Bowling
 */

#ifndef ROBOTC_INCLUDE_CLSFTPCLIENT_H_
#define ROBOTC_INCLUDE_CLSFTPCLIENT_H_

#include <cstdlib>

#include "clsTCPSocket.hpp"

class clsFTPClient{
private:


public:
	clsTCPSocket ClientSocket;
	clsTCPSocket ServerSocket;
	clsTCPSocket ListenSocket;

	bool Configure();
	bool Open(const char*,const char*);
	bool RecieveFile(char*);
};
extern clsFTPClient FTP;

#endif /* ROBOTC_INCLUDE_CLSFTPCLIENT_H_ */
