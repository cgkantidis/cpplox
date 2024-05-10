#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <vector>

#include "token.hpp"

/// The scanner scans the source code, separates it into lexemes, and turns the
/// lexemes into tokens.
///
/// The lexemes are only the raw substrings of the source code. However, in the
/// process of grouping character sequences into lexemes, we also stumble upon
/// some other useful information. When we take the lexeme and bundle it
/// together with that other data, the result is a token
class Scanner {
private:
  std::string_view m_source;
  std::size_t m_current_line{1}; // current line in m_source
  std::size_t m_start_idx{}; // index in m_source where the current lexeme begun
  std::size_t m_current_idx{}; // current index in m_source
  std::vector<Token> m_tokens;
  bool m_had_error{false};

public:
  explicit Scanner(char const *source) : m_source(source) {}
  std::vector<Token> scan_tokens();
  [[nodiscard]] bool had_error() const {
    return m_had_error;
  }

private:
  [[nodiscard]] bool is_at_end() const {
    return m_current_idx >= m_source.size();
  }

  /// Consume the next character and return it
  char advance() {
    return m_source[m_current_idx++];
  }

  /// match() is like a conditional advance()
  /// If the next character is the expected one consume it and return true;
  /// otherwise return false without consuming it
  bool match(char expected) {
    if (is_at_end() || m_source[m_current_idx] != expected) {
      return false;
    }

    ++m_current_idx;
    return true;
  }

  /// Return the next character without consuming it (lookahead)
  [[nodiscard]] char peek() const {
    if (m_current_idx >= m_source.size()) {
      return '\0';
    }
    return m_source[m_current_idx];
  }

  [[nodiscard]] char peek_next() const {
    if (m_current_idx + 1 >= m_source.size()) {
      return '\0';
    }
    return m_source[m_current_idx + 1];
  }

  void add_token(TokenType type) {
    add_token(type, nullptr);
  }

  void add_token(TokenType type, void *literal) {
    m_tokens.emplace_back(
        type,
        m_source.substr(m_start_idx, m_current_idx - m_start_idx),
        literal,
        m_current_line);
  }

  void add_string_token();
  void add_number_token();
  void add_identifier_token();
  void scan_token();
};

#endif // SCANNER_HPP
