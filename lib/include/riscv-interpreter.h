#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void *RiscVInterpreterNew(void *memory, uint32_t memoryLength);
void RiscVInterpreterDelete(void *interpreter);

bool RiscVInterpreterIsStopped(void *interpreter);

void RiscVInterpreterTick(void *interpreter);
void RiscVInterpreterReset(void *interpreter);

#ifdef __cplusplus
}
#endif
