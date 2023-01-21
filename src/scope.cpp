#include "scope.h"

namespace ulang {
  std::optional<Variable*> Scope::find(const std::string &name) {
    auto pos = std::find(m_variableNames.begin(), m_variableNames.end(), name);
    if (pos != m_variableNames.end()) {
      auto index = std::distance(m_variableNames.begin(), pos);
      return m_variables[index].get();
    }
    if (m_parent) {
      return m_parent->find(name);
    }
    return {};
  }

  void Scope::inherit(const std::shared_ptr<Scope> &parent) {
    m_parent = parent;
  }

  void Scope::declare(const std::string& name, const VariableType &varType, Object value) {
    m_variables.emplace_back(std::make_unique<Variable>());
    Variable* var = m_variables.back().get();
    var->type = varType;
    var->value = value;
    m_variableNames.push_back(name);
  }
} // namespace ulang
