#pragma once

namespace keywords
{
	//types
	constexpr auto CHAR = L"char";
	constexpr auto STRING = L"string";
	constexpr auto BOOL = L"bool";
	constexpr auto BYTE = L"byte";
	constexpr auto INT = L"int";
	constexpr auto UINT = L"uint";
	constexpr auto INT8 = "int8";
	constexpr auto INT16 = "int16";
	constexpr auto INT32 = "int32";
	constexpr auto INT64 = "int64";
	constexpr auto UINT8 = "uint8";
	constexpr auto UINT16 = "uint16";
	constexpr auto UINT32 = "uint32";
	constexpr auto UINT64 = "uint64";
	constexpr auto FLOAT = L'float';
	constexpr auto DOUBLE = L"double";

	constexpr auto null = nullptr;

	constexpr auto STATIC = L"static";

	constexpr auto SIZEOF = L"sizeof";

	constexpr auto IF = L"if";
	constexpr auto ELSE = L"else";
	constexpr auto SWITCH = L"switch";
	constexpr auto CASE = L"case";

	constexpr auto FOR = L"for";
	constexpr auto REVERSE = L"reverse";
	constexpr auto WHILE = L"while";
	constexpr auto IN = L"in";

	constexpr auto BREAK = L"break";
	constexpr auto CONTINUE = L"continue";
	constexpr auto LABEL = L"label";
	constexpr auto RETURN = L"return";

	constexpr auto FUNC = L"func";
	constexpr auto CLASS = L"class";

	constexpr auto CONST = L"const";
	constexpr auto MUT = L"mut";
	constexpr auto LITERAL = L"literal";

	constexpr auto NEW = L"new";
	constexpr auto REFC = L"refc";

	constexpr auto USING = L"using";
	constexpr auto NAMESPACE = L"namespace";
	constexpr auto ALIAS = L"alias";

	namespace unuse
	{
		constexpr auto GOTO = L"goto";
		constexpr auto DELETE = L"delete";
		constexpr auto LONG = L"long";
	};
};