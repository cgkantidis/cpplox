#ifndef EXPR_HPP
#define EXPR_HPP

#include <fmt/core.h>
#include <iostream>
#include <memory>

#include "token.hpp"

class Expr {
public:
  virtual ~Expr() = default;

  [[nodiscard]] virtual std::string to_string() const = 0;
};

class Binary : public Expr {
private:
  std::unique_ptr<Expr> m_left;
  Token m_oper;
  std::unique_ptr<Expr> m_right;

public:
  Binary(Expr *left, Token oper, Expr *right)
      : m_left{left},
        m_oper{oper},
        m_right{right} {}

  ~Binary() override {
    std::cout << "~Binary()\n";
  }

  [[nodiscard]] std::string to_string() const override {
    return fmt::format(
        "({} {} {})",
        m_oper.literal_to_string(),
        m_left->to_string(),
        m_right->to_string());
  }
};

class Grouping : public Expr {
  std::unique_ptr<Expr> m_expr;

public:
  explicit Grouping(Expr *expr) : m_expr{expr} {}

  ~Grouping() override {
    std::cout << "~Grouping()\n";
  }

  [[nodiscard]] std::string to_string() const override {
    return fmt::format("(group {})", m_expr->to_string());
  }
};

class Unary : public Expr {
private:
  Token m_oper;
  std::unique_ptr<Expr> m_expr;

public:
  Unary(Token oper, Expr *expr) : m_oper{oper}, m_expr{expr} {}

  ~Unary() override {
    std::cout << "~Unary()\n";
  }

  [[nodiscard]] std::string to_string() const override {
    return fmt::format(
        "({} {})",
        m_oper.literal_to_string(),
        m_expr->to_string());
  }
};

class StringLiteral : public Expr {
private:
  std::string m_str;

public:
  explicit StringLiteral(std::string_view str) : m_str{str} {}

  ~StringLiteral() override {
    std::cout << "~StringLiteral()\n";
  }

  [[nodiscard]] std::string to_string() const override {
    return fmt::format("\"{}\"", m_str);
  }
};

class NumericLiteral : public Expr {
private:
  double m_number;

public:
  explicit NumericLiteral(double number) : m_number{number} {}

  ~NumericLiteral() override {
    std::cout << "~NumericLiteral()\n";
  }

  [[nodiscard]] std::string to_string() const override {
    return fmt::format("{}", m_number);
  }
};

class BoolLiteral : public Expr {
private:
  bool m_val;

public:
  explicit BoolLiteral(bool val) : m_val{val} {}

  ~BoolLiteral() override {
    fmt::println("~BoolLiteral()");
  }

  [[nodiscard]] std::string to_string() const override {
    return fmt::format("{}", m_val);
  }
};

class NilLiteral : public Expr {
public:
  NilLiteral() = default;

  ~NilLiteral() override {
    fmt::println("~NilLiteral()");
  }

  [[nodiscard]] std::string to_string() const override {
    return fmt::format("nil");
  }
};

#endif //EXPR_HPP
