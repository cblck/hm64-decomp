#include "common.h"


INCLUDE_ASM(const s32, "lib/os/libultra/os/thread", __osDequeueThread);

INCLUDE_RODATA(const s32, "lib/os/libultra/os/thread", __osThreadTail);

INCLUDE_RODATA(const s32, "lib/os/libultra/os/thread", __osRunQueue);

INCLUDE_RODATA(const s32, "lib/os/libultra/os/thread", __osActiveQueue);

INCLUDE_RODATA(const s32, "lib/os/libultra/os/thread", __osRunningThread);

INCLUDE_RODATA(const s32, "lib/os/libultra/os/thread", ____osFaultedThread);
