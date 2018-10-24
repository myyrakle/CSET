#pragma once
#include "basic.h"

//console 모듈입니다,
#include <iostream>

namespace console
{
	//개행만 합니다.
	void putline() { putchar('\n'); }

	//문자열을 출력합니다.
	void put(const String& val) { wprintf(L"%s", ((std::wstring)val).c_str()); }
	void putline(const String& val) { put(val); putline(); }

	//정수를 출력합니다.
	template<class IntType>
	void put(Int_Basic<IntType> val) { printf("%d", (IntType)val); }
	template<class IntType>
	void putline(Int_Basic<IntType> val) { put(val); putline(); }

	//단일 문자를 출력합니다.
	void put(Char val) { putwchar((wchar_t)val); }
	void putline(Char val) { put(val); putline(); }

	//실수를 출력합니다.
	template<class FloatType>
	void put(Float_Basic<FloatType> val) { printf("%f", (float)val); }
	template<class FloatType>
	void putline(Float_Basic<FloatType> val) { put(val); putline(); }

	//논리를 출력합니다.
	void put(Bool val) { val ? printf("true") : printf("false"); }
	void putline(Bool val) { put(val); putline(); }

	//버퍼 정리
	void flush_out() { fflush(stdout); }
	void flush_in() { while (getchar() != EOF); }


	//개행단위로 문자열 입력
	String getline() 
	{ 
		String temp; 
		fgetws((wchar_t*) (((std::wstring)temp).data()), INT_MAX, stdin);
		return temp; 
	}

	//엔터'만' 들어올때까지 계속 입력
//#include <list>
//#include <cwchar>
//	std::list<String> getlines()
//	{
//		
//		std::list<String> lines;
//		String str = L"";
//		while (true)
//		{
//			while (true)
//			{
//				char c = std::getwchar();
//				if (c == '\n')
//					break;
//				else
//					str += c;
//			}
//			if (str == String(L"")) //개행만 들어오면 종료
//				return lines;
//			else
//			{
//				lines.push_back(str), str = L"";
//				continue;
//			}
//		}
//		
//	}

	//화이트스페이스 단위로 문자열 입력
	String getword() 
	{ 
		String temp; 
		wscanf(L"%s", (wchar_t*) (((std::wstring)temp).data())); 
		return temp;
	}

	//단일 문자 입력
	Char getc() 
	{ 
		return Char(wchar_t(getwchar()));
	}

	//정수 입력
	void get(Int& value) { wscanf(L"%d", &(int&)value); }
	void get(Uint& value) { wscanf(L"%u", &(unsigned int&)value); }

	//문자 입력
	void get(Char& value) { wscanf(L"%c", &(wchar_t&)value); }

	//실수 입력
	void get(Float& value) { wscanf(L"%f", &(float&)value); }
	void get(Double& value) { wscanf(L"%lf", &(double&)value); }
	void get(Ldouble& value) { wscanf(L"%lf", &(long double&)value); }

	//화면을 청소합니다.
	void clear() { system("cls"); }

	//콘솔을 대기시킵니다.
	void pause() { system("pause"); }

	//백스페이스를 입력합니다.
	void backspace() { putchar('\b'); }
}