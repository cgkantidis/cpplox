#include <iomanip>
#include <iostream>
#include <string_view>

#include "error_message.hpp"

void report(
    std::size_t line,
    std::string_view const where,
    std::string_view const message) {
  std::cerr << "Error at line: " << line << ": " << message << ": "
            << std::quoted(where) << '\n';
}

void error(std::size_t line, std::string_view const message) {
  report(line, "", message);
}
