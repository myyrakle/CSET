#pragma once

#include "basic.h"

#include <vector>
template <class T>
class Vector : public Collection
{
private:
	std::vector<T> value;
public:
	
};

#include <list>
template <class T>
class List : public Collection
{
private:
	std::list<T> value;
public:

};

#include <map>
template <class K, class V>
class TreeMap
{
private:
	std::map<K, V> value;
public:
	V& operator[](const K& index)
	{ return value[K]; }
	const V& operator[](const K& index) const
	{ return value[K]; }
};

#include <set>
template <class T>
class TreeSet
{
private:
	std::set<T> value;
public:
};

#include <unordered_map>
template <class K, class V>
class HashMap
{
private:
	std::unordered_map<K, V> value;
public:
	V & operator[](const K& index)
	{ return value[K]; }
	const V& operator[](const K& index) const
	{ return value[K]; }
};

#include <unordered_set>
template <class T>
class HashSet
{
private:
	std::unordered_set<T> value;
public:
};