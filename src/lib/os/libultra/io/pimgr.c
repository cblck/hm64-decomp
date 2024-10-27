#include "common.h"
//
#include "macros.h"
#include "PR/os_internal.h"
#include "PR/ultraerror.h"
#include "PRinternal/piint.h"
#include "PR/rdb.h"

OSDevMgr __osPiDevMgr = { 0 };
OSPiHandle* __osPiTable = NULL;
#if BUILD_VERSION >= VERSION_J
OSPiHandle __Dom1SpeedParam ALIGNED(8);
OSPiHandle __Dom2SpeedParam ALIGNED(8);
OSPiHandle* __osCurrentHandle[2] ALIGNED(8) = { &__Dom1SpeedParam, &__Dom2SpeedParam };
#else
extern OSPiHandle CartRomHandle;
extern OSPiHandle LeoDiskHandle;
OSPiHandle* __osCurrentHandle[2] ALIGNED(8) = { &CartRomHandle, &LeoDiskHandle };
#endif

INCLUDE_ASM(const s32, "lib/os/libultra/io/pimgr", osCreatePiManager);

INCLUDE_ASM(const s32, "lib/os/libultra/io/pimgr", ramromMain);
