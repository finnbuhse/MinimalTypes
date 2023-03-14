#pragma once
#include "Utils.h"

template <size_t S>
struct String
{
	char buffer[S];

	unsigned int length()
	{
		return strlen((char*)buffer);
	}

	unsigned int size()
	{
		return S;
	}

	operator const char*()
	{
		return buffer;
	}
};

struct StringCreateInfo
{
	const char* buffer;

	template <size_t S>
	operator String<S>()
	{
		assert(("String literal too long for buffer", strlen(buffer) <= S));
		String<S> s;
		strcpy((char*)s.buffer, buffer);
		return s;
	}
};

StringCreateInfo str(const char* pStr);

template <size_t S>
void print(const String<S>& string)
{
	std::cout << string << std::endl;
}

template <size_t S>
std::vector<char> serialize(const String<S>& string)
{
	// Serializing a String will serialize its size first then the characters contained within it.
	std::vector<char> result;

	std::vector<char> vecData = serialize((unsigned int)string.size());
	result.insert(result.end(), vecData.begin(), vecData.end());

	result.insert(result.end(), string, string + string.size());
	return result;
}

template <size_t S>
void deserialize(const std::vector<char>& vecData, String<S>& write)
{
	// Deserializing a string will only yield the string itself and hence vecData should not contain the length of the string,
	// if this is desired, one should deserialize the length into a unsigned integer first then invoke this procedure
	write = str(std::vector<char>(vecData.begin(), vecData.end()).data());
}