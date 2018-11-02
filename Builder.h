#pragma once
//변환된 C++코드를 받아서 g++로 컴파일을 수행합니다.

class Builder
{
public:
	void build(); //본격적인 작업을 수행합니다.
public:
	//void add_header(wstring&& filename);
	void add_file(wstring&& filename);
	void compile() const;

public:
	void set_original_filepath(wstring&&);
private:
	wstring _original_filepath;

private:
	vector<wstring> filenames;
	//array<wstring> headers;
	//vector<CompileOption> options

public: //구현하지 않음
	Builder() = default;
	~Builder() = default;
	Builder(const Builder&) = delete;
	Builder(Builder&&) = delete;
	Builder& operator=(const Builder&) = delete;
	Builder& operator=(Builder&&) = delete;
};