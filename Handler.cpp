#include "pch.h"
#include "Handler.h"

void Handler::run()
{
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
				//한줄 읽어옵니다.
				auto line = this->getline();

				//토큰화합니다.
				this->tokens = std::move(this->tokenizer.tokenize(line));

				//변환작업을 수행합니다.
				this->interpreter.interpret(tokens);

				this->builder.add_file(std::move(this->_original_filepath));
			}
		}
		current_file_inputstream.close();

		this->builder.build();
	}
	//compiler.add_file(original_filepath + L"\\temp\\temp.cpp");
	//compiler.compile();
}

void Handler::add_file(std::wstring&& filename)
{
	filenames.emplace_back(std::move(filename));
}

void Handler::set_original_filepath(const wstring& path)
{
	this->_original_filepath = path;
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
