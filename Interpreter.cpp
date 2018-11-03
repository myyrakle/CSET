#include "pch.h"
#include "Interpreter.h"
#include "Handler.h"

enum class Interpret_Flag
{
	GLOBAL,
	FUNC_SIG,
	LOCAL,
	CLASS,
	METHOD_SIG,
};

void Interpreter::set_caller(Handler * ptr)
{
	caller = ptr;
}

void Interpreter::interpret(std::queue<std::wstring>& tokens)
{
	this->do_import(L"basic");
	//...
	while(!tokens.empty())
		this->interpret_global(tokens);

	this->finish();
}

void Interpreter::do_import(const std::wstring& module_name)
{
	//Library 경로 모듈 읽어넣음
	wifstream read_module(((caller->_original_filepath + L"\\Library\\") + module_name) + L".h");
	
	if (!read_module) this->print_error(L"모듈이 안열리네요.");
	
	while (read_module)
		caller->prototypes += read_module.get();
	caller->prototypes += L"\n\n";

	read_module.close();
}

void Interpreter::finish()
{
	/*wofstream fout(this->_original_filepath + L"\\temp\\temp.cpp");

	
	for (auto& source : this->prototypes)
		fout << source;
	for (auto& source : this->bodys)
		fout << source;

	fout.close();*/
}


//타입 키워드를 실제 구현 클래스명으로 변경
void Interpreter::convert_typename(std::wstring& word) 
{
	auto to_upper = [](wchar_t& c) { if (c >= 'a' && c <= 'z') c -= 32; };

	if (word == keywords::OBJECT ||
		word == keywords::CHAR || keywords::STRING ||
		word == keywords::BOOL || keywords::BYTE ||
		word == keywords::INT ||
		word == keywords::FLOAT || word == keywords::DOUBLE)
		to_upper(word[0]);
	else
		return;
}

//쓰지 않을 예약어 식별자로 쓸수 있게 수정
void Interpreter::convert_unused_keywords(std::wstring & word)
{
	if (word == keywords::unuse::AUTO || word == keywords::unuse::DELETE_ ||
		word == keywords::unuse::GOTO || word == keywords::unuse::LONG)
		word.insert(0, this->TEMP_name);
}


void Interpreter::print_error(wstring_view log)
{
	/*wprintf(L"파일명 %s\n", this->current_filename.data());
	wprintf(L"라인 : %d\n", this->line_num);
	wprintf(L"%s", log.data());
	
	this->finish();

	exit(EXIT_SUCCESS);*/
}

void Interpreter::interpret_global(std::queue<std::wstring>& tokens)
{
	//func 키워드 만날 경우
	if (tokens.front() == keywords::FUNC)
	{
		this->interpret_function(); //시그너처 파싱
		this->interpret_local(); //몸체 로컬 진입
	}

	//static 키워드 만날 경우
	else if (tokens.front() == keywords::STATIC)
	{
		this->print_error(L"전역에서는 static 못써요");
	}

	//const나 mut, literal 키워드일 경우
	else if (tokens.front() == keywords::CONST_ || tokens.front() == keywords::MUT
		|| tokens.front() == keywords::LITERAL)
	{
		this->interpret_variable();
	}

	//import 키워드일 경우
	else if (tokens.front() == keywords::IMPORT)
	{
		tokens.pop(); //import 삭제

		if (tokens.empty())
			this->print_error(L"import할 모듈이 어딨죠?");
		else
		{
			this->do_import(tokens.front()); //console이면 console.h 읽어넣음
			tokens.pop();
		}
	}

	//class 키워드일 경우
	else if (tokens.front() == keywords::CLASS)
	{
		//this->interpret_class();
	}

	else if (tokens.front() == keywords::NAMESPACE)
	{
		//To do
	}

	else if (tokens.front() == keywords::USING)
	{
		tokens.pop();
		caller->bodys += L"using namespace ";
		caller->bodys += tokens.front();
		caller->bodys += ';';
		//this->print_error(L"using에 뭐가 더 들어갔네요?");
	}

	else //아무것도 아닐 경우
	{
		//tokens += token;
		caller->prototypes += L";\n";
		return;
	}
}

void Interpreter::interpret_local(std::queue<std::wstring>& tokens)
{
	if (this->in_main_func) //명령행 인자 있는 main 함수일 경우
	{
		in_main_func = false;

		caller->bodys += L"\nsetlocale(LC_ALL, \"\");\n";
		if (!cmdline_arg_name.empty())
		{
			//이 형태로 변환됨
			/*
			DynamicArray<std::wstring> 명령행_인자명(인자_개수);
			for(int i = 0; i < 인자_개수; i++)
				명령행_인자명[i] = 인자들[i];
			*/
			caller->bodys += L"auto ___strlen = [](const char* str){ size_t i=0; for(;s[i]!='\0';i++); return i; };";
			caller->bodys += L"DynamicArray<std::wstring> "; caller->bodys += cmdline_arg_name;
			caller->bodys += '('; caller->bodys += TEMP_argc; caller->bodys += L");\n";

			caller->bodys += L"for(int i=0;i<"; caller->bodys += TEMP_argc; caller->bodys += L";i++)\n\t";
			caller->bodys += cmdline_arg_name;
			caller->bodys += L"[i]="; caller->bodys += TEMP_args; caller->bodys += L'[i];';
			
			cmdline_arg_name.clear();
		}
	}

	while (this->unclosed_bracket != 0) //중괄호 다 닫힐때까지 루프
	{
		if (tokens.empty())
		{
			caller += L";\n";
			this->read_line_if_empty();
		}

		if (tokens.front() == L"{")
		{
			this->unclosed_bracket++;
			caller->bodys += tokens.front(); tokens.pop();
			continue;
		}
		else if (tokens.front() == L"}")
		{
			this->unclosed_bracket--;
			caller->bodys += tokens.front(); tokens.pop();
			continue;
		}
		//const나 mut, literal 키워드일 경우
		else if (tokens.front() == keywords::CONST_ || tokens.front() == keywords::MUT
			|| tokens.front() == keywords::LITERAL)
		{
			this->interpret_variable();
			caller->bodys += ';';
		}

		else if (tokens.front() == keywords::CLASS)
		{
			this->print_error(L"로컬 클래스 미지원");
		}

		//To do
		else
		{
			caller->bodys += tokens.front(); tokens.pop();
		}
	}

	return;
}

void Interpreter::interpret_function(std::queue<std::wstring>& tokens)
{
	wstring func_signature = L"";

	if (tokens.front() == keywords::FUNC)
		func_signature += L"auto ", tokens.pop();

	//main 함수 여부 체크
	if (tokens.front() == L"main")
		this->in_main_func = true;

	//함수 이름
	func_signature += in_main_func ? L"wmain" : tokens.front();
	tokens.pop();

	//파라미터 체크
	if (tokens.front() != L"(")
		this->print_error(L"여는괄호 어딨어요");
	else
	{
		func_signature += tokens.front();
		tokens.pop();
	}

	/*	main 함수	*/
	if (this->in_main_func)
	{
		if (tokens.front() == L")") //파라미터가 없을 경우
		{

		}
		else //파라미터가 있을 경우
		{
			this->cmdline_arg_name = tokens.front(); tokens.pop(); //명령행 인자명 획득

			// : 체크
			if (tokens.front() == L":")
			{
				tokens.pop();
			}
			else
				this->print_error(L"타입 어디갔어요");
			
			if (tokens.front() == L"string[]")
			{
				tokens.pop();
			}
			else
				this->print_error(L"명령행 인자는 string 배열만 올수 있어요");

			if (tokens.front() != L")")
				this->print_error(L"닫는괄호 어디갔어요?");

			func_signature += L"int ";
			func_signature += TEMP_argc;
			func_signature += L",wchar_t** ";
			func_signature += TEMP_args;
			func_signature += L')';

			if(tokens.empty())
			{
				func_signature += L"-> int";
			}
			else
			{
				if (tokens.front() == L"->")
					func_signature += L"->", tokens.pop();
				else
					this->print_error(L"반환타입 어딨어요");

				if (tokens.front() == L"int")
					func_signature += L"int", tokens.pop();
				else
					this->print_error(L"main 함수 반환형은 무조건 int에요");
			}
		}
	}

	/*main 함수가 아닌 경우들*/
	else
	{
		while (tokens.front() != L")")
		{
			//한정자
			if (tokens.front() == keywords::MUT)
				tokens.pop();
			else if (tokens.front() == keywords::CONST_)
			{
				func_signature += L"const ";
				tokens.pop();
			}
			else //디폴트 const
				func_signature += L"const ";

			//변수명 저장
			auto var_name = tokens.front();
			tokens.pop();

			if (tokens.front() != L":")
				this->print_error(L"파라미터에 콜론 어디갔어요?");
			tokens.pop();

			//타입
			func_signature += tokens.front();
			func_signature += ' ';
			func_signature += var_name;
			tokens.pop();

			//콜론
			if (tokens.front() == L",")
			{
				func_signature += tokens.front();
				tokens.pop();
			}
		}

		func_signature += tokens.front();  tokens.pop(); // ) 붙임

	//리턴타입
		if (tokens.empty() || tokens.front() == L"{") //없으면 void
			func_signature += L"->void";

		else if (tokens.front() == L"->")
		{
			func_signature += tokens.front();
			tokens.pop();
		}
		else
			this->print_error(L"리턴타입 부분이 이상한거같은데");

		caller->prototypes += func_signature;
		caller->prototypes += L";\n";
		caller->bodys += func_signature;
		caller->bodys += '\n';
	}
	
	//
	if (tokens.empty())
		if (caller->readable())
			tokens = std::move(caller->read_line_then_tokenize());

	if (tokens.front() == L"{")
	{
		caller->bodys += tokens.front(); tokens.pop();
		this->unclosed_bracket++;
		this->interpret_local(tokens);
	}

	return;
}


void Interpreter::interpret_variable(std::queue<std::wstring>& tokens)
{
	//wstring variable_expr = L"";
	//bool has_init = false;

	//auto head_word = this->words.front(); this->words.pop();

	//if (head_word == keywords::LITERAL)
	//{
	//	variable_expr += L"constexpr auto ";

	//	while (!words.empty())
	//	{
	//		variable_expr += this->words.front();
	//		variable_expr += ' ';
	//		this->words.pop();
	//	}
	//}

	//else if (head_word == keywords::CONST_ || head_word == keywords::MUT)
	//{
	//	//const 여부 체크
	//	if (head_word == keywords::CONST_)
	//		variable_expr += keywords::CONST_, variable_expr += ' ';
	//	this->words.pop();

	//	//변수 이름 저장
	//	wstring var_name = words.front();
	//	words.pop();

	//	//타입 체크
	//	if (words.front() == L":") //타입이 명시될 경우
	//	{
	//		words.pop();
	//		variable_expr += words.front();
	//		words.pop();
	//	}
	//	else //없으면 추론
	//	{
	//		variable_expr += L"auto";
	//	}

	//	variable_expr += ' ';
	//	variable_expr += var_name;

	//	//나머지 갖다붙이기
	//	while (!words.empty())
	//	{
	//		variable_expr += words.front();
	//		variable_expr += ' ';
	//		words.pop();
	//	}
	//}

	//this->bodys += variable_expr;
}

