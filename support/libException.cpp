/*
 * libException.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: Jack Bowling
 */

#include "libException.h"

void UpdateUser(char* Message, long Code){
	std::cout << "Exception occurred:\n;;";
	std::cout << Code;
	std::cout << ";;\n";
	std::cout << Message << "\n";
}

void UpdateUser(char* Message){
	std::cout << Message << "\n";
}

void UpdateUser(const std::string Message, long Code){
	std::cout << "Exception occurred:\n;;";
	std::cout << Code;
	std::cout << ";;\n";
	std::cout << Message << "\n";
}

void UpdateUser(const std::string Message){
	std::cout << Message.c_str() << "\n";
}

void UpdateUser(std::stringstream& Message){
	std::cout << Message.str() << "\n";
}
