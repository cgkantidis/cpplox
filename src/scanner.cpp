#include "scanner.hpp"
#include "error_message.hpp"
#include "token_type.hpp"

#include <exception>

std::vector<Token> &Scanner::scan_tokens() {
  while (!is_at_end()) {
    m_token_start_idx = m_current_idx;
    scan_token();
  }

  m_tokens.emplace_back(TokenType::END_OF_FILE, "", nullptr, m_current_line);
  return m_tokens;
}

void Scanner::scan_token() {
  switch (advance()) {
  case '(': {
    add_token(TokenType::LEFT_PAREN);
    break;
  }
  case ')': {
    add_token(TokenType::RIGHT_PAREN);
    break;
  }
  case '{': {
    add_token(TokenType::LEFT_BRACE);
    break;
  }
  case '}': {
    add_token(TokenType::RIGHT_BRACE);
    break;
  }
  case ',': {
    add_token(TokenType::COMMA);
    break;
  }
  case '.': {
    add_token(TokenType::DOT);
    break;
  }
  case '-': {
    add_token(TokenType::MINUS);
    break;
  }
  case '+': {
    add_token(TokenType::PLUS);
    break;
  }
  case ';': {
    add_token(TokenType::SEMICOLON);
    break;
  }
  case '*': {
    add_token(TokenType::STAR);
    break;
  }
  case '!': {
    add_token(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
    break;
  }
  case '=': {
    add_token(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
    break;
  }
  case '<': {
    add_token(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
    break;
  }
  case '>': {
    add_token(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
    break;
  }
  case '/': {
    if (match('/')) {
      while (peek() != '\n' && !is_at_end()) {
        advance();
      }
    } else {
      add_token(TokenType::SLASH);
    }
    break;
  }
  case ' ':
  case '\r':
  case '\t': {
    break;
  }
  case '\n': {
    ++m_current_line;
    break;
  }
  default: {
    m_had_error = true;
    error(m_current_line, "Unexpected character.");
  }
  }
}
