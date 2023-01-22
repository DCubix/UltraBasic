#pragma once

#include <functional>
#include <initializer_list>
#include <vector>
#include <optional>

#include "object.h"

namespace ulang {

  using ArgList = std::vector<Object*>;
  using Operation = std::function<Object(const ArgList& args)>;

  enum class Operator : uint8_t {
    add = 0,
    sub,
    mul,
    div,
    pow,
    neg
  };

  struct OperationDefinition {
    Operator op;
    std::vector<ObjectType> args;
    Operation operation;
  };

  class DefaultOperations {
  public:
    static const std::vector<OperationDefinition> operationDefinitions;
    static std::optional<OperationDefinition> getOperation(Operator op, std::initializer_list<ObjectType> argTypes);
  };

} // namespace ulang
