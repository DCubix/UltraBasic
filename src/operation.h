#pragma once

#include <functional>
#include <initializer_list>
#include <vector>
#include <optional>

#include "object.h"

#define getArg(T, index) std::get<T>(stu::vec::at<Object>(args, index).second)

namespace ulang {

  using ArgList = std::initializer_list<Object>;
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
    std::initializer_list<ObjectType> args;
    Operation operation;
  };

  class DefaultOperations {
  public:
    static const std::vector<OperationDefinition> operationDefinitions;
    static std::optional<OperationDefinition> getOperation(Operator op, std::initializer_list<ObjectType> argTypes);
  };

} // namespace ulang
