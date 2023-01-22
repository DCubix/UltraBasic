#pragma once

#include <string>
#include <vector>
#include <optional>
#include <memory>

#include "object.h"

namespace ulang {
  
  enum class VariableType : uint8_t {
    immutableStorage, mutableStorage
  };

  struct Variable {
    Variable() = default;
    ~Variable() = default;

    VariableType type;
    Object value;
  };

  class Scope {
  public:
    std::optional<std::shared_ptr<Variable>> find(const std::string& name);
    void declare(const std::string& name, const VariableType& varType, Object value);
    void inherit(const std::shared_ptr<Scope>& parent);

  private:
    std::shared_ptr<Scope> m_parent{ nullptr };
    std::vector<std::shared_ptr<Variable>> m_variables;
    std::vector<std::string> m_variableNames;
  };

} // namespace ulang
