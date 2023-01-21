#pragma once

#include <array>
#include <vector>
#include <stack>
#include <functional>

#include "object.h"

namespace ulang {

  enum class OpCode : uint8_t {
    noOp = 0,

    push,
    // pop,
    
    addReg,
    subReg,
    mulReg,
    divReg,
    powReg,

    jump // PC = imm
  };

  struct Instruction {
    OpCode opCode{ OpCode::noOp };
    Object object0;
  };

  using Program = std::vector<Instruction>;

  class VirtualMachine {
  public:
    VirtualMachine() = default;
    VirtualMachine(const Program& program);

    void run();

    void dumpStack();

  private:
    std::stack<Object> m_programStack;
    
    Program m_program;
    size_t m_pc{ 0 };

    const Instruction& fetchNext();
    Object binaryOperation(Object a, Object b, OpCode op);

  };

}
