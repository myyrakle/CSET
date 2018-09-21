#include "stdafx.h"
#include "Interpreter.h"

void Interpreter::add_file(wstring && filename)
{
	filenames.emplace_back(std::forward<wstring&&>(filename));
}

void Interpreter::interpret()
{
	this->push_default_header();

	//전부 읽어서 저장.
	for (auto& filename : filenames)
	{
		this->current_filename = filename;

		this->fin.open(filename);
		
		this->line_num = 0;
		while (this->read_line())
			this->interpret_global();

		fin.close();
	}
	
	this->finish();
}

void Interpreter::push_default_header()
{
	prototypes += L"#include <iostream>\n";
	prototypes += L"#include <string>\n";
	prototypes += L"using string = std::wstring;\n";
	prototypes += L"#include <vector>\n";
	prototypes += L"#include <array>\n";

	wcout << this->_original_filepath << endl << endl;
	fin.open(this->_original_filepath + L"\\Library\\console.h");
	if (fin.good()) printf("\n열림\n");
	else printf("\n안열림\n");

	while (fin)
		this->prototypes += fin.get();
	
	fin.close();
}

void Interpreter::finish()
{
	wofstream fout(this->_original_filepath+L"\\temp\\temp.cpp");

	for (auto& source : this->prototypes)
		fout << source;
	for (auto& source : this->bodys)
		fout << source;

	fout.close();
}

bool Interpreter::read_line()
{
	if (this->fin)
	{
		std::getline(this->fin, this->line);
		this->line_num++;
		this->line_to_words();
	}
	return (bool)this->fin;
}

//한줄 읽어서 화이트스페이스 기준으로 단어 분리. 코멘트는 삭제. 문자열 리터럴은 wstring으로 처리
void Interpreter::line_to_words() 
{
	wstring word;
	wstring text_literal=L"";

	bool in_text = false; // 문자열 리터럴을 탐색중인가?
	bool quote_is_open = false; 

	bool is_escape = false;

	bool has_one_slash = false;
	bool in_comment = false;

	for (auto& c : this->line)
	{

		//한줄 주석 제거
		if (c == '/' && in_text == false) //문자열 리터럴 안이 아니면서, /임
		{
			if (has_one_slash == false) //슬래시가 하나도 없었으면 하나 체크
			{
				has_one_slash = true;
				continue;
			}
			else //슬래시가 하나 있었으니 여기서부터는 주석. 전부 자르고 break, 리턴
			{
				words.push(word);
				break;
			}
		}
		else //슬래시가 연속되지 않으니, 체크 해제.
			has_one_slash = false;

		 
		if (c == '\"' && is_escape == false) //" 문자로 문자열 리터럴이 열릴 때
		{
			if (in_text == false) //들어갈 때
			{
				text_literal = L"std::wstring(L\"";
				in_text = true;
				continue;
			}
			else //나갈 때
			{
				text_literal += L"\")";
				this->words.push(text_literal);
				in_text = false;
				continue;
			}
		}
		else if (in_text) //문자열 리터럴 탐색중일 경우
		{
			if (is_escape == true) //체크되어있으면 해제
				is_escape = false;
			else if (c == '\\') //이스케이프 문자 체크
				is_escape = true;
				
			text_literal += c;
			continue;
		}

		//괄호 만날 경우 각 괄호를 한 단어로 분리
		else if (c == '(' || c == ')' || c == '{' || c == '}' || c == ':' || c == '=') 
		{
			if (!word.empty())
			{
				this->convert_typename(word);
				words.push(word);
				word.clear();
			}

			words.push(wstring(1,c));
			continue;
		}
		
		else if (c == '\n' || c == '\t' || c == ' ') //화이트스페이스 기준 분리
		{
			if (word != L"")
			{
				this->convert_typename(word);
				this->words.push(word);
				word.clear();
			}
			continue;
		}
		else
		{
			word += c;
			continue;
		}
	}
}

void Interpreter::convert_typename(std::wstring& word) //타입 키워드를 실제 구현 클래스명으로 변경
{
	auto to_upper = [](wchar_t& c) { if (c >= 'a' && c <= 'z') c -= 32; };

	if (word == keywords::OBJECT || 
		word == keywords::CHAR || keywords::STRING ||
		word == keywords::BOOL || keywords::BYTE ||
		word == keywords::INT || 
		word ==keywords::FLOAT || word == keywords::DOUBLE)
		to_upper(word[0]);
	else
		return;
}

//쓰지 않을 예약어 식별자로 쓸수 있게 수정
void Interpreter::convert_unused_keywords(std::wstring & word) 
{
	if (word == keywords::unuse::AUTO || word == keywords::unuse::DELETE_ ||
		word == keywords::unuse::GOTO || word == keywords::unuse::LONG)
		word.insert(0, this->temp_name);
}

void Interpreter::read_line_if_empty()
{
	if (fin)
		if (words.empty())
			this->read_line();
}

void Interpreter::print_error(wstring_view log) const
{
	wprintf(L"파일명 %s\n", this->current_filename.data());
	wprintf(L"라인 : %d\n", this->line_num);
	wprintf(L"%s", log.data());
	exit(EXIT_SUCCESS);
}

void Interpreter::interpret_global()
{
	auto head_word = words.front();

	//func 키워드 만날 경우
	if (this->words.front() == keywords::FUNC)
	{
		this->interpret_function(); //시그너처 파싱
		this->interpret_local(); //몸체 로컬
	}

	//const나 mut, literal 키워드일 경우
	else if (head_word == keywords::CONST_ || head_word == keywords::MUT || head_word == keywords::LITERAL)
		this->interpret_variable();

	//class 키워드일 경우
	else if (head_word == keywords::CLASS)
		;//this->interpret_class();

	else if (this->words.front() == keywords::NAMESPACE)
	{
		//To do
	}

	else if (this->words.front() == keywords::USING)
	{
		this->words.pop();
		this->bodys += L"using namespace ";
		this->bodys += words.front();
		this->bodys += ';';
		throw new exception("using에 뭐가 더 들어갔네요?");
	}

	else //아무것도 아닐 경우
	{
		this->bodys += this->line;
		bodys += ';\n';
		return;
	}
}

void Interpreter::interpret_local()
{
	if (in_main_func) //main 함수일 경우 명령행 인자 컨버트
	{
		bodys += L"\nsetlocale(LC_ALL, \"\");\n";
		if (!cmdline_arg_name.empty())
		{
			bodys += L"std::vector<std::wstring> "; bodys += cmdline_arg_name; bodys += L";\n";
			bodys += L"for(int i=0;i<"; bodys += temp_argc; bodys += L";i++)\n\t";
			bodys += cmdline_arg_name; bodys += L".emplace_back("; bodys += temp_args; bodys += L"[i]);\n";

			in_main_func = false;
			cmdline_arg_name.clear();
		}
	}

	while (this->unclosed_bracket != 0) //중괄호 다 닫힐때까지 루프
	{
		if (words.empty())
		{
			bodys += L";\n";
			this->read_line_if_empty();
		}

		if (words.front() == L"{")
		{
			this->unclosed_bracket++;
			this->bodys += words.front(); words.pop();
			continue;
		}
		else if (words.front() == L"}")
		{
			this->unclosed_bracket--;
			this->bodys += words.front(); words.pop();
			continue;
		}
		//const나 mut, literal 키워드일 경우
		else if (words.front() == keywords::CONST_ || words.front() == keywords::MUT
			|| words.front() == keywords::LITERAL)
		{
			this->interpret_variable();
			bodys += ';';
		}
		//To do
		else
		{
			bodys += words.front(); words.pop();
		}		
	}

	return;
}

void Interpreter::interpret_function()
{
	wstring func_signature = L"";

	if (this->words.front() == keywords::STATIC)
		func_signature += keywords::STATIC, this->words.pop();
	
	if (this->words.front() == keywords::FUNC)
		func_signature += L"auto ", this->words.pop();


	//함수명 추출
	if (words.front() == L"main") //main 함수면 따로 처리
		this->in_main_func = true;
	
	func_signature += words.front();
	words.pop();

	//파라미터 체크
	if (words.front() != L"(")
		this->print_error(L"명령행 인자에는 하나만 넣을 수 있어요");
	else
	{
		func_signature += words.front();
		words.pop();
	}

	while (words.front() != L")")
	{

				/*	main 함수	*/
				if (this->in_main_func) 
				{
					this->cmdline_arg_name = words.front();
					words.pop();

					if (words.front() != L")") //뭐가 더 있음
					{
						wprintf(func_signature.c_str());
						this->read_line_if_empty();
						if (words.front() != L":")
						{
							words.pop();
							words.pop();
						}
						else
							this->print_error(L"명령행 인자에는 하나만 넣을 수 있어요");
					}

					func_signature += L"int ";
					func_signature += temp_argc;
					func_signature += L",char** ";
					func_signature += temp_args;

					break;
				}
				/*	main 함수	*/


		//한정자
		if (words.front() == keywords::MUT)
			words.pop();
		else if (words.front() == keywords::CONST_)
		{
			func_signature += L"const ";
			words.pop();
		}
		else //디폴트 const
			func_signature += L"const ";


		//변수명 저장
		auto var_name = words.front();
		words.pop();

		if (words.front() != L":")
			this->print_error(L"파라미터에 콜론 어디갔어요?");
		words.pop();

		//타입
		func_signature += words.front();
		func_signature += ' ';
		func_signature += var_name;
		words.pop();

		//콜론
		if (words.front() == L",")
		{
			func_signature += words.front();
			words.pop();
		}
	}
	func_signature += words.front();
	words.pop();
	
	//리턴타입
	if (words.empty() || words.front() == L"{") //없으면 void
		if(in_main_func)
			func_signature += L"->int";
		else
			func_signature += L"->void";
	else if (words.front().find(L"->") != wstring::npos)
	{
		if (words.front() == L"->") //->와 타입이 떨어져있을 경우
		{
			func_signature += words.front();
			words.pop();
		}
		func_signature += words.front();
		words.pop();
	}
	else
		this->print_error(L"리턴타입 부분이 이상한거같은데");
	
	this->prototypes += func_signature;
	this->prototypes += L";\n";
	this->bodys += func_signature;
	this->bodys += '\n';

	//
	if (words.empty())
		this->read_line();
	if (words.front() == L"{")
	{
		this->bodys += words.front(); words.pop();
		this->unclosed_bracket++;
		this->interpret_local();
	}
	return;
}


void Interpreter::interpret_variable()
{
	wstring variable_expr = L"";
	bool has_init = false;

	auto head_word = this->words.front(); this->words.pop();

	if (head_word == keywords::LITERAL)
	{
		variable_expr += L"constexpr auto ";
		
		while ( !words.empty() )
		{
			variable_expr += this->words.front();
			variable_expr += ' ';
			this->words.pop();
		}
	}

	else if (head_word == keywords::CONST_ || head_word == keywords::MUT)
	{
		//const 여부 체크
		if (head_word == keywords::CONST_)
			variable_expr += keywords::CONST_, variable_expr += ' ';
		this->words.pop();

		//변수 이름 저장
		wstring var_name = words.front();
		words.pop();

		//타입 체크
		if (words.front() == L":") //타입이 명시될 경우
		{
			words.pop();
			variable_expr += words.front();
			words.pop();
		}
		else //없으면 추론
		{
			variable_expr += L"auto";
		}

		variable_expr += ' ';
		variable_expr += var_name;

		//나머지 갖다붙이기
		while (!words.empty()) 
		{
			variable_expr += words.front();
			variable_expr += ' ';
			words.pop();
		}
	}

	this->bodys += variable_expr;
}

void Interpreter::set_original_filepath(const wstring &path)
{
	this->_original_filepath = path;
}

