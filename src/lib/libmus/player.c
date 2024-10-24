#include "common.h"

// version 2.00

INCLUDE_ASM(const s32, "lib/libmus/player", __MusIntMain);

INCLUDE_ASM(const s32, "lib/libmus/player", __MusIntGetNewNote);

INCLUDE_ASM(const s32, "lib/libmus/player", __MusIntFlushPending);

INCLUDE_ASM(const s32, "lib/libmus/player", __MusIntSetVolumeAndPan);

INCLUDE_ASM(const s32, "lib/libmus/player", __MusIntSetPitch);

INCLUDE_ASM(const s32, "lib/libmus/player", __MusIntInitEnvelope);

INCLUDE_ASM(const s32, "lib/libmus/player", __MusIntProcessEnvelope);

INCLUDE_ASM(const s32, "lib/libmus/player", __MustIntInitSweep);

INCLUDE_ASM(const s32, "lib/libmus/player", __MusIntProcessSweep);

INCLUDE_ASM(const s32, "lib/libmus/player", __MusIntProcessWobble);

INCLUDE_ASM(const s32, "lib/libmus/player", __MusIntProcessVibrato);

INCLUDE_ASM(const s32, "lib/libmus/player", __MusIntProcessContinuousVolume);

INCLUDE_ASM(const s32, "lib/libmus/player", __MusIntProcessContinuousPitchBend);

INCLUDE_ASM(const s32, "lib/libmus/player", __MusIntPowerOf2);

INCLUDE_ASM(const s32, "lib/libmus/player", __MusIntRemapPtrBank);

INCLUDE_ASM(const s32, "lib/libmus/player", __MusIntRandom);

INCLUDE_ASM(const s32, "lib/libmus/player", __MusIntInitialiseChannel);

INCLUDE_ASM(const s32, "lib/libmus/player", __MusIntFindChannel);

INCLUDE_ASM(const s32, "lib/libmus/player", __MusIntMemSet);

INCLUDE_ASM(const s32, "lib/libmus/player", __MusIntRemapPtrs);

INCLUDE_ASM(const s32, "lib/libmus/player", __MusIntStartEffect);
