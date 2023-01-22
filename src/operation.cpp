#include "operation.h"

#include "stl_utils.hpp"

namespace ulang {
  Object op_add_number_number(const ArgList& args) {
    return { ObjectType::number, std::get<double>(args[0]->second) + std::get<double>(args[1]->second) };
  }

  Object op_sub_number_number(const ArgList& args) {
    return { ObjectType::number, std::get<double>(args[0]->second) - std::get<double>(args[1]->second) };
  }

  Object op_mul_number_number(const ArgList& args) {
    return { ObjectType::number, std::get<double>(args[0]->second) * std::get<double>(args[1]->second) };
  }

  Object op_div_number_number(const ArgList& args) {
    return { ObjectType::number, std::get<double>(args[0]->second) / std::get<double>(args[1]->second) };
  }

  Object op_pow_number_number(const ArgList& args) {
    return { ObjectType::number, ::pow(std::get<double>(args[0]->second), std::get<double>(args[1]->second)) };
  }

  const std::vector<OperationDefinition> DefaultOperations::operationDefinitions = {
    { Operator::add, { ObjectType::number, ObjectType::number }, op_add_number_number },
    { Operator::sub, { ObjectType::number, ObjectType::number }, op_sub_number_number },
    { Operator::mul, { ObjectType::number, ObjectType::number }, op_mul_number_number },
    { Operator::div, { ObjectType::number, ObjectType::number }, op_div_number_number },
    { Operator::pow, { ObjectType::number, ObjectType::number }, op_pow_number_number }
  };

  std::optional<OperationDefinition> DefaultOperations::getOperation(Operator op, std::initializer_list<ObjectType> argTypes) {
    auto pos = std::find_if(operationDefinitions.begin(), operationDefinitions.end(), [=](const OperationDefinition& od) {
      if (argTypes.size() != od.args.size()) return false;

      bool different = false;
      for (size_t i = 0; i < argTypes.size(); i++) {
        auto a1 = *(argTypes.begin() + i);
        auto a2 = od.args[i];
        if (a1 != a2) {
          different = true;
        }
      }
      return od.op == op && !different;
    });
    if (pos == operationDefinitions.end()) return {};
    return *pos;
  }

} // namespace ulang
