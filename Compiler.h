#pragma once
#include "stdafx.h"
//변환된 C++코드를 받아서 g++로 컴파일을 수행합니다.

enum class CompileOption
{
	//...
};

class Compiler
{

public:
	//void add_header(wstring&& filename);
	void add_file(wstring&& filename);
	void add_option(CompileOption&& option);
	const vector<CompileOption> get_options() const;
	void compile() const;

public:
	void set_original_filepath(const wstring&);
private:
	wstring _original_filepath;

private:
	vector<wstring> filenames;
	//array<wstring> headers;
	//vector<CompileOption> options

public: //구현하지 않음
	Compiler() = default;
	~Compiler() = default;
	Compiler(const Compiler&) = delete;
	Compiler(Compiler&&) = delete;
	Compiler& operator=(const Compiler&) = delete;
	Compiler& operator=(Compiler&&) = delete;
};