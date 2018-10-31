#include "pch.h"
#include "Tokenizer.h"

//한줄 읽어서 화이트스페이스 기준으로 단어 분리. 
//코멘트는 삭제. 
//문자열 리터럴은 String으로 처리
//ToDo : 정수는 Int로 처리
//ToDo : 실수는 Double로 처리
//ToDo : 문자는 Char로 처리


//블럭 단위 현재 플래그입니다.
enum class BlockFlag
{
	BlockComment,
	BlockString,
	DEFAULT,
};

//한줄 단위 현재 플래그입니다.
enum class LineFlag
{
	COMMENT, //주석을 읽는 중입니다.
	STRING, //문자열을 읽는 중입니다.
	CHAR, //문자를 읽는 중입니다.
	INTEGER, //정수를 읽는 중입니다.
	ID, //기타 식별자입니다.
	DEFAULT, //기본 플래그입니다.
};

std::queue<std::wstring> Tokenizer::tokenize(const std::wstring& line)
{
	std::queue<std::wstring> tokens; //토큰화한 결과물

	wstring word; //단어 하나

	LineFlag line_flag = LineFlag::DEFAULT;
	static BlockFlag block_flag = BlockFlag::DEFAULT;

	bool in_escape = false;

	for (auto& c : line)
	{
		switch (line_flag)
		{
		case LineFlag::DEFAULT : 
		{
			switch (c)
			{
			case '\'': line_flag = LineFlag::CHAR; 
				if (!word.empty()) 
					tokens.push(std::move(word));
				word = L"Char(L\'";
				break;

			case '\"': line_flag = LineFlag::STRING; 
				if (!word.empty())
					tokens.push(std::move(word));
				word = L"String(L\"";
				break;

			//화이트스페이스 처리
			case ' ' : 
			case '\n' :
			case '\t' :
				break;

			//괄호 처리
			case '(': 
			case ')':
			case '{':
			case '}':
			case '[':
			case ']':
				if (!word.empty()) 
					tokens.push(std::move(word));
				tokens.emplace(1, c);
				break;

			//연산자 처리
			case '+':
			case '-':
			case '/':
			case '*':
			case '%':
			case '&':
			case '|':
			case '?':
			case '!':
			case '=':
			case '<':
			case '>':
			case '^':
			case '~':
			case '.':
			case ';':
			case ':':
				if (!word.empty()) 
					tokens.push(std::move(word));
				word += c;
				break;

			default: word += c;
				if (word == L"//")
				{
					line_flag = LineFlag::COMMENT;
					continue;
				}
			}
		} break;

		case LineFlag::COMMENT : //주석문 처리중
		{
			
		} break;

		case LineFlag::STRING : //문자열 리터럴 처리중
		{
			if (c == '\\')
				if(in_escape==false)
				{
					in_escape = true;
					word += c;
					continue;
				}

			else if (c == '\"'&&in_escape==false)
			{
				word += L"\")";
				tokens.push(std::move(word));
				line_flag = LineFlag::DEFAULT;
				continue;
			}

			if (in_escape == true)
				in_escape = false;

			word += c;

		} break;

		case LineFlag::CHAR : //문자 리터럴 처리중
		{

		} break;

		}


		//화이트스페이스 기준 분리
		if (c == L'\n' || c == L'\t' || c == L' ')
		{
			if (!word.empty())
			{
				this->convert_typename(word);
				this->words.push(word); /*체크용*/ wcout << L"체크 : " << word << endl; /*체크용*/
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
