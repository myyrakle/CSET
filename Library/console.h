#include <cstdio>
#include <cwchar>
#include <cstdarg>
#include <stdint.h>
#include <string>
#define _CRT_NO_VA_START_VALIDATION
#define _CRT_SECURE_NO_WARNINGS



namespace console
{
	
	void put_format(const std::wstring& format, ...)
	{
		va_list args;
		va_start(args, format);

		vwprintf(format.c_str(), args);

		va_end(args);
	}
	void putf(const std::wstring& format, ...)
	{
		va_list args;
		va_start(args, format);

		vwprintf(format.c_str(), args);

		va_end(args);
	}

	void putline()
	{
		putchar('\n');
	}

	void put(const std::wstring& val)
	{
		wprintf(L"%s", val.c_str());
	}
	void putline(const std::wstring& val)
	{
		put(val);
		putline();
	}
	void put(int val)
	{
		printf("%d", val);
	}
	void putline(int val)
	{
		put(val);
		putline();
	}
	void put(wchar_t val)
	{
		putwchar(val);
	}
	void putline(wchar_t val)
	{
		put(val);
		putline();
	}
	void put(float val)
	{
		printf("%f", val);
	}
	void putline(float val)
	{
		put(val);
		putline();
	}
	void put(double val)
	{
		printf("%f", val);
	}
	void putline(double val)
	{
		put(val);
		putline();
	}
	void flush()
	{
		fflush(stdout);
	}



	void pour_buffer()
	{
		while (getchar() != EOF);
	}
	std::wstring getline()
	{
		std::wstring temp;
		fgetws((wchar_t*)temp.data(), INT_MAX, stdin);
		return temp;
	}
	std::wstring getword()
	{
		std::wstring temp;
		wscanf(L"%s", temp.data());
		return temp;
	}
	void get(int& value)
	{
		wscanf(L"%d",&value);
	}
	void get(wchar_t value)
	{
		wscanf(L"%c",&value);
	}
	wchar_t getc()
	{
		wchar_t c = getwchar();
		return c;
	}
	
	
	



	void clean()
	{
		system("cls");
	}
	void pause()
	{
		system("pause");
	}
	void backspace()
	{
		putchar('\b');
	}
}
