#include "pch.h"
#include "Builder.h"

void Builder::add_file(wstring && filename)
{
	filenames.emplace_back(std::forward<wstring&&>(filename));
}

void Builder::compile() const
{
	//환경변수 INCLUDE 설정 (#include 헤더 포함 경로)
	_wsystem(
		(L"setx INCLUDE " + _original_filepath + L"\\msvc\\include;"
			+ _original_filepath + L"\\Library\\SFML\\include").c_str()
	);

	//환경변수 LIB 설정 (lib 의존성 명시)
	_wsystem(
		(L"setx LIB " + _original_filepath + L"\\msvc\\lib;"
			+ _original_filepath + L"\\Library\\SFML\\lib").c_str()
	);


	wstring cmd = this->_original_filepath + L"\\msvc\\bin\\cl.exe /EHsc";

	for (auto& filename : filenames)
		(cmd += ' ') += filename;

	_wsystem(cmd.c_str());
}

void Builder::set_original_filepath(wstring&& path)
{
	this->_original_filepath = path;
}