#include "token.hpp"
#include <iostream>
#include <memory>

class Expr {
public:
  virtual std::string to_string(std::size_t indent = 0) const = 0;
  virtual ~Expr() {};
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

  ~Binary() {
    std::cout << "~Binary()\n";
  }

  std::string to_string(std::size_t indent = 0) const override {
    return std::string(indent * 4, ' ') + m_oper.literal_to_string() + '\n'
           + std::string(indent * 4, ' ') + m_left->to_string(indent + 1) + '\n'
           + std::string(indent * 4, ' ') + m_right->to_string(indent + 1)
           + '\n';
  }
};

class Grouping : public Expr {
  std::unique_ptr<Expr> m_expr;

public:
  Grouping(Expr *expr) : m_expr{expr} {}

  ~Grouping() {
    std::cout << "~Grouping()\n";
  }

  std::string to_string(std::size_t indent = 0) const override {
    return std::string(indent * 4, ' ') + std::string("group") + '\n'
           + std::string(indent * 4, ' ') + m_expr->to_string(indent + 1)
           + '\n';
  }
};

class StringLiteral : public Expr {
private:
  std::unique_ptr<std::string> m_str;

public:
  StringLiteral(std::string *str) : m_str{str} {}

  ~StringLiteral() {
    std::cout << "~StringLiteral()\n";
  }

  std::string
  to_string([[maybe_unused]] std::size_t indent = 0) const override {
    return std::string(indent * 4, ' ') + *m_str;
  }
};

class NumberLiteral : public Expr {
private:
  std::unique_ptr<double> m_number;

public:
  NumberLiteral(double *number) : m_number{number} {}

  ~NumberLiteral() {
    std::cout << "~NumberLiteral()\n";
  }

  std::string
  to_string([[maybe_unused]] std::size_t indent = 0) const override {
    return std::string(indent * 4, ' ') + std::to_string(*m_number);
  }
};

class Unary : public Expr {
private:
  Token m_oper;
  std::unique_ptr<Expr> m_right;

public:
  Unary(Token oper, Expr *right) : m_oper{oper}, m_right{right} {}

  ~Unary() {
    std::cout << "~Unary()\n";
  }

  std::string
  to_string([[maybe_unused]] std::size_t indent = 0) const override {
    return std::string(indent * 4, ' ') + m_oper.literal_to_string() + '\n'
           + std::string(indent * 4, ' ') + m_right->to_string(indent + 1);
  }
};
