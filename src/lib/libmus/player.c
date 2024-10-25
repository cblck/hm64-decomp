#include "common.h"
/*********************************************************

  player.c : Nintendo 64 Music Tools Programmers Library
  (c) Copyright 1997, Software Creations (Holdings) Ltd.

  Version 2.00

  Music player source file.

**********************************************************/

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

/* dont define things as static for test program */
#ifndef _AUDIOVISIBLE
#define STATIC static
#else
#define STATIC
#endif

/* define note data */
#define REST		96	/* c8 = rest				*/
#define BASEOFFSET	48	/* c4 = sample base note default	*/

/* convert unsigned char values to floats */
#define U8_TO_FLOAT(c) ((c)&128) ? -(256-(c)) : (c)


/* internal function prototypes */
static ALMicroTime __MusIntMain			(void *node);
static void __MusIntProcessContinuousVolume	(channel_t *cp);
static void __MusIntProcessContinuousPitchBend	(channel_t *cp);
static void __MusIntGetNewNote			(channel_t *cp, int x);
static void __MusIntFlushPending		(channel_t *cp, int x);
static void __MusIntSetVolumeAndPan		(channel_t *cp, int x);
static void __MusIntSetPitch			(channel_t *cp, int x, float offset);
static void __MusIntInitEnvelope		(channel_t *cp);
static void __MusIntProcessEnvelope		(channel_t *cp);
static void __MusIntInitSweep			(channel_t *cp);
static void __MusIntProcessSweep		(channel_t *cp);
static float __MusIntProcessVibrato		(channel_t *cp);
static float __MusIntProcessWobble		(channel_t *cp);

/* Internal utility functions... */
static void __MusIntInitialiseChannel		(channel_t *cp);
static void __MusIntRemapPtrBank		(char *pptr, char *wptr);

static float __MusIntPowerOf2			(float x);
static int  __MusIntRandom			(int range);
static void __MusIntMemSet			(void *dest, unsigned char value, unsigned long length);
static int __MusIntFindChannel                  (song_t *addr, int song_chan);
static unsigned long __MusIntStartEffect        (channel_t *cp, int number, int volume, int pan, int priority);
static void __MusIntRemapPtrs			(void *addr, void *offset, int count);

/* external vars */

#ifdef	NU_SYSTEM
 #ifdef	N_AUDIO
extern N_ALGlobals	auGlobal;
 #else
extern ALGlobals	auGlobal;
 #endif
#else
extern AMAudioMgr      __am;
#endif


/* internal vars */
STATIC ALPlayer        plr_player;                /* synthesizer player        */
#ifdef	NU_SYSTEM
extern ALHeap	       nuAuHeap;                  /* audio heap                */
#else
STATIC ALHeap	       heap;                      /* audio heap                */
#endif
STATIC int             max_channels;              /* number of channels        */
STATIC ALVoice         *mus_voices;               /* audio library voices      */
STATIC channel_t       *mus_channels;             /* music player channels     */
STATIC unsigned char   **mus_effects;             /* address of sound effects  */
STATIC int             *mus_priority;             /* address of sfx prioritys  */
STATIC int             mus_vsyncs_per_second;     /* video refresh rate        */
STATIC ALMicroTime     mus_next_frame_time;	  /* time until next frame     */
STATIC unsigned short  mus_master_volume_effects; /* sound effect master value */
STATIC unsigned short  mus_master_volume_songs;   /* song master volume        */
STATIC unsigned long   mus_current_handle;        /* current handle number     */
STATIC long            mus_random_seed;           /* random number seed value  */
STATIC ptr_bank_t      *mus_init_bank;		  /* sample bank to initialise */
STATIC ptr_bank_t      *mus_default_bank;	  /* sample bank default       */

/* music player control flag */
unsigned long __muscontrol_flag;


/* C files included directly (to avoid lots of global variables in the library)*/
/* these files are separated just for readability                              */
#include "player_commands.c"
#include "player_api.c"

/* C files for SC effect extensions */
#ifdef SUPPORT_EFFECTS
#include "player_effects.c"
#endif

/* timing workspace */
#ifdef SUPPORT_PROFILER
unsigned long _mus_cpu_last = 0;
unsigned long _mus_cpu_worst = 0;
#endif

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [EXTERNAL FUNCTION]
  __MusExtPlayerDelete(player)

  [Parameters]
  player	address of player structure

  [Explanation]
  Called by synthesis driver to remove the music player. This should never be
  called!

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

#if 0 /* never called */
void __MusExtPlayerDelete(ALPlayer *player)
{
#ifdef	NU_SYSTEM
  alSynRemovePlayer(&auGlobal.drvr, player);
#else
  alSynRemovePlayer(&__am.g.drvr, player);
#endif
  player->handler = NULL;
}
#endif

// version 2.00

INCLUDE_ASM(const s32, "lib/libmus/player", __MusIntMain);

INCLUDE_ASM(const s32, "lib/libmus/player", __MusIntGetNewNote);

INCLUDE_ASM(const s32, "lib/libmus/player", __MusIntFlushPending);

INCLUDE_ASM(const s32, "lib/libmus/player", __MusIntSetVolumeAndPan);

INCLUDE_ASM(const s32, "lib/libmus/player", __MusIntSetPitch);

INCLUDE_ASM(const s32, "lib/libmus/player", __MusIntInitEnvelope);

INCLUDE_ASM(const s32, "lib/libmus/player", __MusIntProcessEnvelope);

INCLUDE_ASM(const s32, "lib/libmus/player", __MusIntInitSweep);

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
