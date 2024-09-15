#include "common.h"


INCLUDE_ASM(const s32, "lib/os/libultra/os/timerintr", __osTimerServicesInit);

INCLUDE_ASM(const s32, "lib/os/libultra/os/timerintr", __osTimerInterrupt);

INCLUDE_ASM(const s32, "lib/os/libultra/os/timerintr", __osSetTimerIntr);

INCLUDE_ASM(const s32, "lib/os/libultra/os/timerintr", __osInsertTimer);
