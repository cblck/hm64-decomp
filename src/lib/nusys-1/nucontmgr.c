#include "common.h"

INCLUDE_ASM(const s32, "lib/nusys-1/nucontmgr", nuContMgrInit);

// nuContMgrRemove
INCLUDE_ASM(const s32, "lib/nusys-1/nucontmgr", func_800FC130);

INCLUDE_ASM(const s32, "lib/nusys-1/nucontmgr", nuContDataClose);

INCLUDE_ASM(const s32, "lib/nusys-1/nucontmgr", nuContDataOpen);

// contReadData
INCLUDE_ASM(const s32, "lib/nusys-1/nucontmgr", func_800FC1A0);
