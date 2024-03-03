#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

#include "token_type.hpp"

/// We use `void *` for m_literal so that we can store different types of Tokens
/// in a vector
class Token {
  TokenType m_type;
  std::string_view m_lexeme;
  void *m_literal;
  unsigned long m_line;

public:
  Token(
      TokenType const type,
      std::string_view const lexeme,
      void *const literal,
      unsigned long const line)
      : m_type(type),
        m_lexeme(lexeme),
        m_literal(literal),
        m_line(line) {}

  std::string to_string() const {
    return tt_to_string(m_type).append(" ").append(m_lexeme).append(" ");  // TODO: add m_literal to the string
  }
};

#endif // TOKEN_HPP
