#include "common.h"

//#include "player_commands.c"

INCLUDE_ASM(const s32, "lib/libmus/player", Fstop);

INCLUDE_ASM(const s32, "lib/libmus/player", Fwave);

INCLUDE_ASM(const s32, "lib/libmus/player", Fport);

INCLUDE_ASM(const s32, "lib/libmus/player", FPortoff);

INCLUDE_ASM(const s32, "lib/libmus/player", Fdefa);

INCLUDE_ASM(const s32, "lib/libmus/player", Ftempo);

INCLUDE_ASM(const s32, "lib/libmus/player", Fendit);

INCLUDE_ASM(const s32, "lib/libmus/player", Fcutoff);

INCLUDE_ASM(const s32, "lib/libmus/player", Fvibup);

INCLUDE_ASM(const s32, "lib/libmus/player", Fvibdown);

INCLUDE_ASM(const s32, "lib/libmus/player", Fviboff);

INCLUDE_ASM(const s32, "lib/libmus/player", Flength);

INCLUDE_ASM(const s32, "lib/libmus/player", Fignore);

INCLUDE_ASM(const s32, "lib/libmus/player", Ftrans);

INCLUDE_ASM(const s32, "lib/libmus/player", Fignore_trans);

INCLUDE_ASM(const s32, "lib/libmus/player", Fdistort);

INCLUDE_ASM(const s32, "lib/libmus/player", Fenvelope);

INCLUDE_ASM(const s32, "lib/libmus/player", Fenvoff);

INCLUDE_ASM(const s32, "lib/libmus/player", Fenvon);

INCLUDE_ASM(const s32, "lib/libmus/player", Ftroff);

INCLUDE_ASM(const s32, "lib/libmus/player", Ftron);

INCLUDE_ASM(const s32, "lib/libmus/player", Ffor);

INCLUDE_ASM(const s32, "lib/libmus/player", Fnext);

INCLUDE_ASM(const s32, "lib/libmus/player", Fwobble);

INCLUDE_ASM(const s32, "lib/libmus/player", Fwobbleoff);

INCLUDE_ASM(const s32, "lib/libmus/player", Fvelon);

INCLUDE_ASM(const s32, "lib/libmus/player", Fveloff);

INCLUDE_ASM(const s32, "lib/libmus/player", Fvelocity);

INCLUDE_ASM(const s32, "lib/libmus/player", Fpan);

INCLUDE_ASM(const s32, "lib/libmus/player", Fstereo);

INCLUDE_ASM(const s32, "lib/libmus/player", Fdrums);

INCLUDE_ASM(const s32, "lib/libmus/player", Fdrumsoff);

INCLUDE_ASM(const s32, "lib/libmus/player", Fprint);

INCLUDE_ASM(const s32, "lib/libmus/player", Fgoto);

INCLUDE_ASM(const s32, "lib/libmus/player", Freverb);

INCLUDE_ASM(const s32, "lib/libmus/player", FrandNote);

INCLUDE_ASM(const s32, "lib/libmus/player", FrandVolume);

INCLUDE_ASM(const s32, "lib/libmus/player", FrandPan);

INCLUDE_ASM(const s32, "lib/libmus/player", Fvolume);

INCLUDE_ASM(const s32, "lib/libmus/player", Fstartfx);

INCLUDE_ASM(const s32, "lib/libmus/player", Fbendrange);

INCLUDE_ASM(const s32, "lib/libmus/player", Fsweep);

//#include "player_api.c"

INCLUDE_ASM(const s32, "lib/libmus/player", MusInitialize);

INCLUDE_ASM(const s32, "lib/libmus/player", MusSetMasterVolume);

INCLUDE_ASM(const s32, "lib/libmus/player", MusStartSong);

INCLUDE_ASM(const s32, "lib/libmus/player", MusStartEffect);

INCLUDE_ASM(const s32, "lib/libmus/player", MusStartEffect2);

INCLUDE_ASM(const s32, "lib/libmus/player", MusStop);

INCLUDE_ASM(const s32, "lib/libmus/player", MusAsk);

INCLUDE_ASM(const s32, "lib/libmus/player", MusHandleStop);

INCLUDE_ASM(const s32, "lib/libmus/player", MusHandleAsk);

INCLUDE_ASM(const s32, "lib/libmus/player", MusHandleSetVolume);

INCLUDE_ASM(const s32, "lib/libmus/player", MusHandlePan);

INCLUDE_ASM(const s32, "lib/libmus/player", MusHandleSetFreqOffset);

INCLUDE_ASM(const s32, "lib/libmus/player", MusHandleSetTempo);

INCLUDE_ASM(const s32, "lib/libmus/player", MusHandleSetReverb);

INCLUDE_ASM(const s32, "lib/libmus/player", MusBankInitialize);

INCLUDE_ASM(const s32, "lib/libmus/player", MusBankStartSong);

INCLUDE_ASM(const s32, "lib/libmus/player", MusBankStartEffect);

INCLUDE_ASM(const s32, "lib/libmus/player", MusBankStartEffect2);

INCLUDE_ASM(const s32, "lib/libmus/player", MusHandleGetPtrAddr);

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
