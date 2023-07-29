#include <riscv-interpreter.h>

#include "interpreter.hpp"

extern "C" {
void *RiscVInterpreterNew(void *memory, uint32_t memoryLength) {
  return new Interpreter(reinterpret_cast<std::byte *>(memory), memoryLength);
}
void RiscVInterpreterDelete(void *interpreter) {
  delete reinterpret_cast<Interpreter *>(interpreter);
}

bool RiscVInterpreterIsStopped(void *interpreter) {
  return reinterpret_cast<Interpreter *>(interpreter)->IsStopped();
}

void RiscVInterpreterTick(void *interpreter) {
  reinterpret_cast<Interpreter *>(interpreter)->Tick();
}
void RiscVInterpreterReset(void *interpreter) {
  reinterpret_cast<Interpreter *>(interpreter)->Reset();
}
}
