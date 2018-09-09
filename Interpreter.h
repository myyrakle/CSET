#pragma once
#include "stdafx.h"

//CSET 코드를 C++ 코드로 변환합니다.
//+ 하나의 cpp 파일로 합칩니다.(바뀔 수 있음)

class Interpreter
{
public:
	void add_file(wstring&& filename); //변환할 파일 추가
	void add_option(); //인터프리트 옵션 추가
	void interpret(); //변환 수행.


private:
	vector<wstring> filenames;
	//vector<?> options;
	vector<wstring> sources;


public: //구현하지 않음
	Interpreter() = default;
	~Interpreter() = default;
	Interpreter(const Interpreter&) = delete;
	Interpreter(Interpreter&&) = delete;
	Interpreter& operator=(const Interpreter&) = delete;
	Interpreter& operator=(Interpreter&&) = delete;
};