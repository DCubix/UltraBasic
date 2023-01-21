#pragma once

#include <vector>
#include <string>
#include <functional>

class StringReader {
public:
  StringReader() = default;
  StringReader(const std::string& input);

  char peek(size_t offset = 0) const;
  bool hasNext() const;

  char pop();

  std::string popWhile(const std::function<bool(char)>& cond, bool checkLast = false);
  std::string peekWhile(const std::function<bool(char)>& cond);

  void flush();

private:
  std::vector<char> m_data;
  std::vector<size_t> m_peekSizes;
};
