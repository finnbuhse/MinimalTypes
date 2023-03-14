#pragma once
#include "Utils.h"

struct KeyHasher
{
	template <typename K>
	std::size_t operator()(const K& key) const
	{
		return hash<K>(key);
	}
};

struct KeyEqual
{
	template <typename K>
	bool operator()(const K& key1, const K& key2) const
	{
		return key1 == key2;
	}
};

/* Hash map which ensures accessed data is stored contiguously. 
   However indices and memory locations are not constant and change when elements are added/removed. */
template <typename K, typename T>
class ContiguousMap
{
public:
	void insert(const K& key, const T& element)
	{
		assert(("[ERROR] ContiguousMap entry with specified key already used", mIndexMap.find(key) == mIndexMap.end()));

		unsigned long long index = mElements.size();
		mIndexMap.insert({ key, index });
		mElements.push_back(element);
	}

	void remove(const K& key)
	{
		typename std::unordered_map<K, unsigned long long, KeyHasher, KeyEqual>::iterator indexIterator = mIndexMap.find(key);
		assert(("[ERROR] Attempted to remove non-existent ContiguousMap entry", indexIterator != mIndexMap.end()));

		// Find relation where it's index accesses the last element in 'mElements', and assign the index to be the index of the component being removed
		for (typename std::unordered_map<K, unsigned long long, KeyHasher, KeyEqual>::iterator it = mIndexMap.begin(); it != mIndexMap.end(); it++)
		{
			if (it->second == mElements.size() - 1)
			{
				it->second = indexIterator->second;
				break;
			}
		}
		mElements[indexIterator->second] = mElements.back(); // Overwrite element being removed with last element. Ensures no gaps

		// Erase last element
		mElements.pop_back();

		// Erase entry from index map
		mIndexMap.erase(indexIterator);
	}

	unsigned long long index(const K& key) const
	{
		#ifndef NDEBUG
		typename std::unordered_map<K, unsigned long long, KeyHasher, KeyEqual>::iterator indexIterator = mIndexMap.find(key);
		assert(("[ERROR] Attempted to get index of non-existent ContiguousMap entry", indexIterator != mIndexMap.end()));
		#endif

		return mIndexMap.at(key);
	}

	T& operator[](const K& key)
	{
		#ifndef NDEBUG
		typename std::unordered_map<K, unsigned long long, KeyHasher, KeyEqual>::iterator indexIterator = mIndexMap.find(key);
		assert(("[ERROR] Attempted to get element of non-existent ContiguousMap entry", indexIterator != mIndexMap.end()));
		#endif

		return mElements[mIndexMap.at(key)];
	}

private:
	std::unordered_map<K, unsigned long long, KeyHasher, KeyEqual> mIndexMap;
	std::vector<T> mElements;
};


/* Hash map which ensures accessed data is stored as contiguously as possible while
   ensuring indices and memory locations are 'static' and don't change. */
template <typename K, typename T, size_t N>
class StaticMap
{
public:
	void insert(const K& key, const T& element)
	{
		assert(("[ERROR] StaticMap entry with specified key already used", mIndexMap.find(key) == mIndexMap.end()));

		unsigned long long index = freeIndexStack.back(); // Get new index from the back (top) of the free index stack
		freeIndexStack.pop_back(); // Pop the index off the stack
		if (freeIndexStack.empty()) // If the free index stack is empty; add next index to the stack
			freeIndexStack.push_back(index + 1);

		mIndexMap.insert({ key, index }); // Add entry to index map
		mElements[index] = element; // Copy new element into elements array
	}

	void remove(const K& key)
	{
		typename std::unordered_map<K, unsigned long long, KeyHasher, KeyEqual>::iterator indexIterator = mIndexMap.find(key);
		assert(("[ERROR] Attempted to remove non-existent StaticMap entry", indexIterator != mIndexMap.end()));

		// Erase element from elements - index becomes unused
		unsigned long long index = (*indexIterator).second;
		mElements[index] = T();
		freeIndexStack.push_back(index);

		// Erase entry from index map
		mIndexMap.erase(key);
	}

	unsigned long long index(const K& key) const
	{
		#ifndef NDEBUG
		typename std::unordered_map<K, unsigned long long, KeyHasher, KeyEqual>::iterator indexIterator = mIndexMap.find(key);
		assert(("[ERROR] Attempted to get index of non-existent ContiguousMap entry", indexIterator != mIndexMap.end()));
		#endif

		return mIndexMap.at(key);
	}

	T& operator[](const K& key)
	{
		#ifndef NDEBUG
		typename std::unordered_map<K, unsigned long long, KeyHasher, KeyEqual>::iterator indexIterator = mIndexMap.find(key);
		assert(("[ERROR] Attempted to get element of non-existent ContiguousMap entry", indexIterator != mIndexMap.end()));
		#endif

		return mElements[mIndexMap.at(key)];
	}

private:
	static std::vector<unsigned long long> freeIndexStack;

	std::unordered_map<K, unsigned long long, KeyHasher, KeyEqual> mIndexMap;
	T mElements[N];
};

template <typename K, typename T, size_t N>
std::vector<unsigned long long> StaticMap<K, T, N>::freeIndexStack = { 0 };