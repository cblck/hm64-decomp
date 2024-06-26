#include "common.h"


INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonreg", __rmonGetGRegisters);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonreg", __rmonSetGRegisters);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonreg", __rmonGetFRegisters);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonreg", __rmonSetFRegisters);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonreg", rmonGetRcpRegister);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonreg", __rmonGetSRegs);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonreg", __rmonSetSRegs);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonreg", __rmonGetVRegs);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonreg", __rmonSetVRegs);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonreg", __rmonGetRegisterContents);
