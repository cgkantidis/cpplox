#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <exception>
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
  Token(Token const &) = delete;
  Token(Token &&) = default;
  Token &operator=(Token const &) = delete;
  Token &operator=(Token &&) = default;
  ~Token() {
    switch (m_type) {
    case TokenType::STRING: {
      delete static_cast<std::string *>(m_literal);
      break;
    }
    default: {
      break;
    }
    }
  }

  std::string to_string() const {
    return std::to_string(m_line)
        .append(": ")
        .append(tt_to_string(m_type))
        .append(" ")
        .append(m_lexeme)
        .append(" ")
        .append(literal_to_string());
  }

  std::string literal_to_string() const {
    switch (m_type) {
    case TokenType::LEFT_PAREN: {
      return "(";
    }
    case TokenType::RIGHT_PAREN: {
      return ")";
    }
    case TokenType::LEFT_BRACE: {
      return "{";
    }
    case TokenType::RIGHT_BRACE: {
      return "}";
    }
    case TokenType::COMMA: {
      return ",";
    }
    case TokenType::DOT: {
      return ".";
    }
    case TokenType::MINUS: {
      return "-";
    }
    case TokenType::PLUS: {
      return "+";
    }
    case TokenType::SEMICOLON: {
      return ";";
    }
    case TokenType::SLASH: {
      return "/";
    }
    case TokenType::STAR: {
      return "*";
    }
    case TokenType::BANG: {
      return "!";
    }
    case TokenType::BANG_EQUAL: {
      return "!=";
    }
    case TokenType::EQUAL: {
      return "=";
    }
    case TokenType::EQUAL_EQUAL: {
      return "==";
    }
    case TokenType::GREATER: {
      return ">";
    }
    case TokenType::GREATER_EQUAL: {
      return ">=";
    }
    case TokenType::LESS: {
      return "<";
    }
    case TokenType::LESS_EQUAL: {
      return "<=";
    }
    case TokenType::END_OF_FILE: {
      return "EOF";
    }
    case TokenType::STRING: {
      return *static_cast<std::string *>(m_literal);
    }
    case TokenType::IDENTIFIER:
    case TokenType::NUMBER:
    case TokenType::AND:
    case TokenType::CLASS:
    case TokenType::ELSE:
    case TokenType::FALSE:
    case TokenType::FUN:
    case TokenType::FOR:
    case TokenType::IF:
    case TokenType::NIL:
    case TokenType::OR:
    case TokenType::PRINT:
    case TokenType::RETURN:
    case TokenType::SUPER:
    case TokenType::THIS:
    case TokenType::TRUE:
    case TokenType::VAR:
    case TokenType::WHILE:
      std::terminate();
    }
    std::terminate();
  }
};

#endif // TOKEN_HPP
