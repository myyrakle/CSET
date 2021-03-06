#include "pch.h"
#include "Handler.h"

void Handler::run()
{
	this->interpreter.set_caller(this);

	interpreter.do_import(L"basic");
	builder.add_file(L"kernel32.lib");
	builder.add_file(L"user32.lib");
	builder.add_file(L"gdi32.lib");
	builder.add_file(L"winspool.lib");
	builder.add_file(L"comdlg32.lib");
	builder.add_file(L"advapi32.lib");
	builder.add_file(L"shell32.lib");
	builder.add_file(L"ole32.lib");
	builder.add_file(L"oleaut32.lib");
	builder.add_file(L"uuid.lib");
	builder.add_file(L"odbc32.lib");
	builder.add_file(L"odbccp32.lib");
		          

	//전부 읽어서 저장.
	for (auto& filename : filenames)
	{
		this->current_filename = filename;

		this->current_file_inputstream.open(filename);
		this->current_file_inputstream.imbue(std::locale(""));

		if (!current_file_inputstream)
			this->print_error(L"소스파일이 안열리네요. 왜이러지");
		else
		{
			this->line_number = 0;

			while (this->current_file_inputstream)
			{
				//토큰화합니다.
				this->tokens = this->read_line_then_tokenize();

				//변환작업을 수행합니다.
				this->interpreter.interpret(tokens);
			}
		}
		this->current_file_inputstream.close();
	}
	this->builder.set_original_filepath(_original_filepath);
	this->builder.add_file(_original_filepath + L"\\temp\\temp.cpp");
	this->builder.build();
}

bool Handler::readable() const
{
	return bool(this->current_file_inputstream);
}

std::queue<std::wstring>&& Handler::read_line_then_tokenize()
{
	line_number++;

	//한줄 읽어옵니다.
	auto line = this->getline();
	//토큰화합니다.
	this->tokens = std::move(this->tokenizer.tokenize(line));

	return std::move(tokens);
}

void Handler::add_file(const std::wstring& filename)
{
	filenames.emplace_back(filename);
}

std::wstring Handler::getline()
{
	std::wstring temp;
	if (this->current_file_inputstream)
	{
		std::getline(this->current_file_inputstream, temp);
		this->line_number++;
	}
	return temp;
}

void Handler::print_error(std::wstring_view str) const
{
	wprintf(L"파일명 : %s\n",current_filename);
	wprintf(L"%s\n", str);
	exit(EXIT_SUCCESS);
}

Handler::Handler(const std::wstring original_) : _original_filepath(original_)
{}