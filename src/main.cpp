#include <iostream>

#include "lox.hpp"

int main(int const argc, char const *const argv[]) {
  if (argc > 2) {
    std::cout << "Usage: " << argv[0] << " [script]\n";
    return 64;
  }

  Lox lox;
  if (argc == 1) {
    return lox.run_prompt();
  }

  return lox.run_file(argv[1]);
}

