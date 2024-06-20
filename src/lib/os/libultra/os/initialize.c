#include "common.h"


INCLUDE_ASM(const s32, "lib/os/libultra/os/initialize", osInitialize);

INCLUDE_ASM(const s32, "lib/os/libultra/os/initialize", createSpeedParam);

INCLUDE_RODATA(const s32, "lib/os/libultra/os/initialize", osClockRate);

INCLUDE_RODATA(const s32, "lib/os/libultra/os/initialize", osViClock);

INCLUDE_RODATA(const s32, "lib/os/libultra/os/initialize", __osShutdown);

INCLUDE_RODATA(const s32, "lib/os/libultra/os/initialize", __OSGlobalIntMask);
