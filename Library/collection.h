#pragma once

#include "basic.h"

//collection 모듈입니다.

#include <array>
template <class T, size_t Length>
class Array;

template <class T>
class DynamicArray;

#include <vector>
template <class T>
class Vector;

#include <list>
template <class T>
class List;

#include <stack>
template <class T>
class Stack;

#include <queue>
template <class T>
class Queue;

#include <map>
template <class K, class V>
class TreeMap;

#include <set>
template <class T>
class TreeSet;

#include <unordered_map>
template <class K, class V>
class HashMap;

#include <unordered_set>
template <class T>
class HashSet;

#include <algorithm>

namespace collection
{
	class Collection : public Object
	{

	};



	template <class T, size_t Length>
	class Array : public Object
	{
	private:
		std::array<T, Length> value;
	public:
		constexpr size_t length() const
		{
			return Length;
		}

		T& operator[](size_t index)
		{
			return value[index];
		}
		const T& operator[](size_t index) const
		{
			return value[index];
		}
	};





	template <class T>
	class Vector : public Collection
	{
	private:
		std::vector<T> value;
	};

}

// collection 모듈입니다.
