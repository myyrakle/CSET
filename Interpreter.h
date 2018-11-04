#pragma once
#pragma once
#include "pch.h"

//CSET 코드를 C++ 코드로 변환합니다.
//+ 하나의 cpp 파일로 합칩니다.(바뀔 수 있음)

enum class State
{
	In_Global,
	In_Func,
	In_Main_Func,
	In_Local,
};

class Handler;

class Interpreter
{
private:
	Handler* caller;
public:
	void set_caller(Handler* ptr);

public:
	void interpret(std::queue<std::wstring>& tokens); //변환 수행.
	void finish(); //다 끝내고 뽑아내기

private: //인터프리트 내부 작업들
	void convert_typename(std::wstring&);
	void convert_unused_keywords(std::wstring&);
	void do_import(const std::wstring&);
private:
	void print_error(wstring_view);
private:
	void interpret_global(std::queue<std::wstring>&);
	void interpret_local(std::queue<std::wstring>&, int);
	void interpret_function(std::queue<std::wstring>&);
	void interpret_lambda(std::queue<std::wstring>&);
	std::wstring interpret_types(std::queue<std::wstring>&);
	void interpret_class(std::queue<std::wstring>&);
	void interpret_variable(std::queue<std::wstring>&); //const, mut, literal
private:
	void interpret_array();
	void interpret_vector();

private:
	wstring TEMP_name = L"__temp__name";
private:
	int line_num = 0;

private:
	//int unclosed_bracket = 0;

private: //main 함수 처리용
	bool in_main_func = false;
	std::wstring cmdline_arg_name;


public: //구현하지 않음
	Interpreter() = default;
	~Interpreter() = default;
	Interpreter(const Interpreter&) = delete;
	Interpreter(Interpreter&&) = delete;
	Interpreter& operator=(const Interpreter&) = delete;
	Interpreter& operator=(Interpreter&&) = delete;
};