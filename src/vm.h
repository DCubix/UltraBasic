#pragma once

#include <array>
#include <vector>
#include <stack>
#include <functional>
#include <map>

#include "object.h"
#include "scope.h"

namespace ulang {

  enum class OpCode : uint8_t {
    noOp = 0,

    push,
    // pop,
    
    add,
    sub,
    mul,
    div,
    pow,
    neg,

    jump, // PC = imm

    call, // Call a function from the stack

    varAccess,
    varDeclare,
    varAssign,

    logEq,
    logNe,
    logGt,
    logGe,
    logLt,
    logLe,
    logNot
  };

  struct Instruction {
    OpCode opCode{ OpCode::noOp };
    Object object0{};
  };

  using Program = std::vector<Instruction>;

  class VirtualMachine {
  public:
    VirtualMachine() = default;
    VirtualMachine(const Program& program);

    void run();
    void dumpStack();

    Object pop();

    void pushNumber(double number);
    double popNumber();

    void pushString(const std::string& str);
    std::string popString();

    // TODO: more functions to push/pop different types

  private:
    std::shared_ptr<Scope> m_globalScope{};

    std::map<size_t, std::string> m_strings;

    std::vector<Object> m_programStack;
    
    Program m_program;
    size_t m_pc{ 0 };

    const Instruction& fetchNext();
    Object binaryOperation(Object& a, Object& b, OpCode op);

  };

}
