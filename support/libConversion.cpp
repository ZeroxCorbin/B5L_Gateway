/*
 * libConversion.cpp
 *
 * Created: 5/3/2014 11:37:46 PM
 *  Author: Jack Bowling
 */ 

#include "libConversion.hpp"

int str_to_int(const char *s)
{
	int result = 0;
	for (; *s; ++s) {
		/* Only process recognized digits */
		if (isdigit((unsigned char)*s)) result = 10 * result + (*s - '0');
		else break;
	}
	return result;
}

unsigned char val(char c)
{
	if ('0' <= c && c <= '9') { return c - '0'; }
	if ('a' <= c && c <= 'f') { return c + 10 - 'a'; }
	if ('A' <= c && c <= 'F') { return c + 10 - 'A'; }
	throw "Eeek";
}
std::string decode(std::string const & s)
{
	if ((s.size() % 2) != 0) { throw "Eeek"; }

	std::string result;
	result.reserve(s.size() / 2);

	for (std::size_t i = 0; i < s.size(); i+=2)
	{
		unsigned char n = val(s[i]) * 16 + val(s[i + 1]);
		result += n;
	}

	return result;
}
