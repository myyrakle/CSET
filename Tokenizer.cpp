#include "pch.h"
#include "Tokenizer.h"

//한줄 읽어서 화이트스페이스 기준으로 단어 분리. done
//코멘트는 삭제. done
//문자열 리터럴은 String으로 처리 done
//ToDo : 정수는 Int로 처리 not yet
//ToDo : 실수는 Double로 처리 not yet
//ToDo : 문자는 Char로 처리 done
//ToDo : 블럭단위 코멘트, 문자열 처리 not yet 


//한줄 단위 현재 플래그입니다.
enum class StateFlag
{
	BLOCK_COMMENT, //블럭단위 주석을 읽는 중입니다.
	BLOCK_STRING, //블럭단위 문자열을 읽는 중입니다.

	LINE_COMMENT, //주석을 읽는 중입니다.
	STRING, //문자열을 읽는 중입니다.
	CHAR, //문자를 읽는 중입니다.
	NUMBER,
	INTEGER, //정수를 읽는 중입니다.
	FLOAT,
	ID, //기타 식별자입니다.
	DEFAULT, //기본 플래그입니다.
};

std::queue<std::wstring> Tokenizer::tokenize(const std::wstring& line)
{
	std::queue<std::wstring> tokens; //토큰화한 결과물

	wstring word; //기본 어휘단어 하나

	StateFlag flag = StateFlag::DEFAULT;

	if (this->in_block_comment)
		flag = StateFlag::BLOCK_COMMENT;
	if (this->in_block_string)
		flag = StateFlag::BLOCK_STRING;

	bool in_escape = false; //이스케이프 문자 구별용 플래그

	//std::wcout << line[0] << std::endl;


	for (int i = 0; line[i] != '\0'; i++)
	{
		const wchar_t c = line[i];

		//std::wcout << c << std::endl;

		switch (flag)
		{
		case StateFlag::DEFAULT: //기본 진입점
		{
			switch (c)
			{
				//문자 리터럴 처리
			case '\'': flag = StateFlag::CHAR;
				if (!word.empty())
					tokens.push(std::move(word));
				word = L"Char(L\'";
				break;

				//문자열 리터럴 처리
			case '\"': flag = StateFlag::STRING;
				if (!word.empty())
					tokens.push(std::move(word));
				word = L"String(L\"";
				break;

				//화이트스페이스 처리
			case ' ': /*case '\n':*/ case '\t':
				//무시함
				if (!word.empty())
				{
					tokens.push(std::move(word));
					tokens.emplace(1, c);
				}
				
				break;

				//바로바로 분리해도 되는 것들
			case '(': case ')':
			case '{': case '}':
			case '[': case ']':
			case '$': case ';':
			case ',':
				if (!word.empty())
					tokens.push(std::move(word));
				tokens.emplace(1, c);
				break;

				//연산자 처리
			case '+': case '-': case '|': case '&': case '=': case '<': case '>':
				if (!word.empty()) tokens.push(std::move(word)); //일단 보내버리고
				if (line[i + 1] != c && line[i + 1] != '=') //2글자 연산자 아니면
				{
					tokens.emplace(1, c);
				}
				else //두글자 연산자면
				{
					word += c;
					word += line[i + 1];
					i++;
					tokens.push(std::move(word));
				}
				break;

				//나눗셈 기호? 주석?
			case '/':
				if (!word.empty()) 
					tokens.push(std::move(word)); //일단 보내버리고
				if (line[1] == '/') //주석기호면
					flag = StateFlag::LINE_COMMENT;
				else if (line[1] == '*') // 블럭단위 주석 표시. 아직 미구현
					;
				else if (line[1] == '=') // /=면
				{
					tokens.emplace(L"/=");
					i++;
				}
				else //그냥 /면
					tokens.emplace(1, c);
				break;

				// ?/?? or ./.. or :/::
			case ':': case '?': case '.':
				if (!word.empty()) 
					tokens.push(std::move(word)); //일단 보내버리고
				if (line[i + 1] == c) //??나 ..면
				{
					tokens.emplace(2, c);
					i++;
				}
				else // ?나 .면
					tokens.emplace(1, c);
				break;

				//단문자나 =버전만 있음
			case '*': case '~': case '!': case '%': case '^':
				if (!word.empty()) 
					tokens.push(std::move(word)); //일단 보내버리고
				if (line[i + 1] == '=')
				{
					word += c;
					word += '=';
					tokens.push(std::move(word));
					i++;
				}
				else
					tokens.emplace(1, c);
				break;

			case L'a': case L'b': case L'c': case L'd': case L'e': case L'f': case L'g':
			case L'h': case L'i': case L'j': case L'k': case L'l': case L'm': case L'n':
			case L'o': case L'p': case L'q': case L'r': case L's': case L't': case L'u':
			case L'v': case L'w': case L'x': case L'y': case L'z':
			case L'A': case L'B': case L'C': case L'D': case L'E': case L'F': case L'G':
			case L'H': case L'I': case L'J': case L'K': case L'L': case L'M': case L'N':
			case L'O': case L'P': case L'Q': case L'R': case L'S': case L'T': case L'U':
			case L'V': case L'W': case L'X': case L'Y': case L'Z':
			case L'_':
				word += c;
				flag = StateFlag::ID;
				break;

			case '0': case '1': case '2': case '3': case '4': case '5': case'6':
			case '7': case '8': case '9':
				word += c;
				flag = StateFlag::NUMBER;
				break;

			default: //???
				//std::cout << "이게 수행되나" << std::endl;
				word += c;
				break;
			}
		} 
		break;

		case StateFlag::ID: //식별자를 파싱합니다. //키워드, 변수명, 타입명 등
		{
			switch (c)
			{
			case L'a': case L'b': case L'c': case L'd': case L'e': case L'f': case L'g':
			case L'h': case L'i': case L'j': case L'k': case L'l': case L'm': case L'n':
			case L'o': case L'p': case L'q': case L'r': case L's': case L't': case L'u':
			case L'v': case L'w': case L'x': case L'y': case L'z':
			case L'A': case L'B': case L'C': case L'D': case L'E': case L'F': case L'G':
			case L'H': case L'I': case L'J': case L'K': case L'L': case L'M': case L'N':
			case L'O': case L'P': case L'Q': case L'R': case L'S': case L'T': case L'U':
			case L'V': case L'W': case L'X': case L'Y': case L'Z':
			case L'_':
			case L'0': case L'1': case L'2': case L'3': case L'4': case L'5': case L'6':
			case L'7': case L'8': case L'9':
				word += c;
				break;

			default: //문자/숫자가 아닌게 나오면 식별자로 분리
				tokens.push(std::move(word));
				flag = StateFlag::DEFAULT; //다시 진입점으로
				continue;
			}
		}
			break;

		case StateFlag::NUMBER:
		{
			//TODO : 읽어나가면서 .이 있으면 float로, 없으면 integer로 위임
		}
			break;

		case StateFlag::INTEGER:
		{

		}
			break;

		case StateFlag::FLOAT:
		{

		}
			break;

		case StateFlag::LINE_COMMENT: //주석문 처리중
		{
			i = line.length(); //루프 탈출
		} break;

		case StateFlag::STRING: //문자열 리터럴 처리중
		{
			if (c == '\\')
				if (in_escape == false)
				{
					in_escape = true;
					word += c;
					continue;
				}

				else if (c == '\"'&&in_escape == false)
				{
					word += L"\")";
					tokens.push(std::move(word));
					flag = StateFlag::DEFAULT;
					continue;
				}

			if (in_escape == true)
				in_escape = false;

			word += c;

		} break;

		case StateFlag::CHAR: //문자 리터럴 처리중
		{
			if (c == '\\')
				if (in_escape == false)
				{
					in_escape = true;
					word += c;
					continue;
				}

				else if (c == '\"'&&in_escape == false)
				{
					word += L"\')";
					tokens.push(std::move(word));
					flag = StateFlag::DEFAULT;
					continue;
				}

			if (in_escape == true)
				in_escape = false;

			word += c;
		} break;

		case StateFlag::BLOCK_COMMENT: //미구현
			break;

		case StateFlag::BLOCK_STRING: //미구현
			break;
	
		}
	}

	if (!word.empty())
		tokens.push(std::move(word));

	tokens.emplace(1, '\n');

	return tokens;
}
