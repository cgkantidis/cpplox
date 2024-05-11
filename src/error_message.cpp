#include <fmt/core.h>
#include <string_view>

#include "error_message.hpp"

void report(
    std::size_t line,
    std::string_view const message,
    std::string_view const where) {
  fmt::println(stderr, "Error at line: {}: {}: {}", line, message, where);
}

void error(std::size_t line, std::string_view const message) {
  report(line, "", message);
}

void error(Token token, std::string_view message) {
  if (token.type() == TokenType::END_OF_FILE) {
    report(token.line(), message, " at end");
  } else {
    report(token.line(), message, fmt::format(" at \"{}\"", token.lexeme()));
  }
}
