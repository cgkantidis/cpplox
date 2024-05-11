#ifndef PARSER_HPP
#define PARSER_HPP

#include <algorithm>
#include <vector>

#include "error_message.hpp"
#include "expr.hpp"
#include "token.hpp"

// Lox grammar
// expression     → equality ;
// equality       → comparison ( ( "!=" | "==" ) comparison )* ;
// comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
// term           → factor ( ( "-" | "+" ) factor )* ;
// factor         → unary ( ( "/" | "*" ) unary )* ;
// unary          → ( "!" | "-" ) unary
//                | primary ;
// primary        → NUMBER | STRING | "true" | "false" | "nil"
//                | "(" expression ")" ;

class ParseError : public std::exception {
public:
  [[nodiscard]] char const *what() const noexcept override {
    return "ParseError";
  }
};

ParseError parse_error(Token token, std::string_view message);

class Parser {
private:
  std::vector<Token> const &m_tokens;
  std::size_t m_current_idx{};

public:
  explicit Parser(std::vector<Token> const &tokens) : m_tokens{tokens} {}

private:
  // non-consumers
  [[nodiscard]] Token peek() const {
    return m_tokens[m_current_idx];
  }
  [[nodiscard]] Token previous() const {
    return m_tokens[m_current_idx - 1];
  }
  [[nodiscard]] bool is_at_end() const {
    return peek().type() == TokenType::END_OF_FILE;
  }
  [[nodiscard]] bool check(TokenType type) const {
    return !is_at_end() && peek().type() == type;
  }

  // consumers
  Token advance() {
    if (!is_at_end()) {
      ++m_current_idx;
    }
    return previous();
  }

  bool match(TokenType type) {
    if (check(type)) {
      advance();
      return true;
    }
    return false;
  }

  bool match(std::initializer_list<TokenType> types) {
    if (std::ranges::any_of(types, [this](TokenType type) {
          return check(type);
        })) {
      advance();
      return true;
    }
    return false;
  }

  Token consume(TokenType type, std::string_view message) {
    if (check(type)) {
      return advance();
    }

    throw parse_error(peek(), message);
  }

  void synchronize() {
    advance();

    while (!is_at_end()) {
      if (previous().type() == TokenType::SEMICOLON) {
        return;
      }

      switch (peek().type()) {
      case TokenType::CLASS:
      case TokenType::FUN:
      case TokenType::VAR:
      case TokenType::FOR:
      case TokenType::IF:
      case TokenType::WHILE:
      case TokenType::PRINT:
      case TokenType::RETURN:
        return;
      default:
        advance();
      }
    }
  }

public:
  std::unique_ptr<Expr> parse() {
    try {
      return expression();
    } catch (ParseError &error) {
      fmt::println("{}", error.what());
      return {};
    }
  }

  std::unique_ptr<Expr> expression() {
    return equality();
  }

  std::unique_ptr<Expr> equality() {
    auto expr = comparison();

    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
      auto op = previous();
      auto right = comparison();
      expr = std::make_unique<Binary>(expr.release(), op, right.release());
    }

    return expr;
  }

  std::unique_ptr<Expr> comparison() {
    auto expr = term();

    while (match(
        {TokenType::GREATER,
         TokenType::GREATER_EQUAL,
         TokenType::LESS,
         TokenType::LESS_EQUAL})) {
      auto op = previous();
      auto right = term();
      expr = std::make_unique<Binary>(expr.release(), op, right.release());
    }

    return expr;
  }

  std::unique_ptr<Expr> term() {
    auto expr = factor();

    while (match({TokenType::MINUS, TokenType::PLUS})) {
      auto op = previous();
      auto right = factor();
      expr = std::make_unique<Binary>(expr.release(), op, right.release());
    }

    return expr;
  }

  std::unique_ptr<Expr> factor() {
    auto expr = unary();

    while (match({TokenType::SLASH, TokenType::STAR})) {
      auto op = previous();
      auto right = unary();
      expr = std::make_unique<Binary>(expr.release(), op, right.release());
    }

    return expr;
  }

  std::unique_ptr<Expr> unary() {
    if (match({TokenType::BANG, TokenType::MINUS})) {
      auto op = previous();
      auto right = unary();
      return std::make_unique<Unary>(op, right.release());
    }

    return primary();
  }

  std::unique_ptr<Expr> primary() {
    if (match(TokenType::FALSE)) {
      return std::make_unique<BoolLiteral>(false);
    }
    if (match(TokenType::TRUE)) {
      return std::make_unique<BoolLiteral>(true);
    }
    if (match(TokenType::NIL)) {
      return std::make_unique<NilLiteral>();
    }
    if (match(TokenType::NUMBER)) {
      return std::make_unique<NumericLiteral>(*previous().literal<double>());
    }
    if (match(TokenType::STRING)) {
      return std::make_unique<StringLiteral>(
          *previous().literal<std::string>());
    }
    if (match(TokenType::LEFT_PAREN)) {
      auto expr = expression();
      consume(TokenType::RIGHT_PAREN, "Exprected ')' after expression");
      return std::make_unique<Grouping>(expr.release());
    }

    throw parse_error(peek(), "Expected expression");
  }
};

#endif
