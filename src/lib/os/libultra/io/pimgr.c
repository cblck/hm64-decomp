#include "common.h"

INCLUDE_ASM(const s32, "lib/os/libultra/io/pimgr", osCreatePiManager);

INCLUDE_ASM(const s32, "lib/os/libultra/io/pimgr", ramromMain);

INCLUDE_RODATA(const s32, "lib/os/libultra/io/pimgr", __osPiDevMgr);

INCLUDE_RODATA(const s32, "lib/os/libultra/io/pimgr", __osPiTable);

INCLUDE_RODATA(const s32, "lib/os/libultra/io/pimgr", __osCurrentHandle);
