#include "parser.h"

namespace ulang {
  Parser::Parser(const std::vector<Token> &input) {
    m_tokens = input;
  }

  Program Parser::parse() {
    parseExpr();
    return m_program;
  }

  bool Parser::containsOneOf(const std::initializer_list<TokenType> &types, bool allowAdvance) {
    if (m_pos > m_tokens.size() - 1) return false;

    bool contains = false;
    for (const auto& type : types) {
      if (currentToken().type == type) {
        contains = true;
        break;
      }
    }
    if (contains && allowAdvance) advance();
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
    } else if (containsOneOf({ TokenType::minus })) {
      Instruction push = { .opCode = OpCode::push, .object0 = { ObjectType::number, std::stod(currentToken().lexeme) } };
      m_program.push_back(push);
      Instruction neg = { .opCode = OpCode::neg };
      m_program.push_back(neg);
    } else if (containsOneOf({ TokenType::identifier })) {
      // Call functions/access variables
      // if the next token is a lParen, it's a function
      auto ident = previousToken().lexeme;
      if (containsOneOf({ TokenType::lParen }, false)) { // without advancing
        parseAtom();

        Instruction pushFn = { .opCode = OpCode::push, .object0 = { ObjectType::string, ident } };
        m_program.push_back(pushFn);
        Instruction call = { .opCode = OpCode::call };
        m_program.push_back(call);
      } else {
        Instruction pushVar = { .opCode = OpCode::push, .object0 = { ObjectType::string, ident } };
        m_program.push_back(pushVar);
        Instruction varAcc = { .opCode = OpCode::varAccess };
        m_program.push_back(varAcc);
      }
    } else {
      // TODO: Error handling
    }
  }

  void Parser::parsePow() {
    parseAtom(); // left

    // read the right side and push a POW instruction
    if (containsOneOf({ TokenType::power })) {
      parseExpr();
      Instruction pow = { .opCode = OpCode::pow };
      m_program.push_back(pow);
    } else {
      parseMulDiv();
    }
  }

  void Parser::parseMulDiv() {
    if (containsOneOf({ TokenType::asterisk, TokenType::divide })) {
      auto op = previousToken().type;
      parseExpr();
      Instruction muldiv = { .opCode = op == TokenType::asterisk ? OpCode::mul : OpCode::div };
      m_program.push_back(muldiv);
    } else {
      parseAddSub();
    }
  }

  void Parser::parseAddSub() {
    if (containsOneOf({ TokenType::plus, TokenType::minus })) {
      auto op = previousToken().type;
      parseExpr();
      Instruction addsub = { .opCode = op == TokenType::plus ? OpCode::add : OpCode::sub };
      m_program.push_back(addsub);
    } else {
      // TODO: Errors
    }
  }

  void Parser::parseExpr() {
    parsePow();
  }

} // namespace ulang
