#pragma once

#include <array>
#include <vector>
#include <functional>

#include "object.h"

namespace ulang {

  constexpr size_t MaxRegisterCount = 8;

  enum class OpCode : uint8_t {
    noOp = 0,

    moveImmReg, // reg[N] = imm
    moveRegReg, // reg[N1] = reg[N2]
    
    addReg, // reg[N1] += reg[N2]
    subReg, // reg[N1] -= reg[N2]
    mulReg, // reg[N1] *= reg[N2]
    divReg, // reg[N1] /= reg[N2] (check for zero-division)
    powReg, // reg[N1] = pow(reg[N1], reg[N2])

    jump // PC = imm
  };

  struct Instruction {
    OpCode opCode{ OpCode::noOp };
    
    union {
      size_t reg0, reg1;
      Object imm;
    };
  };

  using Program = std::vector<Instruction>;

  class VirtualMachine {
  public:
    VirtualMachine() = default;
    VirtualMachine(const Program& program);

    void run();

  private:
    std::array<Object, MaxRegisterCount> m_registers;
    
    Program m_program;
    size_t m_pc{ 0 };

    const Instruction& fetchNext();
    Object binaryOperation(Object a, Object b, OpCode op);

  };

}
