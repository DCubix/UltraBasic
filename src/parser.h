#pragma once

#include "lexer.h"
#include "vm.h"

namespace ulang {

  class Parser {
  public:
    Parser() = default;
    Parser(const std::vector<Token>& input);

    Program parse();

  private:
    size_t m_pos{ 0 };
    std::vector<Token> m_tokens;
    Program m_program{};

    const Token& currentToken() const { return m_tokens[m_pos]; }
    const Token& previousToken() const { return m_tokens[m_pos-1 % m_tokens.size()]; }
    const Token& tokenAt(int offset = 0) const { return m_tokens[(m_pos + offset) % m_tokens.size()]; }
    const Token& advance() { return m_tokens[m_pos++]; }

    bool containsOneOf(const std::initializer_list<TokenType>& types, bool allowAdvance = true);
    bool containsSequence(const std::initializer_list<TokenType>& types, bool allowAdvance = true);
    bool expectOneOf(const std::initializer_list<TokenType>& types);

    void parseAtom();

    void parseVarDeclare();
    void parsePostfix();
    void parseAssignment();
    void parseUnaryMinus();
    void parsePow();
    void parseMulDiv();
    void parseAddSub();
    void parseExpr();
    
  };

} // namespace ulang
