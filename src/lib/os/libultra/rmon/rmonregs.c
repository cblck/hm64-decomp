#include "common.h"


INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonregs", __rmonGetGRegisters);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonregs", __rmonSetGRegisters);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonregs", __rmonGetFRegisters);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonregs", __rmonSetFRegisters);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonregs", rmonGetRcpRegister);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonregs", __rmonGetSRegs);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonregs", __rmonSetSRegs);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonregs", __rmonGetVRegs);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonregs", __rmonSetVRegs);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonregs", __rmonGetRegisterContents);
