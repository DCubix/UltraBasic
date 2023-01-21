#include "parser.h"

namespace ulang {
  Parser::Parser(const std::vector<Token> &input) {
    m_tokens = input;
    for (size_t i = 0; i < MaxRegisterCount; i++) m_availableRegisters.push_back(i);
  }

  Program Parser::parse() {
    parseExpr();
    return m_program;
  }

  bool Parser::containsOneOf(const std::initializer_list<TokenType> &types) {
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
      advance();
      return true;
    }
    // TODO: Error handling
    return false;
  }

  size_t Parser::allocRegister() {
    if (m_availableRegisters.empty()) {
      return 9999;
    }
    size_t reg = m_availableRegisters.back();
    m_availableRegisters.pop_back();
    m_usedRegisters.push_back(reg);
    return reg;
  }

  void Parser::releaseRegister(size_t reg) {
    auto pos = std::find(m_usedRegisters.begin(), m_usedRegisters.end(), reg);
    if (pos == m_usedRegisters.end()) return;

    m_availableRegisters.push_back(reg);
    m_usedRegisters.erase(pos);
  }

  void Parser::parseAtom() {
    if (containsOneOf({ TokenType::number })) {
      m_temporaryObjects.push_back({ ObjectType::number, std::stod(previousToken().lexeme) });
      pushMovFromTemp();
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
      pow.reg0 = m_usedRegisters[0];
      pow.reg1 = m_usedRegisters[1];
      m_program.push_back(pow);
    } else {
      parseMulDiv();
    }
  }

  void Parser::parseMulDiv() {
    if (containsOneOf({ TokenType::asterisk, TokenType::divide })) {
      parseExpr();
      Instruction muldiv = { .opCode = previousToken().type == TokenType::asterisk ? OpCode::mulReg : OpCode::divReg };
      muldiv.reg0 = m_usedRegisters[0];
      muldiv.reg1 = m_usedRegisters[1];
      m_program.push_back(muldiv);
    } else {
      parseAddSub();
    }
  }

  void Parser::parseAddSub() {
    if (containsOneOf({ TokenType::plus, TokenType::minus })) {
      parseExpr();
      Instruction addsub = { .opCode = previousToken().type == TokenType::plus ? OpCode::addReg : OpCode::subReg };
      addsub.reg0 = m_usedRegisters[0];
      addsub.reg1 = m_usedRegisters[1];
      m_program.push_back(addsub);
    } else {
      // TODO: Errors
    }
  }

  void Parser::parseExpr() {
    parsePow();
  }

  Object Parser::popTemp() {
    auto tmp = m_temporaryObjects.back();
    m_temporaryObjects.pop_back();
    return tmp;
  }

  void Parser::pushMovFromTemp() {
    if (m_temporaryObjects.empty()) {
      // move reg to reg
      Instruction mov = {
        .opCode = OpCode::moveRegReg
      };
      mov.reg0 = m_usedRegisters[0];
      mov.reg1 = m_usedRegisters[1];
      m_program.push_back(mov);
    } else {
      auto tmp = popTemp();
      Instruction mov = { .opCode = OpCode::moveImmReg, .imm = tmp };
      mov.reg0 = allocRegister();
      m_program.push_back(mov);
    }
  }

} // namespace ulang
