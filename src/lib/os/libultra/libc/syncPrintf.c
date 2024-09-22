#include "common.h"

INCLUDE_ASM(const s32, "lib/os/libultra/libc/syncPrintf", kmc_proutSyncPrintf);

INCLUDE_ASM(const s32, "lib/os/libultra/libc/syncPrintf", osSyncPrintf);

INCLUDE_ASM(const s32, "lib/os/libultra/libc/syncPrintf", rmonPrintf);

// proutSyncPrintF or proutSprintF
INCLUDE_ASM(const s32, "lib/os/libultra/libc/syncPrintf", func_80100F84);

INCLUDE_RODATA(const s32, "lib/os/libultra/libc/syncPrintf", syncprintf_data_0000);

INCLUDE_RODATA(const s32, "lib/os/libultra/libc/syncPrintf", syncprintf_data_0004);

INCLUDE_RODATA(const s32, "lib/os/libultra/libc/syncPrintf", syncprintf_data_0008);

INCLUDE_RODATA(const s32, "lib/os/libultra/libc/syncPrintf", syncprintf_data_000C);
