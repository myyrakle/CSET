#pragma once
#include "stdafx.h"

//CSET 코드를 C++ 코드로 변환합니다.
//+ 하나의 cpp 파일로 합칩니다.(바뀔 수 있음)

enum class InterpretOption
{
	//...
};

bool is_whitespace(wchar_t c)
{
	if (c == ' ' || c == '\n' || c == '\t')
		return true;
	else
		return false;
}

class Interpreter
{
public:
	void add_file(wstring&& filename); //변환할 파일 추가
	void interpret(); //변환 수행.
	void finish(); //다 끝내고 뽑아내기

private: //인터프리트 내부 작업들
	bool read_line(); //현재 파일에서 한줄 읽어 (bool)fin 리턴
	void line_to_words(); 
	bool read_line_if_words_empty();
	//void chars_to_wstring(); //deprecated
	//void remove_comment(); //deprecated

	void interpret_global();
	void interpret_local();
	void interpret_function();
	void interpret_function_main(const wstring& func_signature);
	void interpret_class();
	void interpret_parameter();
	void interpret_variable(); //const, mut, literal
	void interpret_variable_static();
	void interpret_class();
	
	void interpret_array();

private:
	string temp_name = "__temp__name";
	array<string, 30> unuse_cpp_keywords
	{""};

	vector<wstring> filenames; //파일
	wifstream fin;

	wstring line;
	int line_num=0;
	queue<wstring> words;

	int unclosed_bracket=0;

private: //result
	wstring prototypes; //위에 모아놓을 전방선언들
	wstring bodys; //


public: //아직 사용하지 않음
	const vector<InterpretOption>& get_options() const;
	void add_option(InterpretOption&& option); //인터프리트 옵션 추가
private:
	vector<InterpretOption> options;


public: //구현하지 않음
	Interpreter() = default;
	~Interpreter() = default;
	Interpreter(const Interpreter&) = delete;
	Interpreter(Interpreter&&) = delete;
	Interpreter& operator=(const Interpreter&) = delete;
	Interpreter& operator=(Interpreter&&) = delete;
};