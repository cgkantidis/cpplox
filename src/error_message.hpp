#ifndef ERROR_MESSAGE_HPP
#define ERROR_MESSAGE_HPP

#include <cstddef>

void report(std::size_t line, char const *where, char const *message);
void error(std::size_t line, char const *message);

#endif // ERROR_MESSAGE_HPP
