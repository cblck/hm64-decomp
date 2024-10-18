#include "common.h"

INCLUDE_ASM(const s32, "lib/os/libultra/libc/syncPrintf", kmc_proutSyncPrintf);

INCLUDE_ASM(const s32, "lib/os/libultra/libc/syncPrintf", osSyncPrintf);

INCLUDE_ASM(const s32, "lib/os/libultra/libc/syncPrintf", rmonPrintf);

INCLUDE_ASM(const s32, "lib/os/libultra/libc/syncPrintf", proutSyncPrintf);

INCLUDE_RODATA(const s32, "lib/os/libultra/libc/syncPrintf", stat);

INCLUDE_RODATA(const s32, "lib/os/libultra/libc/syncPrintf", wport);

INCLUDE_RODATA(const s32, "lib/os/libultra/libc/syncPrintf", piok);

INCLUDE_RODATA(const s32, "lib/os/libultra/libc/syncPrintf", kmc_proutSyncPrintf_column);
