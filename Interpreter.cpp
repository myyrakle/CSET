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
	//...
	while(!tokens.empty())
		this->interpret_global(tokens);

	this->finish();
}

void Interpreter::do_import(const std::wstring& module_name)
{
	if (module_name == L"gui") //종속성 체크
	{
		caller->builder.add_file(caller->_original_filepath
			+ L"Library\\SFML\\lib\\sfml-window-s-d.lib");
		caller->builder.add_file(caller->_original_filepath
			+ L"Library\\SFML\\lib\\sfml-graphics-s-d.lib");
	}
	else if (module_name == L"audio")
	{
		caller->builder.add_file(caller->_original_filepath
			+ L"Library\\SFML\\lib\\sfml-audio-s-d.lib");
	}
	else if (module_name == L"netword")
	{
		caller->builder.add_file(caller->_original_filepath
			+ L"Library\\SFML\\lib\\sfml-network-s-d.lib");
	}

	//Library 경로 모듈 읽어넣음
	std::wstring module_path = caller->_original_filepath + L"\\Library\\" 
		+ module_name + L".h";

	wifstream read_module;
	
	read_module.open(module_path);

	if (!read_module) 
		this->print_error(L"모듈이 안열리네요. : "+caller->_original_filepath);
	
	std::wstring temp;

	while (read_module)
		temp += read_module.get();

	caller->headers += temp;

	caller->headers += L"\n\n";

	read_module.close();
}

bool Interpreter::is_keyword(const std::wstring & word) const
{
	if (word == keywords::BOOL ||
		word == keywords::BYTE ||
		word == keywords::ALIAS ||
		word == keywords::BREAK ||
		word == keywords::CASE ||
		word == keywords::CHAR ||
		word == keywords::CLASS ||
		word == keywords::CONSTFUNC ||
		word == keywords::CONST_ ||
		word == keywords::CONTINUE ||
		word == keywords::DOUBLE ||
		word == keywords::ELSE ||
		word == keywords::ENUM ||
		word == keywords::FALSE_ ||
		word == keywords::FLOAT ||
		word == keywords::FOR ||
		word == keywords::FUNC ||
		word == keywords::FUNCTYPE ||
		word == keywords::IF ||
		word == keywords::IMPORT ||
		word == keywords::INT ||
		word == keywords::INT16 ||
		word == keywords::INT32 ||
		word == keywords::INT64 ||
		word == keywords::INT8 ||
		word == keywords::IN_ ||
		word == keywords::LABEL |
		word == keywords::LITERAL ||
		word == keywords::MATCH ||
		word == keywords::MUT ||
		word == keywords::NAMESPACE ||
		word == keywords::NEW ||
		word == keywords::NULL_ ||
		word == keywords::OBJECT ||
		word == keywords::PRIVATE ||
		word == keywords::PROTECTED ||
		word == keywords::PUBLIC ||
		word == keywords::RCNEW ||
		word == keywords::RFOR ||
		word == keywords::SIZEOF ||
		word == keywords::STATIC ||
		word == keywords::STRING ||
		word == keywords::SWITCH ||
		word == keywords::TRUE_ ||
		word == keywords::UINT ||
		word == keywords::UINT16 ||
		word == keywords::UINT32 ||
		word == keywords::UINT64 ||
		word == keywords::UINT8 ||
		word == keywords::USING ||
		word == keywords::WHILE)
		return true;
	else
		return false;
}

void Interpreter::finish()
{
	wofstream fout(caller->_original_filepath + L"\\temp\\temp.cpp");

	for (auto& source : caller->headers)
		fout << source;
	for (auto& source : caller->prototypes)
		fout << source;
	for (auto& source : caller->bodys)
		fout << source;

	fout.close();
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
	wprintf(L"파일명 %s\n",caller->current_filename.data());
	wprintf(L"라인 : %d\n", caller->line_number);
	wprintf(L"%s", log.data());
	
	this->finish();

	exit(EXIT_SUCCESS);
}

void Interpreter::interpret_global(std::queue<std::wstring>& tokens)
{
	//func 키워드 만날 경우
	if (tokens.front() == keywords::FUNC)
	{
		this->interpret_function(tokens); //시그너처 파싱
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
		this->interpret_variable(tokens);
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
		this->print_error(L"아직 클래스 없어요");
	}

	else if (tokens.front() == keywords::NAMESPACE)
	{
		//To do
		this->print_error(L"아직 네임스페이스 없어요");
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
		while (!tokens.empty())
		{
			caller->bodys = tokens.front(); tokens.pop();
		}
		caller->prototypes += L";\n";
		return;
	}
}

void Interpreter::interpret_local(std::queue<std::wstring>& tokens, int unclosed_bracket)
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
			//caller->bodys += L"auto ___strlen = [](const char* str){ size_t i=0; for(;s[i]!='\0';i++); return i; };";
			caller->bodys += L"DynamicArray<String> "; caller->bodys += cmdline_arg_name;
			caller->bodys += '('; caller->bodys += TEMP_argc; caller->bodys += L");\n";

			caller->bodys += L"for(int i=0;i<"; caller->bodys += TEMP_argc; caller->bodys += L";i++)\n\t";
			caller->bodys += cmdline_arg_name;
			caller->bodys += L"[i]="; caller->bodys += TEMP_args; caller->bodys += L"[i];";
			
			cmdline_arg_name.clear();
		}
	}

	//중괄호 다 닫힐때까지 루프
	while (true) 
	{
		while (tokens.empty())
		{
			if (caller->readable())
				tokens = std::move(caller->read_line_then_tokenize());
			else
				return;

			if (!this->pre_word.empty())
				caller->bodys += pre_word;

			//흠..
			caller->bodys += L";\n";
			if (straight)
				caller->bodys += L"}}";
		}

		if (tokens.front() == L"{")
		{
			unclosed_bracket++;
			caller->bodys += tokens.front(); tokens.pop();
			caller->bodys += L'\n';
			//continue;
		}
		else if (tokens.front() == L"}")
		{
			unclosed_bracket--;
		
			caller->bodys += L";\n";
			caller->bodys += tokens.front(); tokens.pop();
			caller->bodys += L'\n';

			

			/*임시방편*/

			/*임시방편*/

			//continue;
		}

		//const나 mut, literal 키워드일 경우

		else if (tokens.front() == keywords::STATIC)
		{
			caller->bodys += L"static ";
			tokens.pop();
		}

		else if (tokens.front() == keywords::CONST_ 
			|| tokens.front() == keywords::MUT
			|| tokens.front() == keywords::LITERAL)
		{
			this->interpret_variable(tokens);
			caller->bodys += ';';
		}

		else if (tokens.front() == keywords::CLASS)
		{
			this->print_error(L"아직 로컬 클래스 미지원");
		}

		else if (tokens.front() == keywords::ENUM)
		{
			this->print_error(L"아직 로컬 열거 미지원");
		}

		else if (tokens.front() == L"$")
		{
			state = InterpreterState::In_LAMBDA;
			caller->bodys += L"[&]"; tokens.pop();
			this->interpret_lambda(tokens);
		}

		else if (tokens.front() == L"for")
		{
			this->interpret_for(tokens);

			while (tokens.empty())
			{
				if (caller->readable())
					tokens = std::move(caller->read_line_then_tokenize());
				else
					return;
			}
		}

		else if (tokens.front() == L"rfor")
		{
			this->print_error(L"rfor는 아직 미완성이에요");
			this->interpret_rfor(tokens);

			while (tokens.empty())
			{
				if (caller->readable())
					tokens = std::move(caller->read_line_then_tokenize());
				else
					return;
			}

			if (tokens.front() == L"{")
			{
				//Todo
			}
			else
			{
				straight = true;
			}
		}

		else if(tokens.front() ==L"while")
		{ 
			this->interpret_while(tokens);
		}

		else if (tokens.front() == L"if")
		{
			this->interpret_if(tokens);
		}

		else if (tokens.front() == L"else")
		{
			this->interpret_if(tokens);
		}

		else //나머지는 그냥
		{
			interpret_check_Range(tokens);
		}

		if (unclosed_bracket == 0)
		{
			break;
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
		this->print_error(L"여는괄호 어딨어요. 이게 나오네요-> "+tokens.front());
	else
	{
		func_signature+=tokens.front(); tokens.pop();
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
			
			if (tokens.front() == L"string")
			{
				tokens.pop();
			}
			else
				this->print_error(L"명령행 인자는 string 배열만 올수 있어요");

			if (tokens.front() == L"[")
				tokens.pop();
			else
				this->print_error(L"명령행 인자는 string 배열만 올수 있어요");

			if (tokens.front() == L"]")
				tokens.pop();
			else
				this->print_error(L"명령행 인자는 string 배열만 올수 있어요");

			if (tokens.front() != L")")
				this->print_error(L"닫는괄호 어디갔어요?");
			tokens.pop();

			func_signature += L"int ";
			func_signature += TEMP_argc;
			func_signature += L",wchar_t** ";
			func_signature += TEMP_args;
			func_signature += L')';

			if(tokens.empty()||tokens.front()==L"{")
			{
				func_signature += L"->int";
			}
			else
			{
				if (tokens.front() == L"->")
					func_signature += L"->", tokens.pop();
				else
					this->print_error(L"반환타입 어딨어요"+tokens.front()+L"흠");

				if (tokens.front() == L"int")
					func_signature += L"int", tokens.pop();
				else
					this->print_error(L"main 함수 반환형은 무조건 int나 void에요");
			}
		}
	}

	/*main 함수가 아닌 경우들*/
	else
	{
		while (tokens.front() != L")")
		{
			bool is_mut = false;

			if (tokens.front() == keywords::MUT)
				tokens.pop(), is_mut = true;

			//한정자
			else if (tokens.front() == keywords::CONST_)
			{
				tokens.pop();
			}
			
			if (!is_mut)
				func_signature += L"const ";

			//변수명 저장
			auto var_name = tokens.front(); tokens.pop();

			if (tokens.front() != L":")
				this->print_error(L"파라미터에 콜론 어디갔어요? <- "+tokens.front());
			else
				tokens.pop();

			func_signature += interpret_types(tokens);

			//타입
			func_signature += L" " + var_name;

			//콜론
			if (tokens.front() == L",")
			{
				func_signature += tokens.front();
				tokens.pop();
			}
			else
			{
				if (tokens.front() != L")")
					print_error(L"뭐야 이게 <-"+tokens.front());
			}
		}

		func_signature += tokens.front();  tokens.pop(); // ) 붙임	

		while (tokens.empty())
			if (caller->readable())
				tokens = std::move(caller->read_line_then_tokenize());

		if (tokens.empty() || tokens.front() == L"{") //없으면 void
			func_signature += L"->void";
		else if (tokens.front() == L"->")
		{
			func_signature += tokens.front(); tokens.pop();
			func_signature += this->interpret_types(tokens);
		}
		else
			this->print_error(L"리턴타입 부분이 이상한거같은데");
	}

	caller->headers += func_signature;
	caller->headers += L";\n";
	caller->prototypes += func_signature;
	caller->prototypes += L";\n";
	caller->bodys += func_signature;
	caller->bodys += '\n';

	while (tokens.empty())
		if (caller->readable())
			tokens = std::move(caller->read_line_then_tokenize());

	if (tokens.front() == L"{")
	{
		caller->bodys += tokens.front(); tokens.pop();
		this->interpret_local(tokens,1);
	}
	return;
}

void Interpreter::interpret_check_Range(std::queue<std::wstring>& tokens)
{
	if (tokens.front() == L"(")
	{
		unclosed_small_bracket++;
	}
	else if (tokens.front() == L")")
	{
		unclosed_small_bracket--;
	}

	if (in_range)
	{
		post_word += tokens.front(); tokens.pop();

		if (unclosed_small_bracket == 0)
		{
			std::wstring temp = L"Range(";
			temp += pre_word;
			temp += L',';
			temp += post_word;
			temp += L')';

			pre_word.clear();
			post_word.clear();
			in_range = false;

			caller->bodys += temp;
		}
	}

	//기본동작
	else
	{
		//.. 범위연산자 대비해서 operand1 저장		
		pre_word += tokens.front(); tokens.pop(); //뽑아놓고

		if (unclosed_small_bracket == 0)
		{
			if (tokens.front() == L"..") //..인지 검사
			{
				in_range = true;
				tokens.pop(); //범위연산자 처리문으로 이동
			}
			else
			{
				caller->bodys += pre_word;
				pre_word.clear(); //아니면 그냥 넘김
			}
		}
	}
}

/*갈아치울 필요*/
void Interpreter::interpret_lambda(std::queue<std::wstring>& tokens)
{
	if (tokens.front() == L"(") //파라미터 있을 경우
	{
		//interpret_function 복붙

		std::wstring lambda_signature = L"("; tokens.pop();

		while (tokens.front() != L")")
		{
			bool is_mut = false;

			if (tokens.front() == keywords::MUT)
				tokens.pop(), is_mut = true;

			//한정자
			else if (tokens.front() == keywords::CONST_)
			{
				tokens.pop();
			}

			if (!is_mut)
				lambda_signature += L"const ";

			//변수명 저장
			auto var_name = tokens.front(); tokens.pop();

			if (tokens.front() != L":")
				this->print_error(L"파라미터에 콜론 어디갔어요?");
			tokens.pop();

			lambda_signature += interpret_types(tokens);

			//타입
			lambda_signature += ' ';
			lambda_signature += var_name;
			
			//콜론
			if (tokens.front() == L",")
			{
				lambda_signature += tokens.front();
				tokens.pop();
			}
			else
				print_error(L"뭐야 이게" + tokens.front());
		}

		lambda_signature += tokens.front();  tokens.pop(); // ) 붙임

	//리턴타입
		while (tokens.empty())
			if(caller->readable())
				tokens = std::move(caller->read_line_then_tokenize());

		if(tokens.front() == L"{") //없으면 void
			lambda_signature += L"->void";
		else if (tokens.front() == L"->")
		{
			lambda_signature += tokens.front(); tokens.pop();
			lambda_signature += this->interpret_types(tokens);
		}
		else
			this->print_error(L"리턴타입 부분이 이상한거같은데");

		caller->bodys += lambda_signature;
		caller->bodys += '\n';
	}
	
	if(tokens.front() == L"{")//파라미터 없을 경우
	{
		this->interpret_local(tokens, 1);
	}
}

void Interpreter::interpret_for(std::queue<std::wstring>& tokens)
{
	caller->bodys += L"for"; tokens.pop();

	if (tokens.front() != L"(")
		this->print_error(L"for에 여는괄호 어딨어요");
	
	caller->bodys += tokens.front(); tokens.pop(); // ( 넣음

	int unclosed_bracket = 1; //안닫힌 소괄호

	//each 변수가 mutable인가?
	bool each_is_mut = false;
	
	if (tokens.front() == L"mut")
		each_is_mut = true, tokens.pop();
	else if (tokens.front() == L"const")
		tokens.pop();

	//each 변수 체크
	std::wstring each_name = tokens.front(); tokens.pop();

	if (tokens.front() == keywords::IN_)
	{
		if (each_is_mut)
			caller->bodys += L"auto&";
		else
			caller->bodys += L"const auto&";

		caller->bodys += each_name;

		caller->bodys += L":"; tokens.pop();
	}
	else
	{
		print_error(L"in 어딨어요");
	}

	if (tokens.empty())
		print_error(L"컨테이너 어딨어요");

	//닫는괄호 나올때까지 입력
	
	while(unclosed_bracket!=0)
	{
		if (tokens.front() == L"(")
			unclosed_bracket++;
		else if (tokens.front() == L")")
			unclosed_bracket--;

		if (unclosed_bracket == 0)
			break;

		this->interpret_check_Range(tokens);
	}
	caller->bodys += tokens.front(); tokens.pop();
}

/*
{
auto 컨테이너 = 표현식;
for(auto i = 컨테이너.rbegin(); i!=컨테이너.rend();i++)
...
}
*/

/*결함투성이*/
void Interpreter::interpret_rfor(std::queue<std::wstring>& tokens)
{
	caller->bodys += L"{\n"; more_bracket_in_rfor++;
	tokens.pop(); //rfot 삭제

	if (tokens.front() != L"(")
		this->print_error(L"rfor에 여는괄호 어딨어요");

	tokens.pop(); // ( 뺌

	int unclosed_bracket = 1; //안닫힌 소괄호

	//each 변수가 mutable인가?
	bool each_is_mut = false;

	if (tokens.front() == L"mut")
		each_is_mut = true, tokens.pop();
	else if (tokens.front() == L"const")
		tokens.pop();

	//each 변수 체크
	std::wstring each_name = tokens.front(); tokens.pop();

	std::wstring container_name = TEMP_name + L"__contanier";

	if (tokens.front() == keywords::IN_)
	{
		caller->bodys += L"auto " + container_name;

		tokens.pop();
	}
	else
	{
		print_error(L"in 어딨어요");
	}

	if (tokens.empty())
		print_error(L"컨테이너 어딨어요");

	//닫는괄호 나올때까지 입력

	while (unclosed_bracket != 0)
	{
		if (tokens.front() == L"(")
			unclosed_bracket++;
		else if (tokens.front() == L")")
			unclosed_bracket--;

		if (unclosed_bracket == 0)
			break;

		this->interpret_check_Range(tokens);
	}
	tokens.pop(); // ) 제거
	
	auto iter_name = TEMP_name + L"__iter";

	caller->bodys += L"for(auto "; caller->bodys += iter_name;

	caller->bodys += '='; caller->bodys += container_name + L".rbegin(); ";

	caller->bodys += iter_name; caller->bodys += L"!="; 
	
	caller->bodys += container_name + L"rend();";

	caller->bodys += iter_name + L"++)\n";

	caller->bodys += L"{\n"; more_bracket_in_rfor++;

	caller->bodys += each_is_mut ? L"auto& " : L"const auto& ";

	caller->bodys += each_name + L"*" + iter_name + L";\n";
}

void Interpreter::interpret_while(std::queue<std::wstring>&)
{
}

void Interpreter::interpret_if(std::queue<std::wstring>&)
{
}

void Interpreter::interpret_else(std::queue<std::wstring>&)
{
}


std::wstring Interpreter::interpret_types(std::queue<std::wstring>& tokens)
{
	//전체적 개선 필요
	//서로 섞이지 못함

	std::wstring type;

	std::wstring array_expr;
	int unclosed_bracket = 0;

	while (true)
	{
		if (tokens.front() == L")" ||
			tokens.front() == L"," ||
			tokens.front() == L"=" ||
			tokens.front() ==L"{")
			break;

		

		//타입 키워드 충돌 방지
		if (tokens.front() == L"int" ||
			tokens.front() == L"double" || 
			tokens.front() == L"float" ||
			tokens.front() == L"char" ||
			tokens.front() == L"bool")
		{
			tokens.front()[0] -= ((int)'a' - (int)'A');
			type += tokens.front(); tokens.pop();
		}

		//배열 축약 변환
		else if(tokens.front()==L"[")
		{
			//static_array<길이, 타입>
			array_expr += L"Array<";
			array_expr += tokens.front(); tokens.pop();

			unclosed_bracket++;
		}
		else if (tokens.front() == L"]") //dynamic_array
		{
			unclosed_bracket--;
			array_expr += L"DynamicArray<"; tokens.pop();
			
			if (tokens.front() == L"[")
			{

			}
			else
			{
				type = array_expr + type + std::wstring(unclosed_bracket, '>'); //??
			}
		}
			
		//박스 체크
		else if (tokens.front() == L"*")
		{
			//아직 미구현
		}

		//rc 박스 체크
		else if (tokens.front() == L"^")
		{
			//아직 미구현
		}

		//nullable 체크
		else if (tokens.front() == L"?")
		{
			//아직 미구현
		}

		else
		{
			type += tokens.front(); tokens.pop();
		}
	}

	return type;
}

void Interpreter::interpret_variable(std::queue<std::wstring>& tokens)
{
	wstring variable_expr = L"";
	bool has_init = false;

	auto head_word = tokens.front(); tokens.pop();

	if (head_word == keywords::LITERAL)
	{
		variable_expr += L"constexpr auto ";

		while (!tokens.empty())
		{
			variable_expr += tokens.front();
			variable_expr += ' ';
			tokens.pop();
		}
	}

	else if (head_word == keywords::CONST_ || head_word == keywords::MUT)
	{
		//const 여부 체크
		if (head_word == keywords::CONST_)
			variable_expr += keywords::CONST_, variable_expr += ' ';
		tokens.pop();

		//변수 이름 저장
		wstring var_name = tokens.front();
		tokens.pop();

		//타입 체크
		if (tokens.front() == L":") //타입이 명시될 경우
		{
			tokens.pop();
			variable_expr += interpret_types(tokens);
		}
		else //없으면 추론
		{
			variable_expr += L"auto";
		}

		variable_expr += ' ';
		variable_expr += var_name;

		//나머지 갖다붙이기
		while (!tokens.empty())
		{
			variable_expr += tokens.front();
			variable_expr += ' ';
			tokens.pop();
		}
	}
	else
		print_error(L"뭐야ㅕ");


	caller->bodys += variable_expr;
}

