#include "stdafx.h"
#include "Compiler.h"

void Compiler::add_file(wstring && filename)
{
	filenames.emplace_back(std::forward<wstring&&>(filename));
}

void Compiler::compile() const
{
	
	//커맨드에 쏠 명령어 구성
	wstring cmd = this->_original_filepath + L"\\MinGW\\bin\\g++.exe";

	for (auto& filename : filenames)
		(cmd += ' ') += filename;
	
	_wsystem(cmd.c_str());
}

void Compiler::set_original_filepath(const wstring & path)
{
	this->_original_filepath = path;
}
