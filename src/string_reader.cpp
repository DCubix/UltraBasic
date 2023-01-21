#include "string_reader.h"

#include "stl_utils.hpp"

StringReader::StringReader(const std::string &input) {
  this->m_data = std::vector<char>(input.begin(), input.end());
}

char StringReader::peek(size_t offset) const {
  if (m_data.empty()) return 0;
  return m_data.front();
}

bool StringReader::hasNext() const {
  return peek() != 0;
}

char StringReader::pop() {
  char temp = peek();
  m_data.erase(m_data.begin());
  return temp;
}

std::string StringReader::popWhile(const std::function<bool(char)> &cond, bool checkLast) {
  std::string ret = "";
  while (hasNext() && cond(peek())) {
    ret += pop();
  }
  if (cond(peek()) && checkLast) pop();
  return ret;
}

std::string StringReader::peekWhile(const std::function<bool(char)> &cond) {
  std::string ret = "";
  size_t off = stu::vec::reduce<size_t, size_t>(m_peekSizes, [](size_t a, size_t b) { return a + b; });
  size_t siz = 0;
  while (peek(off) != 0 && cond(peek(off))) {
    ret += peek(off++);
    siz++;
  }
  m_peekSizes.push_back(siz);
  return ret;
}

void StringReader::flush() {
  while (!m_peekSizes.empty()) {
    size_t sz = m_peekSizes.front();
    m_peekSizes.erase(m_peekSizes.begin());

    while (sz > 0) {
      pop();
      sz--;
    }
  }
}
