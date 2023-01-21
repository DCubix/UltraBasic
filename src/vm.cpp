#include "vm.h"

#include "operation.h"

namespace ulang {
  void VirtualMachine::run() {
    while (m_pc < m_program.size()) {
      auto&& inst = fetchNext();

      switch (inst.opCode) {
        default: break;
        case OpCode::moveImmReg: m_registers[inst.reg0] = inst.imm; break;
        case OpCode::moveRegReg: m_registers[inst.reg0] = m_registers[inst.reg1]; break;
        case OpCode::addReg:
        case OpCode::subReg:
        case OpCode::mulReg:
        case OpCode::divReg:
        case OpCode::powReg:
          m_registers[inst.reg0] = binaryOperation(m_registers[inst.reg0], m_registers[inst.reg1], inst.opCode);
          break;
        case OpCode::jump:
          m_pc = size_t(std::get<double>(inst.imm.second));
          break;
      }
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
