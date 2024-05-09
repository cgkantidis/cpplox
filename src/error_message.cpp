#include <iostream>

#include "error_message.hpp"

void report(std::size_t line, char const *where, char const *message) {
  std::cerr << "Error at line: " << line << ": " << where << '\n'
            << message << '\n';
}

void error(std::size_t line, char const *message) {
  report(line, "", message);
}
