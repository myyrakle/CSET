#include "stdafx.h"
#include "Interpreter.h"
#include "Compiler.h"

// 9/12 변경

void write_default_info()
{
	cout.precision(15);
	cout << "CSET 컴파일러 Ver 1.0" 
		<< newline << newline;
}

int wmain(int argument_count, wchar_t* arguments[])
{
	setlocale(LC_ALL, "");

	wstring original_filepath = current_filepath();

	fs::create_directory(original_filepath+L"\\temp");

	//추가정보가 전달되지 않으면 기본정보를 출력하고 종료합니다.
	if (argument_count == 1) 
	{
		write_default_info();
		exit(EXIT_SUCCESS);
	}

	//본격적인 작업을 수행합니다.
	else
	{
		Interpreter interpreter;	
		interpreter.set_original_filepath(original_filepath);
		Compiler compiler;
		compiler.set_original_filepath(original_filepath);

		for (int i = 1; i < argument_count; i++)
		{
			if (arguments[i][0] == '-');
			//컴파일 옵션 추가
			else
				interpreter.add_file(arguments[i]);
		}
		interpreter.interpret();

		compiler.add_file(original_filepath+L"\\temp\\temp.cpp");
		compiler.compile();
		//fs::remove(L"temp.cpp");
			
		exit(EXIT_SUCCESS);
	}
}