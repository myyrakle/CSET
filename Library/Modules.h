// Types.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//
#include "stdafx.h"

#include <utility>
#include <string>
#include <stdint.h>

#define _CRT_SECURE_NO_WARNINGS

using std::string;
using std::to_string;

class Object;
class Char;
class String;
class Bool;
class Byte;

template<class FloatType>
class Float_Basic;

template<class IntType>
class Int_Basic;



class Object
{
public:
	virtual ~Object() = default;
};


class Char : public Object
{
private:
	wchar_t value = 0;
public:
	bool is_alpha() { return true ? true : false; }
	bool is_korean() { return true ? true : false; }
public:
	Char(wchar_t& c) : value(c) {}
	Char& operator=(wchar_t c) { value = c; }
public:
	operator const wchar_t&() const { return value; }
	operator wchar_t&() { return value; }
public:
	Char() = default;
	virtual ~Char() = default;
	Char(const Char&) = default;
	Char(Char&&) = default;
	Char& operator=(const Char&) = default;
	Char& operator=(Char&&) = default;
};


template<class IntType>
class Int_Basic : public Object
{
private:
	IntType value = 0;
public:
	Int_Basic(IntType in) :value(in) {}
	Int_Basic& operator=(IntType in) { value = in; }
public:
	operator const IntType&() const { return value; }
	operator IntType&() { return value; }
public:
	Int_Basic() = default;
	virtual ~Int_Basic() = default;
	Int_Basic(const Int_Basic&) = default;
	Int_Basic(Int_Basic&&) = default;
	Int_Basic& operator=(const Int_Basic&) = default;
	Int_Basic& operator=(Int_Basic&&) = default;
};
using Int = Int_Basic<int>;
using Uint = Int_Basic<unsigned int>;
using Int8 = Int_Basic<int8_t>;
using Int16 = Int_Basic<int16_t>;
using Int32 = Int_Basic<int32_t>;
using Int64 = Int_Basic<int64_t>;
using Uint8 = Int_Basic<uint8_t>;
using Uint16 = Int_Basic<uint16_t>;
using Uint32 = Int_Basic<uint32_t>;
using Uint64 = Int_Basic<uint64_t>;


template <class FloatType>
class Float_Basic : public Object
{
private:
	FloatType value = 0.0;
public:
	Float_Basic(FloatType f) : value(f) {}
	Float_Basic& operator=(FloatType f) { value = f; }
public:
	operator const FloatType&() const { return value; }
	operator FloatType&() { return value; }
public:
	Float_Basic() = default;
	virtual ~Float_Basic() = default;
	Float_Basic(const Float_Basic&) = default;
	Float_Basic(Float_Basic&&) = default;
	Float_Basic& operator=(const Float_Basic&) = default;
	Float_Basic& operator=(Float_Basic&&) = default;
public: //static
	static constexpr FloatType PI = FloatType(3.1415926535897932385);
};
using Float = Float_Basic<float>;
using Double = Float_Basic<double>;
using Ldouble = Float_Basic<long double>;


class Bool : public Object
{
private:
	bool value = false;
public:
	Bool(bool b) : value(b) {}
	Bool& operator=(bool b) { value = b; }
public:
	void flip() { value = !value; }
public:
	operator const bool&() const { return value; }
	operator bool&() { return value; }
public: //디폴트
	Bool() = default;
 	virtual ~Bool() = default;
	Bool(const Bool&) = default;
	Bool(Bool&&) = default;
	Bool& operator=(const Bool&) = default;
	Bool& operator=(Bool&&) = default;
};

class Byte : public Object
{
private:
	union {
		uint8_t value = 0;
		struct : public Object
		{
			uint8_t _1 : 1;
			uint8_t _2 : 1;
			uint8_t _3 : 1;
			uint8_t _4 : 1;
			uint8_t _5 : 1;
			uint8_t _6 : 1;
			uint8_t _7 : 1;
			uint8_t _8 : 1;
		};
	};
public:
	Byte(uint8_t in) :value(in) {}
	Byte& operator=(uint8_t in) { value = in; }
public:
	//String to_bits() const {return }
public:
	operator const uint8_t&() const { return value; }
	operator uint8_t&() { return value; } 
public: //디폴트
	Byte() = default;
	virtual ~Byte() = default;
	Byte(const Byte&) = default;
	Byte(Byte&&) = default;
	Byte& operator=(const Byte&) = default;
	Byte& operator=(Byte&&) = default;
};


class String : public Object
{
private:
	std::wstring value = L"";
public:
	String(const std::wstring& str) : value(str) {}
	String(const wchar_t* str) : value(std::wstring(std::move(str))) {}
	String(Char c) : value(std::move(std::wstring(1, (wchar_t)c))) {}
	template<class IntType>
	String(Int_Basic<IntType> v) : value(std::move(std::to_wstring((IntType)v))) {}
public:
	wchar_t& operator[](Uint pos) { return value[(unsigned int)pos]; }
	const wchar_t& operator[](Uint pos) const { return value[(unsigned int)pos]; }
	decltype(auto) length() const { return value.length(); }
	bool is_empty() const { return value.empty(); }
	bool not_empty() const { return !value.empty(); }
	bool includes(const String& str) const { return value.find((std::wstring)str) != std::wstring::npos; }
	void reserve() { value.reserve(); }
	Uint find(const String& str) const { return value.find((std::wstring)str); }
	Uint find(const Char& c) const { return value.find((wchar_t)c); }
	String front_string(Uint index) const 
	{ return String(value.substr(0, (unsigned int)index)); }
	String back_string(Uint index) const 
	{ return String(value.substr((unsigned int)index)); }
	String substring(Uint begin, Uint end) const { return String(value.substr((unsigned int)begin, (unsigned int)end)); }
	std::pair<String, String> split(Uint index) const
	{ return std::pair<String, String>(this->front_string(index), this->back_string(index)); }
public://이터레이터
	decltype(auto) begin() { return value.begin(); }
	decltype(auto) end() { return value.end(); }
	decltype(auto) begin() const { return value.begin(); }
	decltype(auto) end() const { return value.end(); }
	decltype(auto) rbegin() { return value.begin(); }
	decltype(auto) rend() { return value.end(); }
	decltype(auto) rbegin() const { return value.begin(); }
	decltype(auto) rend() const { return value.end(); }
public:
	
public: //디폴트 생성자들
	String() = default;
	~String() = default;
	String(const String& str) = default;
	String(String&& str) = default;
	String& operator=(const String& str) = default;
	String& operator=(String&& str) = default;
public: //묵시적 형변환
	operator std::wstring&() { return value; }
	operator const std::wstring&() const { return value; }
public: // extend Object
	String & operator+=(const String& rhs)
	{
		(std::wstring)value += (std::wstring)rhs;
	}
	String & operator+=(const Char& rhs)
	{
		(std::wstring)value += (wchar_t)rhs;
	}
	friend String operator+(const String& lhs, const String& rhs)
	{
		return (std::wstring)lhs + (std::wstring)rhs;
	}
	friend String operator+=(const String& lhs, const Char& rhs)
	{
		return ((std::wstring)lhs)+=((wchar_t)rhs);
	}
	friend String operator+=(const Char& lhs, const String& rhs)
	{
		return operator+(rhs, lhs);
	}
};


class Range : public Object
{
private:
	int head;
	int current_value = 0;
	int tail;
public:
	class iterator : public Object
	{
		using pointer = Range * ;
	private:
		pointer obj = nullptr;
	public:
		iterator(pointer p) : obj(p) { p->current_value = p->head; } //begin/rbegin 생성
		iterator(nullptr_t __) {} // end/rend 생성
		virtual ~iterator() = default;
		iterator() = delete;
		iterator(const iterator&) = default;
		iterator(iterator&&) = default;
		iterator& operator=(const iterator& rhs) = default;
		iterator& operator=(iterator&& rhs) = default;
	public:
		iterator& operator++() 
		{
			if (obj == nullptr) return *this;

			if (obj->current_value != obj->tail)
				(obj->current_value)++;
			else
				obj = nullptr;  
			return *this;
		}
		iterator operator++(int) { return this->operator++(); } //위임
		int& operator*() { return obj->current_value; }
		const int& operator*() const { return obj->current_value; }
		bool operator==(const iterator& rhs)
		{
			return this->obj == rhs.obj;
		}
		bool operator!=(const iterator& rhs)
		{
			return this->obj != rhs.obj;
		}
	};
	class reverse_iterator : public Object
	{
		using pointer = Range *;
	private:
		pointer obj = nullptr;
	public:
		reverse_iterator(pointer p) : obj(p) { p->current_value = p->tail; }
		reverse_iterator(nullptr_t __) {}
		virtual ~reverse_iterator() = default;
		reverse_iterator() = delete;
		reverse_iterator(const reverse_iterator&) = default;
		reverse_iterator(reverse_iterator&&) = default;
		reverse_iterator& operator=(const reverse_iterator&) = default;
		reverse_iterator& operator=(reverse_iterator&&) = default;
	public:
		reverse_iterator& operator++() 
		{
			if (obj == nullptr) return *this;

			if (obj->current_value != obj->tail)
				(obj->current_value)++;
			else
				obj = nullptr;
			return *this;
		}
		reverse_iterator operator++(int) { return this->operator++(); } //위임
		int& operator*() { return obj->current_value; }
		const int& operator*() const { return obj->current_value; }
		bool operator==(const reverse_iterator& rhs)
		{
			return this->obj == rhs.obj;
		}
		bool operator!=(const reverse_iterator& rhs)
		{
			return this->obj != rhs.obj;
		}
	};
public: //반복자
	Range::iterator begin()
	{
		current_value = head;
		return iterator(this);
	}
	Range::iterator end()
	{ return iterator(nullptr); }
public: //역순 반복자
	Range::reverse_iterator rbegin()
	{
		current_value = tail;
		return reverse_iterator(this);
	}
	Range::reverse_iterator rend()
	{ return reverse_iterator(nullptr); }
public:
	Range() = delete;
	virtual ~Range() = default;
	Range(int _lhs, int _rhs) : head(_lhs), tail(_rhs) {}
};


//console 모듈입니다,
namespace console
{
	#include <iostream>
	void putline() { putchar('\n'); }

	void put(const String& val) { wprintf(L"%s", ((std::wstring)val).c_str()); }
	void putline(const String& val) { put(val); putline(); }
	
	template<class IntType>
	void put(Int_Basic<IntType> val) {	printf("%d", (IntType)val); }
	template<class IntType>
	void putline(Int_Basic<IntType> val) { put(val); putline(); }
	
	void put(Char val) { putwchar((wchar_t)val); }
	void putline(Char val) { put(val); putline(); }
	
	void put(Float val) { printf("%f", (float)val); }
	void putline(Float val) { put(val); putline(); }
	
	void put(Double val) { printf("%f", (double)val); }
	void putline(Double val) { put(val); putline(); }

	void put(Bool val) { val ? printf("true") : printf("false"); }
	void putline(Bool val) { put(val); putline(); }
	
	void flush_out() { fflush(stdout); }
	void flush_in() { while (getchar() != EOF); }

	std::wstring getline() { std::wstring temp; fgetws((wchar_t*)temp.data(), INT_MAX, stdin); return temp; }
	std::wstring getword() { std::wstring temp; wscanf(L"%s", (wchar_t*)temp.data()); return temp; }
	wchar_t getc() { return wchar_t(getwchar()); }
	void get(Int& value) { wscanf(L"%d", &(int&)value); }
	void get(Char& value) { wscanf(L"%c", &(wchar_t&)value); }
	void get(Float& value) { wscanf(L"%f", &(float&)value); }
	void get(Double& value) { wscanf(L"%lf", &(double&)value); }
	
	void clear() { system("cls"); }
	void pause() { system("pause"); }
	void backspace() { putchar('\b'); }
}
// console 모듈입니다.



//collection 모듈입니다.
namespace collection
{
	#include <array>
	#include <vector>
	#include <list>
	#include <stack>
	#include <queue>
	#include <map>
	#include <set>
	#include <unordered_map>
	#include <unordered_set>
	#include <algorithm>

	class Collection : public Object
	{

	};

	template <class Type, size_t Length>
	class Array : public Object
	{
	private:
		std::array<Type, Length> value;
	public:
		constexpr size_t length() const { return Length; }
		Type& operator[](size_t index) { return value[index]; }
		const Type& operator[](size_t index) const { return value[index]; }
	};

	template <class Type>
	class Lazyarray : public Object
	{
	private:
		Type * value = nullptr;
		size_t len = 0;
	public:
		Lazyarray(size_t _length) : value(new Type[_length]), len(_length) {}
		virtual ~Lazyarray() { delete[] value; }
	public:
		size_t length() const { return len; }
		Type& operator[](size_t index) { return value[index]; }
		const Type& operator[](size_t index) const { return value[index]; }
	};

	template <class Type>
	class Vector : public Collection
	{
	private:
		std::vector<Type> value;
	};
}
// collection 모듈입니다.


int main()
{
	sqrt(3);
	Range range(0, 15);
	for (auto i = range.rbegin(); i != range.rend(); i++)
		std::cout << *i << std::endl;

	return 0;
}
