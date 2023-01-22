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
    } else {
      // TODO: Error handling
    }
  }

  void Parser::parsePostfix() {
    if (containsOneOf({ TokenType::identifier })) {
      auto ident = previousToken().lexeme;
      Instruction pushFn = { .opCode = OpCode::push, .object0 = { ObjectType::string, ident } };
      m_program.push_back(pushFn);
      
      if (containsOneOf({ TokenType::lParen }, false)) { // without advancing (function call)
        parseUnaryMinus();

        Instruction call = { .opCode = OpCode::call };
        m_program.push_back(call);
      } else if (containsOneOf({ TokenType::equals })) { // assignment
        parseAssignment();
      } else { // Variable access
        Instruction varAcc = { .opCode = OpCode::varAccess };
        m_program.push_back(varAcc);
      }
       // TODO: Array access, member access
    } else {
      parseUnaryMinus();
    }
  }

  void Parser::parseAssignment() {
    // we have parsed the identifier and equals symbols previously
    // let's get the value now
    parseExpr();

    // now push the assignment instruction
    Instruction varAssign = { .opCode = OpCode::varAssign };
    m_program.push_back(varAssign);
  }

  void Parser::parseUnaryMinus() {
    if (containsOneOf({ TokenType::minus })) {
      parsePow();
      Instruction neg = { .opCode = OpCode::neg };
      m_program.push_back(neg);
    } else {
      parsePow();
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
    parsePostfix();
  }

} // namespace ulang
