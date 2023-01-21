#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <ostream>

#include "string_reader.h"

namespace ulang {
enum class TokenType : uint8_t {
  unknown = 0,
  
  identifier,
  
  lParen,
  rParen,

  number,

  plus,
  minus,
  asterisk,
  divide,
  power,

  equals
};

const std::string tokenName[] = {
  "unknown",
  "identifier",
  "lparen",
  "rparen",
  "number",
  "plus",
  "minus",
  "asterisk",
  "divide",
  "power",
  "equals"
};

struct Token {
  TokenType type{ TokenType::unknown };
  std::string lexeme{ "" };
};

class Lexer {
public:
  Lexer(const std::string& input);

  const std::vector<Token>& run();

private:
  StringReader m_reader;
  std::vector<Token> m_tokens;

  void readIdentifier();
  void readNumber();

  void pushToken(TokenType type, const std::string& lexeme = "");

};

std::ostream& operator<<(std::ostream& os, const Token& tok);

}
