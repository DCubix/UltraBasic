#include "parser.h"

namespace ulang {
  Parser::Parser(const std::vector<Token> &input) {
    m_tokens = input;
  }

  Program Parser::parse() {
    parseVarDeclare();
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

  bool Parser::containsSequence(const std::initializer_list<TokenType> &types, bool allowAdvance) {
    if (m_pos > m_tokens.size() - 1) return false;
    bool contains = true;
    for (size_t i = 0; i < types.size(); i++) {
      if ((*(types.begin() + i)) != m_tokens[m_pos+i].type) {
        contains = false;
        break;
      }
    }
    if (contains && allowAdvance) {
      for (const auto& type : types) {
        advance();
      }
    }
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

  void Parser::parseVarDeclare() {
    if (containsOneOf({ TokenType::keyword }) && previousToken().lexeme == "var") {
      Instruction ident = { .opCode = OpCode::push, .object0 = { ObjectType::string, currentToken().lexeme } };
      m_program.push_back(ident);
      advance();
      
      Instruction varDecl = { .opCode = OpCode::varDeclare };
      m_program.push_back(varDecl);

      if (containsOneOf({ TokenType::equals })) {
        m_program.push_back(ident);

        parseExpr();

        Instruction varAssign = { .opCode = OpCode::varAssign };
        m_program.push_back(varAssign);
      }
    } else {
      parseAssignment();
    }
  }

  void Parser::parsePostfix() {
    if (containsOneOf({ TokenType::identifier })) {
      auto ident = previousToken().lexeme;
      Instruction pushFn = { .opCode = OpCode::push, .object0 = { ObjectType::string, ident } };
      
      if (containsOneOf({ TokenType::lParen }, false)) { // without advancing (function call)
        parseUnaryMinus();

        m_program.push_back(pushFn);
        
        Instruction call = { .opCode = OpCode::call };
        m_program.push_back(call);
      } else { // Variable access
        m_program.push_back(pushFn);
        Instruction varAcc = { .opCode = OpCode::varAccess };
        m_program.push_back(varAcc);
      }
       // TODO: Array access, member access
    } else {
      parseUnaryMinus();
    }
  }

  void Parser::parseAssignment() {
    if (containsSequence({ TokenType::identifier, TokenType::equals })) {
      auto ident = tokenAt(-2).lexeme;
      Instruction pushFn = { .opCode = OpCode::push, .object0 = { ObjectType::string, ident } };
      m_program.push_back(pushFn);

      parseExpr();

      Instruction varAssign = { .opCode = OpCode::varAssign };
      m_program.push_back(varAssign);
    } else {
      parsePostfix();
    }
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
