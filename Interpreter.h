#pragma once
#pragma once
#include "pch.h"

//CSET 코드를 C++ 코드로 변환합니다.
//+ 하나의 cpp 파일로 합칩니다.(바뀔 수 있음)

enum class InterpreterState
{
	In_Global,
	In_Func,
	In_Main_Func,
	In_Local,
	In_LAMBDA,
	DEFAULT,
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
public:
	void do_import(const std::wstring&);
	bool is_keyword(const std::wstring&) const;
private:
	void print_error(wstring_view);
private:
	void interpret_global(std::queue<std::wstring>&);
	void interpret_local(std::queue<std::wstring>&, int);
	void interpret_function(std::queue<std::wstring>&);
	std::wstring interpret_types(std::queue<std::wstring>&);
	void interpret_class(std::queue<std::wstring>&);
	void interpret_variable(std::queue<std::wstring>&); //const, mut, literal
public:
	void interpret_check_Range(std::queue<std::wstring>&);
	void interpret_lambda(std::queue<std::wstring>&);
	void interpret_for(std::queue<std::wstring>&);
	void interpret_rfor(std::queue<std::wstring>&);
	void interpret_while(std::queue<std::wstring>&);
	void interpret_if(std::queue<std::wstring>&);
	void interpret_else(std::queue<std::wstring>&);

private:
	void interpret_vector();

private:
	wstring TEMP_name = L"__temp__name";
private:
	int line_num = 0;

private:
	//중괄호를 추가로 썼나?
	int more_bracket_in_rfor = 0;
	bool straight = false;

private: //main 함수 처리용
	bool in_main_func = false;
	std::wstring cmdline_arg_name;

private: //only for Range
	bool in_range = false;
	std::wstring pre_word;
	std::wstring post_word;
	int unclosed_small_bracket = 0;

private:
	InterpreterState state = InterpreterState::DEFAULT;

public: //구현하지 않음
	Interpreter() = default;
	~Interpreter() = default;
	Interpreter(const Interpreter&) = delete;
	Interpreter(Interpreter&&) = delete;
	Interpreter& operator=(const Interpreter&) = delete;
	Interpreter& operator=(Interpreter&&) = delete;
};