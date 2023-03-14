#pragma once
#include "Vector.h"
#include "String.h"
#include "Map.h"

// Deserializing a string will only yield the string itself and hence vecData should not contain the length of the string,
// if this is desired, one should deserialize the length into a unsigned integer first then invoke this procedure

#define MINIMAL_TYPES_INCLUDE "MinimalTypesCPPInclude.h"

#define MINIMAL_TYPES_CPP \
StringCreateInfo str(const char* pStr) \
{ \
	StringCreateInfo sci; \
	sci.buffer = pStr; \
	return sci; \
} \
std::vector<char> readFile(const char* directory) \
{ \
	std::ifstream file(directory, std::ios::binary | std::ios::ate); \
	assert(("[ERROR] Failed to open file", file.is_open())); \
	std::vector<char> content(file.tellg()); \
	file.seekg(0); \
	file.read(content.data(), content.size()); \
	file.close(); \
	return content; \
} \
void writeFile(const char* directory, std::vector<char> content) \
{ \
	std::ofstream file(directory, std::ios::binary); \
	assert(("[ERROR] Failed to open file", file.is_open())); \
	file.write(content.data(), content.size()); \
	file.close(); \
} \
void print(const std::string& string) \
{ \
	std::cout << string << std::endl; \
} \
void print(const std::vector<char>& string) \
{ \
	std::cout << string.data() << std::endl; \
} \
int find(const std::vector<char>& string, const std::vector<char>& find, const unsigned int& start) \
{ \
	unsigned nFoundCharacters = 0; \
	for (unsigned int i = start; i < string.size(); i++) \
	{ \
		if (string[i] == find[nFoundCharacters]) \
		{ \
			nFoundCharacters++; \
			if (nFoundCharacters == find.size()) \
				return i - nFoundCharacters + 1; \
		} \
		else \
			nFoundCharacters = 0; \
	} \
	return -1; \
} \
std::vector<std::vector<char>> split(const std::vector<char>& string, const std::vector<char>& split) \
{ \
	std::vector<std::vector<char>> result; \
	unsigned int beginElement = 0; \
	int endElement; \
	while (true) \
	{ \
		endElement = find(string, split, beginElement); \
		if (endElement == -1) \
		{ \
			std::vector<char> subStr(string.begin() + beginElement, string.end()); \
			result.push_back(subStr); \
			return result; \
		} \
		std::vector<char> subStr(string.begin() + beginElement, string.begin() + endElement); \
		result.push_back(subStr); \
		beginElement = endElement + split.size(); \
	} \
} \
unsigned int countCharacter(const std::vector<char>& string, const char& character) \
{ \
	unsigned int count = 0; \
	for (const char& c : string) \
	{ \
		if (c == character) \
			count++; \
	} \
	return count; \
} \
unsigned int countCharacter(const std::string& string, const char& character) \
{ \
	unsigned int count = 0; \
	for (const char& c : string) \
	{ \
		if (c == character) \
			count++; \
	} \
	return count; \
} \
template <> \
std::vector<char> serialize(const std::string& string) \
{ \
	std::vector<char> result; \
	std::vector<char> vecData = serialize((unsigned int)string.size()); \
	result.insert(result.end(), vecData.begin(), vecData.end()); \
	result.insert(result.end(), string.c_str(), string.c_str() + string.size()); \
	return result; \
}
template <> \
void deserialize<std::string>(const std::vector<char>& vecData, std::string& write) \
{ \
	write = std::string(vecData.data(), vecData.data() + vecData.size()); \
}