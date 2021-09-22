/*
 * libException.h
 *
 *  Created on: Dec 18, 2014
 *      Author: Jack Bowling
 */

#ifndef ROBOTC_INCLUDE_LIBEXCEPTION_H_
#define ROBOTC_INCLUDE_LIBEXCEPTION_H_

// using standard exceptions
#include <iostream>
#include <exception>
#include <sstream>

class SocketException
{
public:
	SocketException ( std::string s, long c ): m_s (s), _code (c) {};
	~SocketException (){};

	std::string description() { return m_s; }
	long code() {return _code;}

private:
	std::string m_s;
	long _code;
};

class GeneralException
{
public:
	GeneralException ( std::string s, long c ): m_s (s), _code (c) {};
	~GeneralException (){};

	std::string description() { return m_s; }
	long code() {return _code;}

private:
	std::string m_s;
	long _code;
};

void UpdateUser(char*, long);
void UpdateUser(char*);
void UpdateUser(const std::string, long);
void UpdateUser(const std::string);
void UpdateUser(std::stringstream&);

#endif /* ROBOTC_INCLUDE_LIBEXCEPTION_H_ */
