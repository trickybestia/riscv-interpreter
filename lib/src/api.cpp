#include <riscv-interpreter.h>

#include "interpreter.hpp"

extern "C" {
void *RiscVInterpreterNew(void *memory, uint32_t memoryLength,
                          int32_t (*syscallHandler)(uint32_t number,
                                                    int32_t arg1)) {
  return new Interpreter(reinterpret_cast<std::byte *>(memory), memoryLength,
                         syscallHandler);
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
