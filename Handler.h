#pragma once

#include "Tokenizer.h"
#include "Interpreter.h"
#include "Builder.h"

//모든 행동을 제어합니다.

class Handler
{
public: 
	void run();
public:
	void add_file(std::wstring&&);
	void set_original_filepath(const std::wstring&);
private:
	std::wstring getline();
private:
	Tokenizer tokenizer;
	Interpreter interpreter;
	Builder builder;
private:
	std::wstring prototypes; //위에 모아놓을 전방선언들
	std::wstring bodys; //몸체들
private:
	std::queue<std::wstring> tokens; //tokenize 결과물
	int line_number = 0;
private:
	std::vector<std::wstring> filenames; //파일 이름들
	std::wifstream current_file_inputstream; //현재 열려있는 파일 입력 스트림
	std::wstring _original_filepath; //컴파일러 실제 디렉터리 위치 기준.
private: //에러 로그용
	std::wstring_view current_filename; //현재 열려있는 파일명
	void print_error(std::wstring_view str) const;

public: 
	Handler() = default;
	virtual ~Handler() = default;
public:
	Handler(const Handler&) = delete;
	Handler(Handler&&) = delete;
	Handler& operator=(const Handler&) = delete;
	Handler& operator=(Handler&) = delete;
};

