#include "pch.h"
#include "Builder.h"

void Builder::build()
{
	this->compile();
}

void Builder::add_file(const std::wstring & filename)
{
	filenames.emplace_back(filename);
}

void Builder::compile() const
{
	//환경변수 INCLUDE 설정 (#include 헤더 포함 경로)
	//기본 라이브러리와 sfml 라이브러리 추가.
	_wsystem(
		(L"setx INCLUDE " + _original_filepath + L"\\msvc\\include;"
			+ _original_filepath + L"\\Library\\SFML\\include").c_str()
	);

	//환경변수 LIB 설정 (lib 의존성 명시)
	//기본 라이브러리와 sfml 라이브러리 추가.
	_wsystem(
		(L"setx LIB " + _original_filepath + L"\\msvc\\lib;"
			+ _original_filepath + L"\\Library\\SFML\\lib").c_str()
	);

	wstring cmd = this->_original_filepath + L"\\msvc\\bin\\cl.exe /EHsc";

	for (auto& filename : filenames)
		std::wcout << L"파일네임 : " << filename << std::endl;

	for (auto& filename : filenames)
		(cmd += ' ') += filename;

	_wsystem(cmd.c_str());
}

void Builder::set_original_filepath(const std::wstring& path)
{
	this->_original_filepath = path;
}