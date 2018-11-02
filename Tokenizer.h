#pragma once
class Tokenizer
{
public:
	std::queue<std::wstring> tokenize(const std::wstring&); //본격적인 작업을 수행합니다.
public: //기본 생성/대입
	Tokenizer() = default;
	virtual ~Tokenizer() = default;
	Tokenizer(const Tokenizer&) = delete;
	Tokenizer& operator=(const Tokenizer&) = delete;
	Tokenizer(Tokenizer&&) = delete;
	Tokenizer& operator=(Tokenizer&&) = delete;
private:
	bool in_block_comment = false;
	bool in_block_string = false;
};