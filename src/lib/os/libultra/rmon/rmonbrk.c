#include "common.h"

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonbrk", __rmonSetBreak);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonbrk", _rmonListBreak);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonbrk", __rmonClearBreak);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonbrk", __rmonGetBranchTarget);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonbrk", __rmonSetSingleStep);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonbrk", __rmonGetExceptionStatus);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonbrk", __rmonHitBreak);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonbrk", __rmonHitSpBreak);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonbrk", __rmonHitCpuFault);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonbrk", rmonFindFaultedThreads);
