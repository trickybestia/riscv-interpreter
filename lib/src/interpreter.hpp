#pragma once

#include <vector>

#include "instructions.hpp"
#include "regfile.hpp"

class Interpreter {
public:
  Interpreter(std::byte *mem, uint32_t memLength,
              int32_t (*syscallHandler)(void *interpreter, uint32_t number,
                                        int32_t arg1));

  bool IsStopped() const { return this->isStopped; }

  void Tick();
  void Reset();

private:
  std::byte *const mem;
  const uint32_t memLength;
  int32_t (*const syscallHandler)(void *interpreter, uint32_t number,
                                  int32_t arg1);
  RegFile regFile;
  bool isStopped;

  Instruction ReadInstruction(uint32_t address) const;

  void ReadMemory(std::byte *buffer, size_t count, uint32_t address) const;
  void WriteMemory(std::byte *buffer, size_t count, uint32_t address);

  void rw(uint32_t index, uint32_t value); // Register Write
  uint32_t rr(uint32_t index) const;       // Register Read

  void ThrowMemoryOutOfRangeException(uint32_t address) const;
};
