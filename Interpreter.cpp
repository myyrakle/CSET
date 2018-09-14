#include "stdafx.h"
#include "Interpreter.h"

void Interpreter::add_file(wstring && filename)
{
	filenames.emplace_back(std::forward<wstring&&>(filename));
}

void Interpreter::interpret()
{
	//전부 읽어서 저장.
	for (auto& filename : filenames)
	{
		this->fin.open(filename);
		
		this->line_num = 0;
		while (this->read_line())
			this->interpret_global();

		fin.close();
	}
}

void Interpreter::finish()
{
	wofstream fout(L"temp\\temp.cpp");

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

void Interpreter::line_to_words() //한줄 읽어서 화이트스페이스 기준으로 단어 분리. 코멘트는 삭제. 문자열 리터럴은 wstring으로 처리
{
	wstring word;
	wstring text_literal=L"";

	bool in_text = false;
	bool quote_is_open = false;

	bool is_escape = false;

	bool has_one_slash = false;
	bool in_comment = false;

	for (auto& c : this->line)
	{
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

		if (c == '\\') //이스케이프 문자 체크
		{
			is_escape = true;
			text_literal += '\\';
		}
		else if (c == '\"')
		{
			if (quote_is_open == false) //들어갈 때
			{
				text_literal = L"wstring(";
				in_text = true;
			}
			else if (is_escape == false)//나갈 때
			{
				text_literal += L"\")";
				this->words.push(text_literal);
				in_text = false;
			}
			else // " 이스케이프 문자일 때
				text_literal += c;
		}
		else if (in_text) //문자열 리터럴 탐색중
		{
			text_literal += c;
		}
		else if (c == '\n' || c == '\t' || c == ' ') //화이트스페이스 기준 분리
		{
			if (word != L"")
			{
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

bool Interpreter::read_line_if_words_empty()
{
	if (this->words.empty())
		return this->read_line();
	return false;
}

/* deprecated
void Interpreter::chars_to_wstring()
{
	auto left_quote = this->line.find('\"');

	if (left_quote != wstring::npos)
	{
		auto right_quote = this->line.rfind('\"');
		if (left_quote == left_quote)
			throw new exception("따옴표 하나 어디갔소?");

		this->line.insert(left_quote - 1, L"wstring(");
		this->line.insert(right_quote + 1, L")");
	}
}
//코멘트, 큰따옴표 우선순위 충돌 미해결
void Interpreter::remove_comment()
{
	if (this->line.find(L"//") != wstring::npos)
	{
		this->line = line.substr(0, this->line.find(L"//"));
	}
	else
		return;
}
*/

void Interpreter::interpret_global()
{
	auto head_word = words.front();

	//func 키워드 만날 경우
	if (this->words.front() == keywords::FUNC)
		this->interpret_function();

	//const나 mut, literal 키워드일 경우
	else if (head_word == keywords::CONST || head_word == keywords::MUT || head_word == keywords::LITERAL)
		this->interpret_variable();

	//class 키워드일 경우
	else if (head_word == keywords::CLASS)
		this->interpret_class;

	/*
	//static일 경우
	else if (head_word == keywords::STATIC)
	{
		if (head_word == keywords::FUNC) //static 함수
			this->interpret_function();

		else if (head_word == keywords::CONST || this->words.front() == keywords::MUT) // static 변수
			this->interpret_variable();
	}
	*/

	else if (this->words.front() == keywords::NAMESPACE);

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
		bodys += ';';
		return;
	}
}

void Interpreter::interpret_local()
{

}

void Interpreter::interpret_function()
{
	wstring func_signature = L"";

	if (this->words.front() == keywords::STATIC)
		func_signature += keywords::STATIC, this->words.pop();
	
	if (this->words.front() == keywords::FUNC)
		func_signature += L"auto", this->words.pop();

	//함수명 추출
	{
		wstring func_name;
		auto left_bracket = words.front().find('(');
		if (left_bracket != wstring::npos) // (가 붙어있을 경우 떼냄
		{
			func_name = words.front().substr(0, left_bracket);
			words.front() = words.front().substr(left_bracket);
		}
		else
		{
			func_name = words.front();
			words.pop();
		}
		func_signature += ' ';
		func_signature += func_name;

		if (func_name == L"main") //main 함수면 따로 처리
		{
			this->interpret_function_main(func_signature);
			return;
		}
	}

	//파라미터 체크
	if (words.front() == L"(") //(만 떨어져있을 경우
	{
		func_signature 
	}
	if (head_word == keywords::CONST || head_word == keywords::MUT)
	{
		//const 여부 체크
		if (head_word == keywords::CONST)
			variable_expr += keywords::CONST, variable_expr += ' ';
		this->words.pop();

		//변수 이름 저장
		wstring var_name;
		auto cur_word = this->words.front();
		if (cur_word.find(':') == wstring::npos)
		{
			var_name = cur_word;
			this->words.pop();
		}
		else // 이름에 : 붙어있을 경우
		{
			var_name = cur_word.substr(0, cur_word.find(':'));
			this->words.front() = cur_word.substr(cur_word.find(':'));
		}

		//타입체크 + 변수명 연결
		if (this->words.front().find(':') == wstring::npos) //:타입 표기 없으면 초기화 추론
		{
			variable_expr += L" auto ";
			variable_expr += var_name;
		}
		else //있으면 표기
		{
			if (words.front() == L":") //:만 달랑 있으면
			{
				words.pop(); //버리고

				variable_expr += words.front(); //다음 워드의 타입 저장
			}
			else //:이 타입이랑 붙어있으면
			{
				variable_expr += words.front().substr(1); //떼어내고 타입 저장
			}
			words.pop();
			variable_expr += ' ';

			variable_expr += var_name;
		}
	//

}

void Interpreter::interpret_function_main(const wstring& func_signature)
{

}

void Interpreter::interpret_parameter()
{

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

	else if (head_word == keywords::CONST || head_word == keywords::MUT)
	{
		//const 여부 체크
		if (head_word == keywords::CONST)
			variable_expr += keywords::CONST, variable_expr += ' ';
		this->words.pop();

		//변수 이름 저장
		wstring var_name;
		auto cur_word = this->words.front();
		if (cur_word.find(':') == wstring::npos)
		{
			var_name = cur_word;
			this->words.pop();
		}
		else // 이름에 : 붙어있을 경우
		{
			var_name = cur_word.substr(0, cur_word.find(':'));
			this->words.front() = cur_word.substr(cur_word.find(':'));
		}

		//타입체크 + 변수명 연결
		if (this->words.front().find(':') == wstring::npos) //:타입 표기 없으면 초기화 추론
		{
			variable_expr += L" auto ";
			variable_expr += var_name;
		}
		else //있으면 표기
		{
			if (words.front() == L":") //:만 달랑 있으면
			{
				words.pop(); //버리고

				variable_expr += words.front(); //다음 워드의 타입 저장
			}
			else //:이 타입이랑 붙어있으면
			{
				variable_expr += words.front().substr(1); //떼어내고 타입 저장
			}
			words.pop();
			variable_expr += ' ';

			variable_expr += var_name;
		}
		//

		//나머지 갖다붙이기
		while (!words.empty()) 
		{
			variable_expr += words.front();
			variable_expr += ' ';
			words.pop();
		}
		variable_expr += ';';
	}


	
}