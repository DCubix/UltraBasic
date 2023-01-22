#include "lexer.h"

namespace ulang {
  Lexer::Lexer(const std::string &input) {
    m_reader = StringReader(input);
  }

  const std::vector<Token>& Lexer::run() {
    auto&& sr = m_reader;

    while (sr.hasNext()) {
      char ch = sr.peek();

      if (::isalpha(ch) || ch == '_') { // identifier
        readIdentifier();
      } else if (ch == '(') {
        pushToken(TokenType::lParen);
        sr.pop();
      } else if (ch == ')') {
        pushToken(TokenType::rParen);
        sr.pop();
      } else if (::isdigit(ch) || ch == '.') {
        readNumber();
      } else if (ch == '+') {
        pushToken(TokenType::plus);
        sr.pop();
      } else if (ch == '-') {
        pushToken(TokenType::minus);
        sr.pop();
      } else if (ch == '*') {
        pushToken(TokenType::asterisk);
        sr.pop();
      } else if (ch == '/') {
        pushToken(TokenType::divide);
        sr.pop();
      } else if (ch == '^') {
        pushToken(TokenType::power);
        sr.pop();
      } else if (ch == '=') {
        pushToken(TokenType::equals);
        sr.pop();
      } else { // other characters
        sr.pop();
      }
    }

    return m_tokens;
  }

  void Lexer::readIdentifier() {
    std::string ret = m_reader.popWhile([](char ch) {
      return ::isalnum(ch) || ch == '_';
    });
    
    auto kwPos = std::find(std::begin(keywords), std::end(keywords), ret);
    if (kwPos != std::end(keywords)) {
      pushToken(TokenType::keyword, ret);
    } else {
      pushToken(TokenType::identifier, ret);
    }
  }

  void Lexer::readNumber() {
    std::string ret = m_reader.popWhile([](char ch) {
      return ::isdigit(ch) || ch == '.';
    });
    pushToken(TokenType::number, ret);
  }

  void Lexer::pushToken(TokenType type, const std::string &lexeme) {
    Token tok = { .type = type, .lexeme = lexeme };
    m_tokens.push_back(tok);
  }

  std::ostream &operator<<(std::ostream &os, const Token &tok) {
    os << tokenName[size_t(tok.type)];
    if (!tok.lexeme.empty()) {
      os << "(" << tok.lexeme << ")";
    }
    return os;
  }
}