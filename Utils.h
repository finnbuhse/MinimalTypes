#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <assert.h>
#include <iostream>

// FILE IO

std::vector<char> readFile(const char* directory);

void writeFile(const char* directory, std::vector<char> content);

// PRINT

void print(const std::string& string);

void print(const std::vector<char>& string);

template <typename K, typename T>
void print(const std::unordered_map<K, T>& map)
{
	for (const std::pair<K, T>& pair : map)
		std::cout << pair.first << ": " << pair.second << "\n";
}

// VECTOR STRING OPERATIONS

int find(const std::vector<char>& string, const std::vector<char>& find, const unsigned int& start = 0);

std::vector<std::vector<char>> split(const std::vector<char>& string, const std::vector<char>& split);

unsigned int countCharacter(const std::vector<char>& string, const char& character);
unsigned int countCharacter(const std::string& string, const char& character);

// HASHING

template <class T>
std::size_t hash(const T& value)
{
	return std::hash<T>()(value);
}

template <class T>
inline void hashCombine(std::size_t& hash, const T& value)
{
	hash ^= std::hash<T>()(value) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
}

// SERIALIZING

template <typename T>
std::vector<char> serialize(const T& data)
{
	const char* byteData = reinterpret_cast<const char*>(&data);
	return std::vector<char>(byteData, byteData + sizeof(T));
}

template <>
std::vector<char> serialize(const std::string& string); // Returns byte data containing the string's length followed by the string

template <typename T>
void deserialize(const std::vector<char>& vecData, T& write)
{
	write = *(reinterpret_cast<const T*>(vecData.data()));
}

template <>
void deserialize(const std::vector<char>& vecData, std::string& write); // Deserializes a string given that vecData has the same length as the string