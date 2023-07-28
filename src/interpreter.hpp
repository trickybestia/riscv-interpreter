#pragma once

#include <vector>

#include "instructions.hpp"
#include "regfile.hpp"

class Interpreter {
public:
  Interpreter();

  std::vector<std::byte> &Mem() { return this->mem; }
  bool Stop() const { return this->stop; }

  void Tick();
  void Reset();

private:
  std::vector<std::byte> mem;
  RegFile regFile;
  bool stop;

  Instruction ReadInstruction(uint32_t address) const;

  int32_t HandleSyscall(uint32_t number, int32_t arg1);

  void rw(uint32_t index, uint32_t value); // Register Write
  uint32_t rr(uint32_t index) const;       // Register Read
};
