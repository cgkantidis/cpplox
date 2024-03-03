#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <vector>

#include "token.hpp"

class Scanner {
private:
  std::string_view m_source{};
  std::size_t m_current_line{1};
  std::size_t m_current_idx{};
  std::size_t m_token_start_idx{};
  std::vector<Token> m_tokens;
  bool m_had_error;

public:
  Scanner(char const *source) : m_source(source) {}
  std::vector<Token> scan_tokens();
  bool had_error() const {
    return m_had_error;
  }

private:
  bool is_at_end() const {
    return m_current_idx == m_source.size();
  }

  char advance() {
    return m_source[m_current_idx++];
  }

  void add_token(TokenType type) {
    add_token(type, nullptr);
  }

  void add_token(TokenType type, void *literal) {
    m_tokens.emplace_back(
        type,
        m_source.substr(m_token_start_idx, m_current_idx - m_token_start_idx),
        literal,
        m_current_line);
  }

  char peek() const {
    return m_source[m_current_idx];
  }

  bool match(char expected) {
    if (is_at_end() || m_source[m_current_idx] != expected) {
      return false;
    }

    ++m_current_idx;
    return true;
  }

  void add_string_token();
  void scan_token();
};

#endif // SCANNER_HPP
