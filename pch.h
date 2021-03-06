#pragma once

#include <iostream>
using std::cout;
using std::wcout;
using std::cin;
using std::wcin;
using std::endl;


#include <fstream>
//using std::fstream;
//using std::wfstream;
//using std::ifstream;
using std::wifstream;
//using std::ofstream;
using std::wofstream;

#include <string>
using std::string;
using std::wstring;

#include <string_view>
using std::wstring_view;

#include <clocale>

/*
#include <regex>
using std::regex;
using std::wregex;
*/

#include <filesystem>
namespace fs = std::filesystem;

#include <stdexcept>
using std::exception;

#include <utility>

#include <vector>
using std::vector;
#include <queue>
using std::queue;
#include <array>
using std::array;

#include <cwchar>

#include <limits>

#include <windows.h>

std::wstring current_filepath();

#include "Keywords.h"
#include "Constants.h"