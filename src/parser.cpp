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
    } else if (containsOneOf({ TokenType::keyword })) {
      auto kw = previousToken().lexeme;
      if (kw == "true" || kw == "false") {
        Instruction boolInst = { .opCode = OpCode::push, .object0 = { ObjectType::boolean, kw == "true" ? true : false } };
        m_program.push_back(boolInst);
      }
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
        parseUnaryLogicNot();

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
      parseUnaryLogicNot();
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

  void Parser::parseUnaryLogicNot() {
    if (containsOneOf({ TokenType::logicalNot })) {
      parseUnaryMinus();
      Instruction neg = { .opCode = OpCode::logNot };
      m_program.push_back(neg);
    } else {
      parseUnaryMinus();
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

    if (m_pos >= m_tokens.size()) return;

    // read the right side and push a POW instruction
    if (containsOneOf({ TokenType::power })) {
      parseMulDiv();
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
      parseRelational();
      Instruction addsub = { .opCode = op == TokenType::plus ? OpCode::add : OpCode::sub };
      m_program.push_back(addsub);
    } else {
      parseRelational();
    }
  }

  void Parser::parseRelational() {
    if (containsOneOf({ TokenType::greater, TokenType::greaterEquals, TokenType::less, TokenType::lessEquals })) {
      auto tok = previousToken().type;
      parseEquality();
      Instruction op{};
      switch (tok) {
        default: break;
        case TokenType::greater: op.opCode = OpCode::logGt; break;
        case TokenType::greaterEquals: op.opCode = OpCode::logGe; break;
        case TokenType::less: op.opCode = OpCode::logLt; break;
        case TokenType::lessEquals: op.opCode = OpCode::logLe; break;
      }
      m_program.push_back(op);
    } else {
      parseEquality();
    }
  }

  void Parser::parseEquality() {
    if (containsOneOf({ TokenType::equality, TokenType::difference })) {
      auto tok = previousToken().type;
      parseExpr();
      Instruction op{};
      switch (tok) {
        default: break;
        case TokenType::equality: op.opCode = OpCode::logEq; break;
        case TokenType::difference: op.opCode = OpCode::logNe; break;
      }
      m_program.push_back(op);
    } else {
      parseExpr();
    }
  }

  void Parser::parseExpr() {
    parsePostfix();
  }

} // namespace ulang
