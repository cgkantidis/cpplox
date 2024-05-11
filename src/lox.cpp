#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <sysexits.h>  // EX_DATAERR
#include <vector>  // std::vector

#include "lox.hpp"
#include "parser.hpp"
#include "scanner.hpp"

/// Read the whole file at script_path into memory and pass its contents to
/// `run()`.
/// In case of error it returns a non-zero value, else it returns zero.
int Lox::run_file(char const *script_path) {
  std::ifstream instream(script_path);
  // read the whole file in memory
  std::stringstream ss;
  ss << instream.rdbuf();
  run(ss.str().c_str());

  if (m_had_error) {
    return EX_DATAERR;
  }
  return 0;
}

/// Read a line from standard input and pass it to the Lox interpreter, until
/// EOF is givern.
/// Returns zero (errors are ignored in interactive mode).
int Lox::run_prompt() {
  std::string input_line;
  while (true) {
    std::cout << "> ";
    std::getline(std::cin, input_line);
    if (!std::cin) {
      std::cout << '\n';
      break;
    }
    run(input_line.c_str());
  }
  return 0;
}

/// Run the Lox interpreter on the `source` code
void Lox::run(char const *source) {
  Scanner scanner(source);
  std::vector<Token> tokens = scanner.scan_tokens();
  m_had_error = scanner.had_error();

  Parser parser(tokens);
  auto expr = parser.parse();
  m_had_error = m_had_error || !expr;

  if (m_had_error) {
    return;
  }

  fmt::println("{}", expr->to_string());
  std::for_each(tokens.begin(), tokens.end(), std::mem_fn(&Token::free_token));
}
