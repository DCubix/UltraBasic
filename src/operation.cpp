#include "operation.h"

#include "stl_utils.hpp"

#define getArg(T, index) std::get<T>(stu::vec::at<Object>(args, index).second)

namespace ulang {
  Object op_add_number_number(const ArgList& args) {
    return { ObjectType::number, getArg(double, 0) + getArg(double, 1) };
  }

  Object op_sub_number_number(const ArgList& args) {
    return { ObjectType::number, getArg(double, 0) - getArg(double, 1) };
  }

  Object op_mul_number_number(const ArgList& args) {
    return { ObjectType::number, getArg(double, 0) * getArg(double, 1) };
  }

  Object op_div_number_number(const ArgList& args) {
    return { ObjectType::number, getArg(double, 0) / getArg(double, 1) };
  }

  Object op_pow_number_number(const ArgList& args) {
    return { ObjectType::number, ::pow(getArg(double, 0), getArg(double, 1)) };
  }

  std::vector<OperationDefinition> operationDefinitions = {
    { Operator::add, { ObjectType::number, ObjectType::number }, op_add_number_number },
    { Operator::sub, { ObjectType::number, ObjectType::number }, op_sub_number_number },
    { Operator::mul, { ObjectType::number, ObjectType::number }, op_mul_number_number },
    { Operator::div, { ObjectType::number, ObjectType::number }, op_div_number_number },
    { Operator::pow, { ObjectType::number, ObjectType::number }, op_pow_number_number }
  };

  std::optional<OperationDefinition> getOperation(Operator op, std::initializer_list<ObjectType> argTypes) {
    auto pos = std::find_if(operationDefinitions.begin(), operationDefinitions.end(), [&](const OperationDefinition& od) {
      bool equal = argTypes.size() == od.args.size() &&
        std::equal(argTypes.begin(), argTypes.end(), od.args.begin(), od.args.end());
      return od.op == op && equal;
    });
    if (pos == operationDefinitions.end()) return {};
    return *pos;
  }

} // namespace ulang
