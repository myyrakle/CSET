#include "pch.h"
#include "Interpreter.h"
#include "Handler.h"

void Interpreter::set_caller(Handler * ptr)
{
	caller = ptr;
}

void Interpreter::interpret(const std::queue<std::wstring>& tokens)
{
	this->push_default_header();

	//...
	

	this->finish();
}

void Interpreter::push_default_header()
{
	this->do_import(L"basic");
	//this->do_import(L"console");
	//
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

void Interpreter::read_line()
{
	//if (this->current_file_input)
	//{
	//	std::getline(this->current_file_input, this->line);
	//	/*테스트용*/ wcout << this->line << endl;
	//	this->line_num++;
	//}
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

void Interpreter::do_import(const std::wstring& module_name)
{
	////Library 경로 모듈 읽어넣음
	//wifstream read_module(((this->_original_filepath + L"\\Library\\") + module_name) + L".h");
	//
	//if (!read_module) this->print_error(L"모듈이 안열리네요.");
	//
	//while (read_module)
	//	this->prototypes += read_module.get();
	//this->prototypes += L"\n\n";

	//read_module.close();
}

void Interpreter::read_line_if_empty()
{
	/*if (current_file_input)
		if (words.empty())
			this->read_line();*/
}

void Interpreter::print_error(wstring_view log)
{
	/*wprintf(L"파일명 %s\n", this->current_filename.data());
	wprintf(L"라인 : %d\n", this->line_num);
	wprintf(L"%s", log.data());
	
	this->finish();

	exit(EXIT_SUCCESS);*/
}

void Interpreter::interpret_global()
{
	////func 키워드 만날 경우
	//if (words.front() == keywords::FUNC)
	//{
	//	this->interpret_function(); //시그너처 파싱
	//	this->interpret_local(); //몸체 로컬 진입
	//}

	////static 키워드 만날 경우
	//else if (words.front() == keywords::STATIC)
	//{
	//	//ToDo:static 애들 처리
	//}

	////const나 mut, literal 키워드일 경우
	//else if (words.front() == keywords::CONST_ || words.front() == keywords::MUT
	//	|| words.front() == keywords::LITERAL)
	//{
	//	this->interpret_variable();
	//}

	////import 키워드일 경우
	//else if (words.front() == keywords::IMPORT)
	//{
	//	cout << "이거 실행 안되나" << endl << endl;
	//	this->words.pop(); //import 삭제

	//	if (words.empty())
	//		this->print_error(L"import할 모듈이 어딨죠?");
	//	else
	//	{
	//		this->do_import(words.front()); //console이면 console.h 읽어넣음
	//		words.pop();
	//	}
	//}

	////class 키워드일 경우
	//else if (words.front() == keywords::CLASS)
	//{
	//	//this->interpret_class();
	//}

	//else if (words.front() == keywords::NAMESPACE)
	//{
	//	//To do
	//}

	//else if (words.front() == keywords::USING)
	//{
	//	this->words.pop();
	//	this->bodys += L"using namespace ";
	//	this->bodys += words.front();
	//	this->bodys += ';';
	//	throw new exception("using에 뭐가 더 들어갔네요?");
	//}

	//else //아무것도 아닐 경우
	//{
	//	this->bodys += this->line;
	//	bodys += ';\n';
	//	return;
	//}
}

void Interpreter::interpret_local()
{
	//if (in_main_func) //main 함수일 경우 명령행 인자 컨버트
	//{
	//	bodys += L"\nsetlocale(LC_ALL, \"\");\n";
	//	if (!cmdline_arg_name.empty())
	//	{
	//		//이 형태로 변환됨
	//		/*
	//		std::vector<std::wstring> 명령행_인자명;
	//		for(int i = 0; i < 인자_개수; i++)
	//			명령행_인자명.emplace_back(인자들[i]);
	//		*/
	//		bodys += L"auto ___strlen = [](const char* str){ size_t i=0; for(;s[i]!='\0';i++); return i; };";
	//		bodys += L"std::vector<std::wstring> "; bodys += /*명령행 인자*/cmdline_arg_name;
	//		bodys += L";\n";

	//		bodys += L"for(int i=0;i<"; bodys += TEMP_argc; bodys += L";i++)\n\t";
	//		bodys += cmdline_arg_name;
	//		bodys += L".emplace_back("; bodys += TEMP_args; bodys += L'[i]);'; 
	//		in_main_func = false;
	//		cmdline_arg_name.clear();
	//	}
	//}

	//while (this->unclosed_bracket != 0) //중괄호 다 닫힐때까지 루프
	//{
	//	if (words.empty())
	//	{
	//		bodys += L";\n";
	//		this->read_line_if_empty();
	//	}

	//	if (words.front() == L"{")
	//	{
	//		this->unclosed_bracket++;
	//		this->bodys += words.front(); words.pop();
	//		continue;
	//	}
	//	else if (words.front() == L"}")
	//	{
	//		this->unclosed_bracket--;
	//		this->bodys += words.front(); words.pop();
	//		continue;
	//	}
	//	//const나 mut, literal 키워드일 경우
	//	else if (words.front() == keywords::CONST_ || words.front() == keywords::MUT
	//		|| words.front() == keywords::LITERAL)
	//	{
	//		this->interpret_variable();
	//		bodys += ';';
	//	}

	//	else if (words.front() == keywords::CLASS)
	//	{
	//		this->print_error(L"로컬 클래스 미지원");
	//	}

	//	//To do
	//	else
	//	{
	//		bodys += words.front(); words.pop();
	//	}
	//}

	//return;
}

void Interpreter::interpret_function()
{
	//wstring func_signature = L"";

	//if (this->words.front() == keywords::STATIC)
	//	func_signature += keywords::STATIC, this->words.pop();

	//if (this->words.front() == keywords::FUNC)
	//	func_signature += L"auto ", this->words.pop();


	////함수명 추출
	//if (words.front() == L"main") //main 함수면 따로 처리
	//	this->in_main_func = true;

	//
	//func_signature += in_main_func ? L"wmain" : words.front();
	//words.pop();

	////파라미터 체크
	//if (words.front() != L"(")
	//	this->print_error(L"여는괄호 어딨어요"+words.front());
	//else
	//{
	//	func_signature += words.front();
	//	words.pop();
	//}

	//while (words.front() != L")")
	//{

	//	/*	main 함수	*/
	//		if (this->in_main_func)
	//		{
	//			this->cmdline_arg_name = words.front();
	//			words.pop();

	//			if (words.front() != L")") //뭐가 더 있음
	//			{
	//				wprintf(func_signature.c_str());
	//				this->read_line_if_empty();
	//				if (words.front() != L":")
	//				{
	//					words.pop();
	//					words.pop();
	//				}
	//				else
	//					this->print_error(L"명령행 인자에는 하나만 넣을 수 있어요");
	//			}

	//			func_signature += L"int ";
	//			func_signature += TEMP_argc;
	//			func_signature += L",wchar_t** ";
	//			func_signature += TEMP_args;

	//			break;
	//		}
	//	/*	main 함수	*/


	//	//한정자
	//	if (words.front() == keywords::MUT)
	//		words.pop();
	//	else if (words.front() == keywords::CONST_)
	//	{
	//		func_signature += L"const ";
	//		words.pop();
	//	}
	//	else //디폴트 const
	//		func_signature += L"const ";


	//	//변수명 저장
	//	auto var_name = words.front();
	//	words.pop();

	//	if (words.front() != L":")
	//		this->print_error(L"파라미터에 콜론 어디갔어요?");
	//	words.pop();

	//	//타입
	//	func_signature += words.front();
	//	func_signature += ' ';
	//	func_signature += var_name;
	//	words.pop();

	//	//콜론
	//	if (words.front() == L",")
	//	{
	//		func_signature += words.front();
	//		words.pop();
	//	}
	//}
	//func_signature += words.front();
	//words.pop();

	////리턴타입
	//if (words.empty() || words.front() == L"{") //없으면 void
	//	if (in_main_func)
	//		func_signature += L"->int";
	//	else
	//		func_signature += L"->void";
	//else if (words.front().find(L"->") != wstring::npos)
	//{
	//	if (words.front() == L"->") //->와 타입이 떨어져있을 경우
	//	{
	//		func_signature += words.front();
	//		words.pop();
	//	}
	//	func_signature += words.front();
	//	words.pop();
	//}
	//else
	//	this->print_error(L"리턴타입 부분이 이상한거같은데");

	//this->prototypes += func_signature;
	//this->prototypes += L";\n";
	//this->bodys += func_signature;
	//this->bodys += '\n';

	////
	//if (words.empty())
	//	this->read_line();
	//if (words.front() == L"{")
	//{
	//	this->bodys += words.front(); words.pop();
	//	this->unclosed_bracket++;
	//	this->interpret_local();
	//}
	//return;
}


void Interpreter::interpret_variable()
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

