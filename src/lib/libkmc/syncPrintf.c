#include "common.h"

INCLUDE_ASM(const s32, "lib/libkmc/syncPrintf", kmc_proutSyncPrintf);

INCLUDE_ASM(const s32, "lib/libkmc/syncPrintf", osSyncPrintf);

INCLUDE_ASM(const s32, "lib/libkmc/syncPrintf", rmonPrintf);

// proutSyncPrintF or proutSprintF
INCLUDE_ASM(const s32, "lib/libkmc/syncPrintf", func_80100F84);

INCLUDE_RODATA(const s32, "lib/libkmc/syncPrintf", stat);

INCLUDE_RODATA(const s32, "lib/libkmc/syncPrintf", wport);

INCLUDE_RODATA(const s32, "lib/libkmc/syncPrintf", piok);

INCLUDE_RODATA(const s32, "lib/libkmc/syncPrintf", column);
