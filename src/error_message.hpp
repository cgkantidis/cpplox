#ifndef ERROR_MESSAGE_HPP
#define ERROR_MESSAGE_HPP

#include <cstddef>
#include <string_view>

void report(
    std::size_t line,
    std::string_view const where,
    std::string_view const message);
void error(std::size_t line, std::string_view const message);

#endif // ERROR_MESSAGE_HPP
