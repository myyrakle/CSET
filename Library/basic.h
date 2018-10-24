#pragma once

#include <utility>
#include <string>
#include <stdint.h>
#include <vector>


class Object; //최상위 클래스
class Char; //문자 타입
class String; //문자열 타입
class Bool; //논리 타입
class Byte; //바이트 타입

//실수 타입
template<class FloatType>
class Float_Basic;
using Float = Float_Basic<float>;
using Double = Float_Basic<double>;
using Ldouble = Float_Basic<long double>;

//정수 타입
template<class IntType>
class Int_Basic;
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

class Range; //범위 표현 컨테이너
class Nullable; //옵셔널

class Box; //소유권 기반 스마트포인터
class RcBox; //레퍼런스 카운팅 스마트포인터


//최상위 클래스입니다.
class Object
{
public:
	Object() = default;
	virtual ~Object() = default;
public:
	Object(const Object&) = default;
	Object(Object&&) = default;
	Object& operator=(const Object&) = default;
	Object& operator=(Object&&) = default;
public:
	virtual Object&& move() 
	{
		return std::move(*this);
	}
	virtual Uint get_size() const;
	virtual String get_typename() const;
	virtual String to_string() const;
};

//문자 타입입니다.
#include <cctype>
class Char : public Object
{
private:
	wchar_t value = 0;
public:
	bool is_alpha()  //알파벳인지 확인합니다.
	{
		return std::isalpha(value);
	}
	bool is_lower() const //알파벳 소문자인지 확인합니다.
	{
		return std::islower(value);
	}
	bool is_upper() const //알파벳 대문자인지 확인합니다.
	{
		return std::isupper(value);
	}
	bool is_digit() const //숫자 문자인지 확인합니다.
	{
		return std::isdigit(value);
	}
	bool is_whitespace() const //화이트스페이스인지 확인합니다.
	{
		return (value == ' ' || value == '\n' || value == '\t');
	}
	bool is_korean() const; //한글인지 확인합니다. 아직 구현하지 않았습니다.
public:
	Char(wchar_t c) : value(c) {}
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

public: //Object methods
	virtual Char&& move() override
	{
		return std::move(*this);
	}
	virtual Uint get_size() const override;
	virtual String get_typename() const override;
	virtual String to_string() const override;
};


//정수 타입의 템플릿입니다.
#include <limits>
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
public:
	static constexpr Int_Basic max() //미정의
	{
		return std::numeric_limits<IntType>::max();
	}
	static constexpr Int_Basic min()
	{
		return std::numeric_limits<IntType>::min();
	}
	static constexpr bool is_signed()
	{
		return std::numeric_limits<IntType>::is_signed();
	}
public:

public: //Object methods
	virtual Int_Basic&& move() override
	{
		return std::move(*this);
	}
	virtual Uint get_size() const override
	{
		return Uint(sizeof(value));
	}
	virtual String get_typename() const override;
	virtual String to_string() const override;
};



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

public: //Object methods
	virtual Float_Basic&& move() override
	{
		return std::move(*this);
	}
	virtual Uint get_size() const override
	{
		return Uint(sizeof(value));
	}
	virtual String get_typename() const override;
	virtual String to_string() const override;
};



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

public: //Object methods
	virtual Bool&& move() override
	{
		return std::move(*this);
	}
	virtual Uint get_size() const override
	{
		return Uint(sizeof(value));
	}
	virtual String get_typename() const override;
	virtual String to_string() const override;
};

class Byte : public Object
{
private:
	union {
		uint8_t value = 0;
		struct bit : public Object
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
	{
		return String(value.substr(0, (unsigned int)index));
	}
	String back_string(Uint index) const
	{
		return String(value.substr((unsigned int)index));
	}
	String substring(Uint begin, Uint end) const { return String(value.substr((unsigned int)begin, (unsigned int)end)); }
	std::pair<String, String> split(Uint index) const
	{
		return std::pair<String, String>(this->front_string(index), this->back_string(index));
	}
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
		return ((std::wstring)lhs) += ((wchar_t)rhs);
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
		iterator(nullptr_t __n) : obj(__n) {} // end/rend 생성
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
		using pointer = Range * ;
	private:
		pointer obj = nullptr;
	public:
		reverse_iterator(pointer p) : obj(p) { p->current_value = p->tail; }
		reverse_iterator(nullptr_t __n) : obj(__n) {}
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
	{
		return iterator(nullptr);
	}
public: //역순 반복자
	Range::reverse_iterator rbegin()
	{
		current_value = tail;
		return reverse_iterator(this);
	}
	Range::reverse_iterator rend()
	{
		return reverse_iterator(nullptr);
	}
public:
	Range() = delete;
	virtual ~Range() = default;
	Range(int _lhs, int _rhs) : head(_lhs), tail(_rhs) {}
};


/*.*/
inline Uint Object::get_size() const
{
	return Uint(0);
}

inline String Object::get_typename() const
{
	return String(L"Object");
}

inline String Object::to_string() const
{
	return String(L"");
}

template<class IntType>
inline String Int_Basic<IntType>::get_typename() const
{
	return String(L"String");
}

template<class IntType>
inline String Int_Basic<IntType>::to_string() const
{
	return String(std::to_wstring(value));
}
