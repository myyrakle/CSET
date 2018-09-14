#pragma once

#include <exception>

class syntax_error : public std::exception
{
};

class logic_error : public std::exception
{
};