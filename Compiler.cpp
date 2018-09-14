#include "stdafx.h"
#include "Compiler.h"

void Compiler::add_file(wstring && filename)
{
	filenames.emplace_back(std::forward<wstring&&>(filename));
}

void Compiler::compile() const
{
	//g++ 경로 탐색
	wifstream fin(path_filename);
	wstring gpp_path;
	fin>>gpp_path;
	fin.close();

	//커맨드에 쏠 명령어 구성
	wstring cmd = gpp_path + L"\\g++.exe";
	for (auto& filename : filenames)
		(cmd += ' ') += filename;
	
	_wsystem(cmd.c_str());
}
