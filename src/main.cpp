#include <iostream>  // cerr
#include <sysexits.h>  // EX_USAGE

#include "expr.hpp"
#include "lox.hpp"

int main(int argc, char const *const *argv) {
  if (argc > 2) {
    std::cerr << "Usage: " << argv[0] << " [script]\n";
    return EX_USAGE;
  }

  Lox lox;
  if (argc == 1) {
    return lox.run_prompt();
  }

  return lox.run_file(argv[1]);
}
