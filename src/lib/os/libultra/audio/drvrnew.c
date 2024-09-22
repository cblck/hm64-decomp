#include "common.h"

INCLUDE_ASM(const s32, "lib/os/libultra/audio/drvrnew", init_lpfilter);

INCLUDE_ASM(const s32, "lib/os/libultra/audio/drvrnew", alFxNew);

INCLUDE_ASM(const s32, "lib/os/libultra/audio/drvrnew", alEnvmixerNew);

INCLUDE_ASM(const s32, "lib/os/libultra/audio/drvrnew", alLoadNew);

INCLUDE_ASM(const s32, "lib/os/libultra/audio/drvrnew", alResampleNew);

INCLUDE_ASM(const s32, "lib/os/libultra/audio/drvrnew", alAuxBusNew);

INCLUDE_ASM(const s32, "lib/os/libultra/audio/drvrnew", alMainBusNew);

INCLUDE_ASM(const s32, "lib/os/libultra/audio/drvrnew", alSaveNew);

INCLUDE_RODATA(const s32, "lib/os/libultra/audio/drvrnew", SMALLROOM_PARAMS);

INCLUDE_RODATA(const s32, "lib/os/libultra/audio/drvrnew", BIGROOM_PARAMS);

INCLUDE_RODATA(const s32, "lib/os/libultra/audio/drvrnew", ECHO_PARAMS);

INCLUDE_RODATA(const s32, "lib/os/libultra/audio/drvrnew", CHORUS_PARAMS);

INCLUDE_RODATA(const s32, "lib/os/libultra/audio/drvrnew", FLANGE_PARAMS);

INCLUDE_RODATA(const s32, "lib/os/libultra/audio/drvrnew", NULL_PARAMS);
