#pragma once
#include "stdafx.h"

//CSET 코드를 C++ 코드로 변환합니다.
//+ 하나의 cpp 파일로 합칩니다.(바뀔 수 있음)

enum class InterpretOption
{
	//...
};

class Interpreter
{
public:
	void add_file(wstring&& filename); //변환할 파일 추가
	void interpret(); //변환 수행.
	void push_default_header();
	void finish(); //다 끝내고 뽑아내기

private: //인터프리트 내부 작업들
	bool read_line(); //현재 파일에서 한줄 읽어 (bool)fin 리턴
	void line_to_words(); 
	void read_line_if_empty();
	void print_error(wstring_view) const;
private:
	void interpret_global();
	void interpret_local();
	void interpret_function();
	void interpret_class();
	void interpret_variable(); //const, mut, literal
private:
	void interpret_array();
	void interpret_vector();

public:
	void set_original_filepath(const wstring&);
private:
	wstring _original_filepath;

private:
	wstring temp_name = L"__temp__name";
	
private:
	vector<wstring> filenames; //파일
	wstring_view current_filename;
	wifstream fin;

private:
	wstring line;
	int line_num=0;
	queue<wstring> words;

private:
	int unclosed_bracket=0;

private: //main 함수 처리용
	bool in_main_func = false;
	wstring cmdline_arg_name;

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