#pragma once

#include <vector>

#include "instructions.hpp"
#include "regfile.hpp"

class Interpreter {
public:
  Interpreter(std::byte *mem, uint32_t memLength);

  bool IsStopped() const { return this->isStopped; }

  void Tick();
  void Reset();

private:
  std::byte *const mem;
  const uint32_t memLength;
  RegFile regFile;
  bool isStopped;

  Instruction ReadInstruction(uint32_t address) const;

  int32_t HandleSyscall(uint32_t number, int32_t arg1);

  void rw(uint32_t index, uint32_t value); // Register Write
  uint32_t rr(uint32_t index) const;       // Register Read
};
