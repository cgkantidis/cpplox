#include "scanner.hpp"
#include "error_message.hpp"
#include "token_type.hpp"

#include <charconv>
#include <unordered_map>

std::vector<Token> Scanner::scan_tokens() {
  while (!is_at_end()) {
    m_token_start_idx = m_current_idx;
    scan_token();
  }

  m_tokens.emplace_back(TokenType::END_OF_FILE, "", nullptr, m_current_line);
  return std::move(m_tokens);
}

void Scanner::add_string_token() {
  std::size_t lines_to_advance = 0;
  while (peek() != '"' && !is_at_end()) {
    // we allow string literals spanning multiple lines
    if (peek() == '\n') {
      ++lines_to_advance;
    }
    advance();
  }

  if (is_at_end()) {
    error(m_current_line, "Unterminated string.");
    return;
  }

  // the closing "
  advance();

  auto *str = new std::string(
      m_source.data() + m_token_start_idx + 1,
      m_current_idx - m_token_start_idx - 2);
  add_token(TokenType::STRING, str);
  m_current_line += lines_to_advance;
}

void Scanner::add_number_token() {
  while (isdigit(peek())) {
    advance();
  }

  // look for a fractional part
  if (peek() == '.' && isdigit(peek_next())) {
    // consume the '.'
    advance();

    while (std::isdigit(peek())) {
      advance();
    }
  }

  auto *value = new double();
  std::from_chars(
      m_source.data() + m_token_start_idx,
      m_source.data() + m_current_idx,
      *value);
  add_token(TokenType::NUMBER, value);
}

constexpr bool isdigit(char const ch) {
  return ch >= '0' && ch <= '9';
}

constexpr bool isalpha(char const ch) {
  return ch == '_' || (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
}

constexpr bool isalnum(char const ch) {
  return isdigit(ch) || isalpha(ch);
}

void Scanner::add_identifier_token() {
  static const std::unordered_map<std::string_view, TokenType> keywords(
      {{"and", TokenType::AND},
       {"class", TokenType::CLASS},
       {"else", TokenType::ELSE},
       {"false", TokenType::FALSE},
       {"for", TokenType::FOR},
       {"fun", TokenType::FUN},
       {"if", TokenType::IF},
       {"nil", TokenType::NIL},
       {"or", TokenType::OR},
       {"print", TokenType::PRINT},
       {"return", TokenType::RETURN},
       {"super", TokenType::SUPER},
       {"this", TokenType::THIS},
       {"true", TokenType::TRUE},
       {"var", TokenType::VAR},
       {"while", TokenType::WHILE}});

  while (isalnum(peek())) {
    advance();
  }

  std::string_view text(
      m_source.data() + m_token_start_idx,
      m_source.data() + m_current_idx);
  if (auto it = keywords.find(text); it != keywords.end()) {
    add_token(it->second);
  } else {
    add_token(TokenType::IDENTIFIER, new std::string(text));
  }
}

void Scanner::scan_token() {
  char ch = advance();
  if (ch == ' ' || ch == '\r' || ch == '\t') {
    return;
  }
  if (ch == '\n') {
    ++m_current_line;
    return;
  }
  if (ch == '(') {
    add_token(TokenType::LEFT_PAREN);
    return;
  }
  if (ch == ')') {
    add_token(TokenType::RIGHT_PAREN);
    return;
  }
  if (ch == '{') {
    add_token(TokenType::LEFT_BRACE);
    return;
  }
  if (ch == '}') {
    add_token(TokenType::RIGHT_BRACE);
    return;
  }
  if (ch == ',') {
    add_token(TokenType::COMMA);
    return;
  }
  if (ch == '.') {
    add_token(TokenType::DOT);
    return;
  }
  if (ch == '-') {
    add_token(TokenType::MINUS);
    return;
  }
  if (ch == '+') {
    add_token(TokenType::PLUS);
    return;
  }
  if (ch == ';') {
    add_token(TokenType::SEMICOLON);
    return;
  }
  if (ch == '*') {
    add_token(TokenType::STAR);
    return;
  }
  if (ch == '!') {
    add_token(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
    return;
  }
  if (ch == '=') {
    add_token(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
    return;
  }
  if (ch == '<') {
    add_token(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
    return;
  }
  if (ch == '>') {
    add_token(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
    return;
  }
  if (ch == '/') {
    if (match('/')) {
      while (peek() != '\n' && !is_at_end()) {
        advance();
      }
    } else {
      add_token(TokenType::SLASH);
    }
    return;
  }
  if (ch == '"') {
    add_string_token();
    return;
  }
  if (isdigit(ch)) {
    add_number_token();
    return;
  }
  if (isalpha(ch)) {
    add_identifier_token();
    return;
  }

  m_had_error = true;
  error(m_current_line, "Unexpected character.");
}
