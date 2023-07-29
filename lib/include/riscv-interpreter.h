#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void *RiscVInterpreterNew(void *memory, uint32_t memoryLength,
                          int32_t (*syscallHandler)(void *interpreter,
                                                    uint32_t number,
                                                    int32_t arg1));
void RiscVInterpreterDelete(void *interpreter);

bool RiscVInterpreterIsStopped(void *interpreter);

void RiscVInterpreterTick(void *interpreter);
void RiscVInterpreterReset(void *interpreter);

#ifdef __cplusplus
}
#endif
