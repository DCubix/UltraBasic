#pragma once

#include "lexer.h"

namespace ulang {

  class Parser {
  public:
    Parser() = default;
    Parser(const std::vector<Token>& input) : m_tokens(input) {}

    

  private:
    size_t m_pos{ 0 };
    std::vector<Token> m_tokens;
  };

} // namespace ulang
