// cset.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include "Handler.h"


void write_default_info()
{
	cout.precision(15);
	cout << "CSET 컴파일러 Ver 1.0 Alpha"
		<< newline << newline;
}

int wmain(int argument_count, wchar_t* arguments[])
{
	setlocale(LC_ALL, "");
	//wcout.imbue(std::locale(""));

	wstring original_filepath = current_filepath();
	//std::wcout << original_filepath << std::endl;

	fs::create_directory(original_filepath + L"\\temp");

	//추가정보가 전달되지 않으면 기본정보를 출력하고 종료합니다.
	if (argument_count == 1)
	{
		write_default_info();
		exit(EXIT_SUCCESS);
	}

	//본격적인 작업을 수행합니다.
	else
	{
		Handler handler(original_filepath);
		
		for (int i = 1; i < argument_count; i++)
		{
			if (arguments[i][0] == '-');
			//컴파일 옵션 추가
			else
				handler.add_file(arguments[i]);
		}
		handler.run();

		//fs::remove(L"temp.cpp");

		exit(EXIT_SUCCESS);
	}
}