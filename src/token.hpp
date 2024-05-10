#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <fmt/core.h>
#include <stdexcept>
#include <string>

#include "token_type.hpp"

/// We use `void *` for m_literal so that we can store different types of Tokens
/// in a vector
/// TODO: consider using a class hierarchy for the different Token types
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

  // we don't use a destructor so that if Token is stored in a std::vector and
  // the vector is resized, the move operator can treat the Token a trivially
  // copyable. This means that the owner of each token must call free_token of
  // it. (pesimization?)
  void free_token() const {
    switch (m_type) {
    case TokenType::STRING: {
      delete static_cast<std::string *>(m_literal);
      break;
    }
    case TokenType::NUMBER: {
      delete static_cast<double *>(m_literal);
      break;
    }
    case TokenType::IDENTIFIER: {
      delete static_cast<std::string *>(m_literal);
      break;
    }
    default: {
      break;
    }
    }
  }

  [[nodiscard]] std::string to_string() const {
    return std::to_string(m_line)
        .append(": ")
        .append(tt_to_string(m_type))
        .append(" ")
        .append(m_lexeme)
        .append(" ")
        .append(literal_to_string());
  }

  [[nodiscard]] std::string literal_to_string() const {
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
    case TokenType::IDENTIFIER: {
      return *static_cast<std::string *>(m_literal);
    }
    case TokenType::NUMBER: {
      return fmt::format("{}", *static_cast<double *>(m_literal));
    }
    case TokenType::STRING: {
      return *static_cast<std::string *>(m_literal);
    }
    case TokenType::AND: {
      return "and";
    }
    case TokenType::CLASS: {
      return "class";
    }
    case TokenType::ELSE: {
      return "else";
    }
    case TokenType::FALSE: {
      return "false";
    }
    case TokenType::FUN: {
      return "fun";
    }
    case TokenType::FOR: {
      return "for";
    }
    case TokenType::IF: {
      return "if";
    }
    case TokenType::NIL: {
      return "nil";
    }
    case TokenType::OR: {
      return "or";
    }
    case TokenType::PRINT: {
      return "print";
    }
    case TokenType::RETURN: {
      return "return";
    }
    case TokenType::SUPER: {
      return "super";
    }
    case TokenType::THIS: {
      return "this";
    }
    case TokenType::TRUE: {
      return "true";
    }
    case TokenType::VAR: {
      return "var";
    }
    case TokenType::WHILE: {
      return "while";
    }
    }
    throw std::runtime_error("Unexpected token type");
  }
};

#endif // TOKEN_HPP
