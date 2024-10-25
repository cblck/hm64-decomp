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
//STATIC ALPlayer        plr_player;                /* synthesizer player        */
#ifdef	NU_SYSTEM
extern ALHeap	       nuAuHeap;                  /* audio heap                */
#else
STATIC ALHeap	       heap;                      /* audio heap                */
#endif
//STATIC int             max_channels;              /* number of channels        */
//STATIC ALVoice         *mus_voices;               /* audio library voices      */
//STATIC channel_t       *mus_channels;             /* music player channels     */
//STATIC unsigned char   **mus_effects;             /* address of sound effects  */
//STATIC int             *mus_priority;             /* address of sfx prioritys  */
//STATIC int             mus_vsyncs_per_second;     /* video refresh rate        */
//STATIC ALMicroTime     mus_next_frame_time;	  /* time until next frame     */
//STATIC unsigned short  mus_master_volume_effects; /* sound effect master value */
//STATIC unsigned short  mus_master_volume_songs;   /* song master volume        */
//STATIC unsigned long   mus_current_handle;        /* current handle number     */
//STATIC long            mus_random_seed;           /* random number seed value  */
//STATIC ptr_bank_t      *mus_init_bank;		  /* sample bank to initialise */
//STATIC ptr_bank_t      *mus_default_bank;	  /* sample bank default       */

/* music player control flag */
//unsigned long __muscontrol_flag;


/* C files included directly (to avoid lots of global variables in the library)*/
/* these files are separated just for readability                              */
//#include "player_commands.c"
static unsigned char* Fstop(channel_t* cp, unsigned char* ptr);
static unsigned char* Fwave(channel_t* cp, unsigned char* ptr);
static unsigned char* Fport(channel_t* cp, unsigned char* ptr);
static unsigned char* Fportoff(channel_t* cp, unsigned char* ptr);
static unsigned char* Fdefa(channel_t* cp, unsigned char* ptr);
static unsigned char* Ftempo(channel_t* cp, unsigned char* ptr);
static unsigned char* Fendit(channel_t* cp, unsigned char* ptr);
static unsigned char* Fcutoff(channel_t* cp, unsigned char* ptr);
static unsigned char* Fvibup(channel_t* cp, unsigned char* ptr);
static unsigned char* Fvibdown(channel_t* cp, unsigned char* ptr);
static unsigned char* Fviboff(channel_t* cp, unsigned char* ptr);
static unsigned char* Flength(channel_t* cp, unsigned char* ptr);
static unsigned char* Fignore(channel_t* cp, unsigned char* ptr);
static unsigned char* Ftrans(channel_t* cp, unsigned char* ptr);
static unsigned char* Fignore_trans(channel_t* cp, unsigned char* ptr);
static unsigned char* Fdistort(channel_t* cp, unsigned char* ptr);
static unsigned char* Fenvelope(channel_t* cp, unsigned char* ptr);
static unsigned char* Fenvoff(channel_t* cp, unsigned char* ptr);
static unsigned char* Fenvon(channel_t* cp, unsigned char* ptr);
static unsigned char* Ftroff(channel_t* cp, unsigned char* ptr);
static unsigned char* Ftron(channel_t* cp, unsigned char* ptr);
static unsigned char* Ffor(channel_t* cp, unsigned char* ptr);
static unsigned char* Fnext(channel_t* cp, unsigned char* ptr);
static unsigned char* Fwobble(channel_t* cp, unsigned char* ptr);
static unsigned char* Fwobbleoff(channel_t* cp, unsigned char* ptr);
static unsigned char* Fvelon(channel_t* cp, unsigned char* ptr);
static unsigned char* Fveloff(channel_t* cp, unsigned char* ptr);
static unsigned char* Fvelocity(channel_t* cp, unsigned char* ptr);
static unsigned char* Fpan(channel_t* cp, unsigned char* ptr);
static unsigned char* Fstereo(channel_t* cp, unsigned char* ptr);
static unsigned char* Fdrums(channel_t* cp, unsigned char* ptr);
static unsigned char* Fdrumsoff(channel_t* cp, unsigned char* ptr);
static unsigned char* Fprint(channel_t* cp, unsigned char* ptr);
static unsigned char* Fgoto(channel_t* cp, unsigned char* ptr);
static unsigned char* Freverb(channel_t* cp, unsigned char* ptr);
static unsigned char* FrandNote(channel_t* cp, unsigned char* ptr);
static unsigned char* FrandVolume(channel_t* cp, unsigned char* ptr);
static unsigned char* FrandPan(channel_t* cp, unsigned char* ptr);
static unsigned char* Fvolume(channel_t* cp, unsigned char* ptr);
static unsigned char* Fstartfx(channel_t* cp, unsigned char* ptr);
static unsigned char* Fbendrange(channel_t* cp, unsigned char* ptr);
static unsigned char* Fsweep(channel_t* cp, unsigned char* ptr);

INCLUDE_ASM(const s32, "lib/libmus/player", Fstop);

INCLUDE_ASM(const s32, "lib/libmus/player", Fwave);

INCLUDE_ASM(const s32, "lib/libmus/player", Fport);

INCLUDE_ASM(const s32, "lib/libmus/player", Fportoff);

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

command_func_t jumptable[]=
{
  Fstop,		//80
  Fwave,		//81
  Fport,		//82
  Fportoff,		//83
  Fdefa,		//84
  Ftempo,		//85
  Fcutoff,		//86
  Fendit, 		//87
  Fvibup,		//88
  Fvibdown,		//89
  Fviboff,		//8a
  Flength, 		//8b
  Fignore, 		//8c
  Ftrans, 		//8d
  Fignore_trans, 	//8e
  Fdistort,		//8f
  Fenvelope,		//90
  Fenvoff,		//91
  Fenvon, 		//92
  Ftroff, 		//93
  Ftron,  		//94
  Ffor, 		//95
  Fnext,		//96
  Fwobble, 		//97
  Fwobbleoff, 		//98
  Fvelon, 		//99
  Fveloff, 		//9a
  Fvelocity, 		//9b
  Fpan, 		//9c
  Fstereo, 		//9d
  Fdrums, 		//9e
  Fdrumsoff, 		//9f
  Fprint, 		//a0
  Fgoto, 		//a1
  Freverb, 		//a2
  FrandNote, 		//a3
  FrandVolume,		//a4
  FrandPan,		//a5
  Fvolume,		//a6
  Fstartfx,		//a7
  Fbendrange,		//a8
  Fsweep,		//a8
};

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
