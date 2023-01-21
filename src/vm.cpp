#include "vm.h"

#include <iostream>
#include <string>
#include "operation.h"

namespace ulang {
  struct make_string_functor {
    std::string operator()(const std::string &x) const { return x; }
    std::string operator()(double x) const { return std::to_string(x); }
    std::string operator()(nullptr_t x) const { return "null"; }
  };

  VirtualMachine::VirtualMachine(const Program &program) {
    m_program = program;
  }

  void VirtualMachine::run() {
    while (m_pc < m_program.size()) {
      auto&& inst = fetchNext();

      switch (inst.opCode) {
        default: break;
        case OpCode::push: m_programStack.push(inst.object0); break;
        case OpCode::addReg:
        case OpCode::subReg:
        case OpCode::mulReg:
        case OpCode::divReg:
        case OpCode::powReg: {
          auto ob1 = m_programStack.top(); m_programStack.pop();
          auto ob0 = m_programStack.top(); m_programStack.pop();
          auto res = binaryOperation(ob0, ob1, inst.opCode);
          m_programStack.push(res);
        } break;
        case OpCode::jump:
          // TODO: ...
          break;
      }
    }
  }

  void VirtualMachine::dumpStack() {
    while (!m_programStack.empty()) {
      auto ob = m_programStack.top(); m_programStack.pop();
      std::cout << std::visit(make_string_functor(), ob.second) << std::endl;
    }
  }

  const Instruction& VirtualMachine::fetchNext() {
    return m_program[m_pc++];
  }

  Object VirtualMachine::binaryOperation(Object a, Object b, OpCode opCode) {
    if (a.first == ObjectType::null || b.first == ObjectType::null) {
      // TODO: Exception handling
      return { ObjectType::null, nullptr };
    }

    if (a.first != b.first) {
      // TODO: Operators on different types
      return { ObjectType::null, nullptr };
    }

    Object ret{};
    switch (opCode) {
      default: break;
      case OpCode::addReg: {
        auto op = DefaultOperations::getOperation(Operator::add, { a.first, b.first });
        if (!op.has_value()) {
          // TODO: Exception handling
          return { ObjectType::null, nullptr };
        }
        ret = op.value().operation({ a, b });
      } break;
      case OpCode::subReg: {
        auto op = DefaultOperations::getOperation(Operator::sub, { a.first, b.first });
        if (!op.has_value()) {
          // TODO: Exception handling
          return { ObjectType::null, nullptr };
        }
        ret = op.value().operation({ a, b });
      } break;
      case OpCode::mulReg: {
        auto op = DefaultOperations::getOperation(Operator::mul, { a.first, b.first });
        if (!op.has_value()) {
          // TODO: Exception handling
          return { ObjectType::null, nullptr };
        }
        ret = op.value().operation({ a, b });
      } break;
      case OpCode::divReg: {
        auto op = DefaultOperations::getOperation(Operator::div, { a.first, b.first });
        if (!op.has_value()) {
          // TODO: Exception handling
          return { ObjectType::null, nullptr };
        }
        ret = op.value().operation({ a, b });
      } break;
      case OpCode::powReg: {
        auto op = DefaultOperations::getOperation(Operator::pow, { a.first, b.first });
        if (!op.has_value()) {
          // TODO: Exception handling
          return { ObjectType::null, nullptr };
        }
        ret = op.value().operation({ a, b });
      } break;
    }

    return ret;
  }
}
