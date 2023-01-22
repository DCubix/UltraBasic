#include "vm.h"

#include <iostream>
#include <string>
#include "operation.h"

namespace ulang {
  struct make_string_functor {
    std::string operator()(const std::string &x) const { return x; }
    std::string operator()(double x) const { return std::to_string(x); }
    std::string operator()(nullptr_t x) const { return "null"; }
    std::string operator()(UFunc x) const { return "UFunc"; }
  };

  size_t buintin_sin(VirtualMachine* vm) {
    double param0 = vm->popNumber();
    vm->pushNumber(::sin(param0));
    return 1;
  }

  size_t buintin_cos(VirtualMachine* vm) {
    double param0 = vm->popNumber();
    vm->pushNumber(::cos(param0));
    return 1;
  }

  VirtualMachine::VirtualMachine(const Program &program) {
    m_program = program;

    // initialize global scope with some functions and variables
    m_globalScope = std::make_shared<Scope>();

    m_globalScope->declare("pi", VariableType::immutableStorage, { ObjectType::number, 3.141592654 });
    m_globalScope->declare("sin", VariableType::immutableStorage, { ObjectType::function, buintin_sin });
    m_globalScope->declare("cos", VariableType::immutableStorage, { ObjectType::function, buintin_cos });
  }

  void VirtualMachine::run() {
    while (m_pc < m_program.size()) {
      auto&& inst = fetchNext();

      switch (inst.opCode) {
        default: break;
        case OpCode::push: m_programStack.push(inst.object0); break;
        case OpCode::add:
        case OpCode::sub:
        case OpCode::mul:
        case OpCode::div:
        case OpCode::pow: {
          auto ob1 = m_programStack.top(); m_programStack.pop();
          auto ob0 = m_programStack.top(); m_programStack.pop();
          auto res = binaryOperation(ob0, ob1, inst.opCode);
          m_programStack.push(res);
        } break;
        case OpCode::neg: {
          auto ob = m_programStack.top(); m_programStack.pop();
          if (ob.first != ObjectType::number) {
            m_programStack.push({ ObjectType::null, nullptr });
          } else {
            m_programStack.push({ ObjectType::number, -std::get<double>(ob.second) });
          }
        } break;
        case OpCode::jump:
          // TODO: ...
          break;
        case OpCode::varAccess: {
          auto str = popString();
          auto var = m_globalScope->find(str);
          if (var.has_value()) {
            m_programStack.push(var.value()->value);
          }
        } break;
        case OpCode::call: {
          auto str = popString();
          auto var = m_globalScope->find(str);
          // TODO: not found error

          if (var.has_value()) {
            auto&& val = var.value()->value;
            if (val.first == ObjectType::function) {
              auto&& fn = std::get<UFunc>(val.second);
              fn(this);
            }
            // TODO: not callable error
          }
        } break;
      }
    }
  }

  void VirtualMachine::dumpStack() {
    while (!m_programStack.empty()) {
      auto ob = m_programStack.top(); m_programStack.pop();
      std::cout << std::visit(make_string_functor(), ob.second) << std::endl;
    }
  }

  void VirtualMachine::pushNumber(double number) {
    Object ob = { ObjectType::number, number };
    m_programStack.push(ob);
  }

  double VirtualMachine::popNumber() {
    auto [ type, value ] = m_programStack.top(); m_programStack.pop();
    if (type != ObjectType::number) {
      // TODO: Error handling
      return 0.0;
    }
    return std::get<double>(value);
  }

  void VirtualMachine::pushString(const std::string &str) {
    Object ob = { ObjectType::string, str };
    m_programStack.push(ob);
  }

  std::string VirtualMachine::popString() {
    auto [ type, value ] = m_programStack.top(); m_programStack.pop();
    if (type != ObjectType::string) {
      // TODO: Error handling
      return "";
    }
    return std::get<std::string>(value);
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
      case OpCode::add: {
        auto op = DefaultOperations::getOperation(Operator::add, { a.first, b.first });
        if (!op.has_value()) {
          // TODO: Exception handling
          return { ObjectType::null, nullptr };
        }
        ret = op.value().operation({ a, b });
      } break;
      case OpCode::sub: {
        auto op = DefaultOperations::getOperation(Operator::sub, { a.first, b.first });
        if (!op.has_value()) {
          // TODO: Exception handling
          return { ObjectType::null, nullptr };
        }
        ret = op.value().operation({ a, b });
      } break;
      case OpCode::mul: {
        auto op = DefaultOperations::getOperation(Operator::mul, { a.first, b.first });
        if (!op.has_value()) {
          // TODO: Exception handling
          return { ObjectType::null, nullptr };
        }
        ret = op.value().operation({ a, b });
      } break;
      case OpCode::div: {
        auto op = DefaultOperations::getOperation(Operator::div, { a.first, b.first });
        if (!op.has_value()) {
          // TODO: Exception handling
          return { ObjectType::null, nullptr };
        }
        ret = op.value().operation({ a, b });
      } break;
      case OpCode::pow: {
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
