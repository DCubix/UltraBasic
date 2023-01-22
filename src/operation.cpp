#include "operation.h"

#include <string>

#include "stl_utils.hpp"

#define param(x, type) std::get<type>(args[x]->second)
#define basic_op(typeA, typeB, op) std::get<typeA>(args[0]->second) op std::get<typeB>(args[1]->second)
#define function_op(typeA, typeB, fn) fn(std::get<typeA>(args[0]->second), std::get<typeB>(args[1]->second))
#define operator_header(name, typeA, typeB) Object op_##name##_##typeA##_##typeB##(const ArgList& args)
#define basic_arithmetic_op(name, ret, op) operator_header(name, number, number) { \
  return { ret, basic_op(double, double, op) }; \
}
#define boolean_op(name, ret, op) operator_header(name, bool, bool) { \
  return { ret, basic_op(bool, bool, op) }; \
}

namespace ulang {
  basic_arithmetic_op(add, ObjectType::number, +)
  basic_arithmetic_op(sub, ObjectType::number, -)
  basic_arithmetic_op(mul, ObjectType::number, *)
  basic_arithmetic_op(div, ObjectType::number, /)
  operator_header(pow, number, number) {
    return { ObjectType::number, function_op(double, double, ::pow) };
  }

  basic_arithmetic_op(eq, ObjectType::boolean, ==)
  basic_arithmetic_op(ne, ObjectType::boolean, !=)
  basic_arithmetic_op(gt, ObjectType::boolean, >)
  basic_arithmetic_op(lt, ObjectType::boolean, <)
  basic_arithmetic_op(ge, ObjectType::boolean, >=)
  basic_arithmetic_op(le, ObjectType::boolean, <=)

  boolean_op(eq, ObjectType::boolean, ==)
  boolean_op(ne, ObjectType::boolean, !=)

  operator_header(concat, string, string) {
    return { ObjectType::string, basic_op(std::string, std::string, +) };
  }

  operator_header(concat, string, number) {
    return { ObjectType::string, param(0, std::string) + std::to_string(param(1, double)) };
  }

  operator_header(concat, number, string) {
    return { ObjectType::string, std::to_string(param(1, double)) + param(1, std::string) };
  }

  const std::vector<OperationDefinition> DefaultOperations::operationDefinitions = {
    { Operator::add, { ObjectType::number, ObjectType::number }, op_add_number_number },
    { Operator::sub, { ObjectType::number, ObjectType::number }, op_sub_number_number },
    { Operator::mul, { ObjectType::number, ObjectType::number }, op_mul_number_number },
    { Operator::div, { ObjectType::number, ObjectType::number }, op_div_number_number },
    { Operator::pow, { ObjectType::number, ObjectType::number }, op_pow_number_number },

    { Operator::add, { ObjectType::string, ObjectType::string }, op_concat_string_string },
    { Operator::add, { ObjectType::number, ObjectType::string }, op_concat_number_string },
    { Operator::add, { ObjectType::string, ObjectType::number }, op_concat_string_number },

    { Operator::eq, { ObjectType::number, ObjectType::number }, op_eq_number_number },
    { Operator::neq, { ObjectType::number, ObjectType::number }, op_ne_number_number },
    { Operator::grt, { ObjectType::number, ObjectType::number }, op_gt_number_number },
    { Operator::lst, { ObjectType::number, ObjectType::number }, op_lt_number_number },
    { Operator::geq, { ObjectType::number, ObjectType::number }, op_ge_number_number },
    { Operator::leq, { ObjectType::number, ObjectType::number }, op_le_number_number },

    { Operator::eq, { ObjectType::boolean, ObjectType::boolean }, op_eq_bool_bool },
    { Operator::neq, { ObjectType::boolean, ObjectType::boolean }, op_ne_bool_bool }
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
