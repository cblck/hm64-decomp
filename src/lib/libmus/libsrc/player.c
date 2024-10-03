#include <ultra64.h>

/* include current configuration settings */
#include "libmus_config.h"

#ifdef	NU_SYSTEM
#ifdef N_AUDIO
#include <n_libaudio_sc.h>
#include <n_libaudio_sn_sc.h>
#else
#include <libaudio.h>
#endif
#include <nusys.h>
#else
#include "audio.h"
#endif
#include "player.h"


#include "common.h"

// version 2.00

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", __MusIntMain);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", __MusIntGetNewNote);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", __MusIntFlushPending);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", __MusIntSetVolumeAndPan);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", __MusIntSetPitch);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", __MusIntInitEnvelope);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", __MusIntProcessEnvelope);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", __MusIntInitSweep);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", __MusIntProcessSweep);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", __MusIntProcessWobble);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", __MusIntProcessVibrato);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", __MusIntProcessContinuousVolume);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", __MusIntProcessContinuousPitchBend);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", __MusIntPowerOf2);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", __MusIntRemapPtrBank);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", __MusIntRandom);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", __MusIntInitialiseChannel);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", __MusIntFindChannel);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", __MusIntMemSet);

// not in version 1.0
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", __MusIntRemapPtrs);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", __MusIntStartEffect);
