#include <iostream>

#include "lexer.h"

int main(int argc, char** argv) {

  std::string input = "4 + 5 * (2 + 2) / -1";
  ulang::Lexer lex{input};

  for (const auto& tok : lex.run()) {
    std::cout << tok << " ";
  }
  std::cout << std::endl;

  return 0;
}
