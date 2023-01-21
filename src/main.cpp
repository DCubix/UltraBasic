#include <iostream>

#include "parser.h"

int main(int argc, char** argv) {

  std::string input = "4 + 5 * (2 + 2) / 5";
  ulang::Lexer lex{input};

  auto tokens = lex.run();
  for (const auto& tok : tokens) {
    std::cout << tok << " ";
  }
  std::cout << std::endl;

  ulang::Parser parser(tokens);
  parser.parse();

  return 0;
}
