#include "parser.hpp"

ParseError parse_error(Token token, std::string_view message) {
  error(token, message);
  return {};
}

