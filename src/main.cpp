#include <iostream>

#include "parser.h"
#include "vm.h"

int main(int argc, char** argv) {

  std::string input = "2 >= 10";
  ulang::Lexer lex{input};

  auto tokens = lex.run();
  for (const auto& tok : tokens) {
    std::cout << tok << " ";
  }
  std::cout << std::endl;

  ulang::Parser parser{tokens};
  auto prog = parser.parse();

  ulang::VirtualMachine vm{prog};
  vm.run();
  vm.dumpStack();

  return 0;
}
