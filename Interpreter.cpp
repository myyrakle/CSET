#include "stdafx.h"
#include "Interpreter.h"

void Interpreter::add_file(wstring && filename)
{
	filenames.emplace_back(std::forward<wstring&&>(filename));
}

void Interpreter::interpret()
{
	//전부 읽어서 저장.
	for (auto& filename : filenames)
	{
		wifstream fin(filename);
		
		while (fin)
			sources.back() += fin.get();

		fin.close();
	}

	/*
	----------------------변환작업---------------------
	*/
	//cpp 소스파일로 변환
	wofstream fout("temp.cpp");
	for (auto& source : sources)
		fout << source;
	fout.close();
}
