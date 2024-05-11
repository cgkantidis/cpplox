#ifndef ERROR_MESSAGE_HPP
#define ERROR_MESSAGE_HPP

#include "token.hpp"
#include <cstddef>
#include <string_view>

void report(
    std::size_t line,
    std::string_view const message,
    std::string_view const where);
void error(std::size_t line, std::string_view const message);
void error(Token token, std::string_view message);

#endif // ERROR_MESSAGE_HPP
