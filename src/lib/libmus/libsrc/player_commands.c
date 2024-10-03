#include "common.h"

// version 2.00

// player_commands.c

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Fstop);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Fwave);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Fport);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Fportoff);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Fdefa);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Ftempo);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Fendit);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Fcutoff);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Fvibup);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Fvibdown);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Fviboff);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Flength);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Fignore);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Ftrans);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Fignore_trans);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Fdistort);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Fenvelope);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Fenvoff);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Fenvon);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Ftroff);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Ftron);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Ffor);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Fnext);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Fwobble);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Fwobbleoff);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Fvelon);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Fveloff);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Fvelocity);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Fpan);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Fstereo);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Fdrums);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Fdrumsoff);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Fprint);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Fgoto);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Freverb);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", FrandNote);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", FrandVolume);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", FrandPand);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Fvolume);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Fstartfx);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Fbendrange);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Fsweep);


// version 2.00

// FIXME: split this somehow
// player_api.c

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", MusInitialize);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", MusSetMasterVolume);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", MusStartSong);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", MusStartEffect);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", MusStartEffect2);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", MusStop);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", MusAsk);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", MusHandleStop);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", MusHandleAsk);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", MusHandleSetVolume);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", MusHandlePan);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", MusHandleSetFreqOffset);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", MusHandleSetTempo);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", MusHandleSetReverb);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", MusBankInitialize);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", MusBankStartSong);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", MusBankStartEffect);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", MusBankStartEffect2);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", MusHandleGetPtrAddr);
