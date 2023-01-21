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
    const Token& advance() { return m_tokens[m_pos++]; }

    bool containsOneOf(const std::initializer_list<TokenType>& types);
    bool expectOneOf(const std::initializer_list<TokenType>& types);

    size_t allocRegister();
    void releaseRegister(size_t reg);

    void parseAtom();
    void parsePow();
    void parseMulDiv();
    void parseAddSub();
    void parseExpr();

    Object popTemp();
    void pushMovFromTemp();

    std::vector<size_t> m_availableRegisters;
    std::vector<size_t> m_usedRegisters;

    std::vector<Object> m_temporaryObjects;
  };

} // namespace ulang
