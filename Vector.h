#pragma once
#include <cstring>
#include <cassert>
#include <iostream>

// POD dynamic array
template <typename T>
struct Vector
{
	T* data;
	unsigned int size; // Size of allocated space in bytes
	unsigned int length; // Number of elements

	void initialize(const unsigned int& nElements = 0)
	{
		size = nElements;
		data = new T[size];
		length = nElements;
	}

	void free()
	{
		delete[] data;
		data = nullptr;
	}

	void push(const T& element) 
	{
		length++;
		if (length > size)
		{
			size = length;
			T* temp = new T[size];
			memmove(temp, data, sizeof(T) * (length - 1));
			delete[] data;
			data = temp;
		}
		data[length - 1] = element;
	}

	void pop()
	{
		assert(("Cannot pop if length is 0", length > 0));
		length--;
	}

	void remove(const unsigned int& index)
	{
		assert(("Vector index out of range", index < length));
		length--;
		memmove(data + index, data + index + 1, sizeof(T) * (length - index));
	}

	T& operator [](const unsigned int& index)
	{
		assert(("Vector index out of range", index < length));
		return data[index];
	}

	T operator [](const unsigned int& index) const
	{
		assert(("Vector index out of range", index < length));
		return data[index];
	}

	T& back()
	{
		assert(("No back element", length != 0));
		return data[length - 1];
	}

	void reserve(const unsigned int& nElements)
	{
		assert(("Cannot reserve less than or equal to length elements", nElements > length));
		size = nElements;
		T* temp = new T[size];
		memmove(temp, data, sizeof(T) * length);
		delete[] data;
		data = temp;
	}

	void resize(const unsigned int& nElements)
	{
		reserve(nElements);
		length = nElements;
	}

	void clear()
	{
		delete[] data;
		size = 0;
		data = new T[0];
		length = 0;
	}

	int find(const T& element)
	{
		for (unsigned int i = 0; i < length; i++)
		{
			if (data[i] == element)
				return i;
		}
		return -1;
	}
};

template <typename T>
struct ListElement
{
	ListElement* previous = nullptr;
	ListElement* next = nullptr;

	T value;

	~ListElement()
	{
		if (next)
			delete next;
	}
};

template <typename T>
struct SortedList;

// Possesses constructor - not POD
template <typename T>
struct List
{
protected:
	friend SortedList<T>;

	ListElement<T>* mFirst = nullptr;
	ListElement<T>* mLast = nullptr;
	unsigned int mLength = 0;

public:
	List() {}

	List(const unsigned int& length) : mLength(length)
	{
		ListElement<T>* element = mFirst = new ListElement<T>{};
		for (unsigned int i = 1; i < mLength; i++)
		{
			element->next = new ListElement<T>{ element };
			element = element->next;
		}
		mLast = element;
	}

	List(T* arr, const unsigned int& length) : mLength(length)
	{
		ListElement<T>* element = mFirst = new ListElement<T>{};
		element->value = arr[0];
		for (unsigned int i = 1; i < mLength; i++)
		{
			element->value = arr[i];
			element->next = new ListElement<T>{ element };
			element = element.next;
		}
		mLast = element;
	}

	~List()
	{
		if(mFirst)
			delete mFirst;
	}

	void push(const T& value)
	{
		if (mLength == 0)
			mLast = mFirst = new ListElement<T>{ nullptr, nullptr, value };
		else
			mLast = mLast->next = new ListElement<T>{ mLast, nullptr, value };
			
		mLength++;
	}

	T& pushr(const T& value) // Push return
	{
		if (mLength == 0)
			mLast = mFirst = new ListElement<T>{ nullptr, nullptr, value };
		else
			mLast = mLast->next = new ListElement<T>{ mLast, nullptr, value };

		mLength++;
		return mLast->value;
	}

	void pop()
	{
		mLast = mLast->previous;
		delete mLast->next;
		mLength--;
	}

	void remove(const unsigned int& index)
	{
		assert(("List index out of range", index < mLength));

		ListElement<T>* element = mFirst;
		for (unsigned int i = 0; i < index; i++)
			element = element->next;

		bool first = element == mFirst;
		bool last = element == mLast;
		if (first)
		{
			mFirst = mFirst->next;
			if(mFirst)
				mFirst->previous = nullptr;
		}	
		if (last)
		{
			mLast = mLast->previous;
			if(mLast)
				mLast->next = nullptr;
		}
		if (!first && !last)
		{
			element->previous->next = element->next;
			element->next->previous = element->previous;
		}

		element->next = nullptr;
		delete element;

		mLength--;
	}

	unsigned int length() const { return mLength; }

	T& operator [](const unsigned int& index)
	{
		assert(("List index out of range", index < mLength));
		ListElement<T>* element = mFirst;
		for (unsigned int i = 0; i < index; i++)
			element = element->next;
		return element->value;
	}

	T operator [](const unsigned int& index) const
	{
		assert(("List index out of range", index < mLength));
		ListElement<T>* element = mFirst;
		for (unsigned int i = 0; i < index; i++)
			element = element->next;
		return element->value;
	}

	int find(const T& value) const
	{
		ListElement<T>* element = mFirst;
		for (unsigned int i = 0; i < mLength; i++)
		{
			if (element->value == value)
				return i;
			element = element->next;
		}
		return -1;
	}

	void print() const
	{
		if (mLength > 0)
		{
			std::cout << mFirst->value;
			ListElement<T>* element = mFirst;
			for (unsigned int i = 1; i < mLength; i++)
			{
				element = element->next;
				std::cout << ", " << element->value;
			}
			std::cout << std::endl;
		}
	}
};

template <typename T>
struct SortedList
{
private:
	List<T> mList;

public:
	void push(const T& value)
	{
		if (mList.mLength == 0)
		{
			mList.push(value);
			return;
		}

		ListElement<T>* element = mList.mFirst;
		while (element->value < value)
		{
			if (element->next == nullptr)
			{
				mList.push(value);
				return;
			}
			element = element->next;
		}
		if (element == mList.mFirst)
			mList.mFirst = new ListElement<T>{ nullptr, mList.mFirst, value };
		else
			element->previous = element->previous->next = new ListElement<T>{ element->previous, element, value };

		mList.mLength++;
	}

	T& pushr(const T& value)
	{
		if (mList.mLength == 0)
			return mList.pushr(value);

		ListElement<T>* element = mList.mFirst;
		while (element->value < value)
		{
			if (element->next == nullptr)
				return mList.pushr(value);
			element = element->next;
		}
		if (element == mList.mFirst)
		{
			mList.mFirst = new ListElement<T>{ nullptr, mList.mFirst, value };
			mList.mLength++;
			return mList.mFirst->value;
		}
		else
		{
			element->previous = element->previous->next = new ListElement<T>{ element->previous, element, value };
			mList.mLength++;
			return element->previous->value;
		}
	}

	void remove(const unsigned int& index)
	{
		mList.remove(index);
	}

	unsigned int length() const { return mList.mLength; }

	T& operator [](const unsigned int& index)
	{
		return mList[index];
	}

	T operator [](const unsigned int& index) const
	{
		return mList[index];
	}

	int find(const T& value)
	{
		return mList.find(value);
	}

	void print() const
	{
		mList.print();
	}
};