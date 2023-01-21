#include "vm.h"

#include <iostream>
#include <string>
#include "operation.h"

namespace ulang {
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
    Object obPi = { .type = ObjectType::number, .number = 3.141592654 };
    Object obSin = { .type = ObjectType::function, .function = buintin_sin };
    Object obCos = { .type = ObjectType::function, .function = buintin_cos };

    m_globalScope->declare("pi", VariableType::immutableStorage, obPi);
    m_globalScope->declare("sin", VariableType::immutableStorage, obSin);
    m_globalScope->declare("cos", VariableType::immutableStorage, obCos);
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
          if (ob.type != ObjectType::number) {
            m_programStack.push({ ObjectType::null, nullptr });
          } else {
            m_programStack.push({ .type = ObjectType::number, .number = -ob.number });
          }
        } break;
        case OpCode::jump:
          // TODO: ...
          break;
        case OpCode::call:
          auto str = popString();
          auto var = m_globalScope->find(str);
          // TODO: not found error

          if (var.has_value()) {
            auto&& val = var.value()->value;
            if (val.type == ObjectType::function) val.function(this);
            // TODO: not callable error
          }
          break;
      }
    }
  }

  void VirtualMachine::dumpStack() {
    while (!m_programStack.empty()) {
      auto ob = m_programStack.top(); m_programStack.pop();
      // TODO: Dump it
    }
  }

  void VirtualMachine::pushNumber(double number) {
    Object ob = { .type = ObjectType::number, .number = number };
    m_programStack.push(ob);
  }

  double VirtualMachine::popNumber() {
    auto ob = m_programStack.top(); m_programStack.pop();
    if (ob.type != ObjectType::number) {
      // TODO: Error handling
      return 0.0;
    }
    return ob.number;
  }

  void VirtualMachine::pushString(const std::string &str) {
    char* strData = new char[str.size()];
    ::strcpy(strData, str.data());
    // i know this is bad, but idk what else to use other than tagged unions :(
    Object ob = { .type = ObjectType::string, .string = strData };
    m_programStack.push(ob);
  }

  std::string VirtualMachine::popString() {
    auto ob = m_programStack.top(); m_programStack.pop();
    if (ob.type != ObjectType::string) {
      // TODO: Error handling
      return "";
    }
    std::string str(ob.string);
    delete[] ob.string;
    return str;
  }

  const Instruction& VirtualMachine::fetchNext() {
    return m_program[m_pc++];
  }

  Object VirtualMachine::binaryOperation(Object a, Object b, OpCode opCode) {
    if (a.type == ObjectType::null || b.type == ObjectType::null) {
      // TODO: Exception handling
      return { ObjectType::null, nullptr };
    }

    if (a.type != b.type) {
      // TODO: Operators on different types
      return { ObjectType::null, nullptr };
    }

    Object ret{};
    switch (opCode) {
      default: break;
      case OpCode::add: {
        auto op = DefaultOperations::getOperation(Operator::add, { a.type, b.type });
        if (!op.has_value()) {
          // TODO: Exception handling
          return { .type = ObjectType::null, .null = nullptr };
        }
        ret = op.value().operation({ a, b });
      } break;
      case OpCode::sub: {
        auto op = DefaultOperations::getOperation(Operator::sub, { a.type, b.type });
        if (!op.has_value()) {
          // TODO: Exception handling
          return { .type = ObjectType::null, .null = nullptr };
        }
        ret = op.value().operation({ a, b });
      } break;
      case OpCode::mul: {
        auto op = DefaultOperations::getOperation(Operator::mul, { a.type, b.type });
        if (!op.has_value()) {
          // TODO: Exception handling
          return { .type = ObjectType::null, .null = nullptr };
        }
        ret = op.value().operation({ a, b });
      } break;
      case OpCode::div: {
        auto op = DefaultOperations::getOperation(Operator::div, { a.type, b.type });
        if (!op.has_value()) {
          // TODO: Exception handling
          return { .type = ObjectType::null, .null = nullptr };
        }
        ret = op.value().operation({ a, b });
      } break;
      case OpCode::pow: {
        auto op = DefaultOperations::getOperation(Operator::pow, { a.type, b.type });
        if (!op.has_value()) {
          // TODO: Exception handling
          return { .type = ObjectType::null, .null = nullptr };
        }
        ret = op.value().operation({ a, b });
      } break;
    }

    return ret;
  }
}
