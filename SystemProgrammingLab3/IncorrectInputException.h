#pragma once
#include <exception>

class IncorrectInputException : public std::exception
{
public:
	IncorrectInputException(const char* in) : std::exception(in) {}
};

