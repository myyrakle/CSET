#pragma once

namespace keywords
{
	//types
	constexpr auto OBJECT = L"object";
	constexpr auto CHAR = L"char";
	constexpr auto STRING = L"string";
	constexpr auto BOOL = L"bool";
	constexpr auto BYTE = L"byte";
	constexpr auto INT = L"int";
	constexpr auto UINT = L"uint";
	constexpr auto INT8 = L"int8";
	constexpr auto INT16 = L"int16";
	constexpr auto INT32 = L"int32";
	constexpr auto INT64 = L"int64";
	constexpr auto UINT8 = L"uint8";
	constexpr auto UINT16 = L"uint16";
	constexpr auto UINT32 = L"uint32";
	constexpr auto UINT64 = L"uint64";
	constexpr auto FLOAT = L"float";
	constexpr auto DOUBLE = L"double";
	//types

	//상수
	constexpr auto null = nullptr;
	constexpr auto TRUE_ = L"true";
	constexpr auto FALSE_ = L"false";
	//상수

	constexpr auto STATIC = L"static";

	constexpr auto SIZEOF = L"sizeof";

	//분기
	constexpr auto IF = L"if";
	constexpr auto ELSE = L"else";
	constexpr auto SWITCH = L"switch";
	constexpr auto CASE = L"case";
	//분기

	//반복
	constexpr auto FOR = L"for";
	constexpr auto RFOR = L"rfor";
	constexpr auto IN_ = L"in";
	constexpr auto REVERSE = L"reverse";
	constexpr auto WHILE = L"while";
	//반복

	//제어
	constexpr auto BREAK = L"break";
	constexpr auto CONTINUE = L"continue";
	constexpr auto LABEL = L"label";
	constexpr auto RETURN = L"return";
	//제어

	//선언
	constexpr auto FUNC = L"func";
	constexpr auto CLASS = L"class";

	//변수
	constexpr auto CONST_ = L"const";
	constexpr auto MUT = L"mut";
	constexpr auto LITERAL = L"literal";
	//변수

	//동적 할당
	constexpr auto NEW = L"new";
	constexpr auto REFC = L"refc";
	//동적 할당

	//네임스페이스
	constexpr auto USING = L"using";
	constexpr auto NAMESPACE = L"namespace";
	constexpr auto ALIAS = L"alias";

	constexpr auto IMPORT = L"import";

	namespace unuse
	{
		constexpr auto STRUCT = L"struct";
		constexpr auto UNION = L"union";
		constexpr auto VIRTUAL = L"virtual";
		constexpr auto GOTO = L"goto";
		constexpr auto AUTO = L"auto";
		constexpr auto DELETE_ = L"delete";
		constexpr auto CONSTEXPR = L"constexpr";
		constexpr auto SHORT = L"short";
		constexpr auto WCHAR_T = L"wchar_t";
		constexpr auto LONG = L"long";
		constexpr auto SIGNED = L"signed";
		constexpr auto UNSIGNED = L"unsigned";
		constexpr auto FRIEND = L"friend";
		constexpr auto INLINE = L"inline";
		constexpr auto TYPENAME = L"typename";
		constexpr auto ASM = L"asm";
		constexpr auto REGISTER = L"register";
		constexpr auto NOEXCEPT = L"noexcept";
	};
};