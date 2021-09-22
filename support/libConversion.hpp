/*
 * libConversion.h
 *
 * Created: 5/3/2014 11:37:46 PM
 *  Author: Jack Bowling
 */ 

#ifndef LIBCONVERSION_H_
#define LIBCONVERSION_H_

#include <cstdio>
#include <cstdlib>
#include <sstream>

#include <vector>

int str_to_int(const char *);
unsigned char val(char);
std::string decode(std::string const &);

#endif
