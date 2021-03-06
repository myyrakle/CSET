// pch.cpp: 미리 컴파일된 헤더에 해당하는 소스 파일입니다. 성공하려면 컴파일이 필요합니다.

#include "pch.h"

// 일반적으로 이 파일을 무시하지만 미리 컴파일된 헤더를 사용하는 경우 유지합니다.

std::wstring current_filepath()
{
	constexpr auto buf_size = 200;

	wchar_t buffer[buf_size];

	GetModuleFileNameW(nullptr, buffer, buf_size);

	std::wstring str(buffer);

	return str.substr(0, str.rfind(L"\\"));
}