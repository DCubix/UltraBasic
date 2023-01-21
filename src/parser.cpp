#include "parser.h"

namespace ulang {
  Parser::Parser(const std::vector<Token> &input) {
    m_tokens = input;
  }

  Program Parser::parse() {
    parseExpr();
    return m_program;
  }

  bool Parser::containsOneOf(const std::initializer_list<TokenType> &types) {
    if (m_pos > m_tokens.size() - 1) return false;

    bool contains = false;
    for (const auto& type : types) {
      if (currentToken().type == type) {
        contains = true;
        break;
      }
    }
    if (contains) advance();
    return contains;
  }

  bool Parser::expectOneOf(const std::initializer_list<TokenType> &types) {
    if (containsOneOf(types)) {
      return true;
    }
    // TODO: Error handling
    return false;
  }

  void Parser::parseAtom() {
    if (containsOneOf({ TokenType::number })) {
      Instruction push = { .opCode = OpCode::push, .object0 = { ObjectType::number, std::stod(previousToken().lexeme) } };
      m_program.push_back(push);
    } else if (containsOneOf({ TokenType::lParen })) {
      parseExpr();
      expectOneOf({ TokenType::rParen });
    } else {
      // TODO: Error handling
    }
  }

  void Parser::parsePow() {
    parseAtom(); // left

    // read the right side and push a POW instruction
    if (containsOneOf({ TokenType::power })) {
      parseExpr();
      Instruction pow = { .opCode = OpCode::powReg };
      m_program.push_back(pow);
    } else {
      parseMulDiv();
    }
  }

  void Parser::parseMulDiv() {
    if (containsOneOf({ TokenType::asterisk, TokenType::divide })) {
      auto op = previousToken().type;
      parseExpr();
      Instruction muldiv = { .opCode = op == TokenType::asterisk ? OpCode::mulReg : OpCode::divReg };
      m_program.push_back(muldiv);
    } else {
      parseAddSub();
    }
  }

  void Parser::parseAddSub() {
    if (containsOneOf({ TokenType::plus, TokenType::minus })) {
      auto op = previousToken().type;
      parseExpr();
      Instruction addsub = { .opCode = op == TokenType::plus ? OpCode::addReg : OpCode::subReg };
      m_program.push_back(addsub);
    } else {
      // TODO: Errors
    }
  }

  void Parser::parseExpr() {
    parsePow();
  }

} // namespace ulang
