#pragma once
#include "stdafx.h"
//변환된 C++코드를 받아서 g++로 컴파일을 수행합니다.

class Compiler
{

public:
	void add_file(wstring&& filename);
	void compile() const;

private:
	//CompileFlag flag = COMPILE;
	vector<wstring> filenames;

public: //구현하지 않음
	Compiler() = default;
	~Compiler() = default;
	Compiler(const Compiler&) = delete;
	Compiler(Compiler&&) = delete;
	Compiler& operator=(const Compiler&) = delete;
	Compiler& operator=(Compiler&&) = delete;
};