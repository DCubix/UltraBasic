#include "scope.h"

namespace ulang {
  std::optional<std::shared_ptr<Variable>> Scope::find(const std::string &name) {
    auto pos = std::find(m_variableNames.begin(), m_variableNames.end(), name);
    if (pos != m_variableNames.end()) {
      auto index = std::distance(m_variableNames.begin(), pos);
      return m_variables[index];
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
    Variable* var = new Variable();
    var->type = varType;
    var->value = value;
    m_variables.push_back(std::shared_ptr<Variable>(var));
    m_variableNames.push_back(name);
  }

} // namespace ulang
