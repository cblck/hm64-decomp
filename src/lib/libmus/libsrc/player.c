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

/* define note data */
#define REST		96	/* c8 = rest				*/
#define BASEOFFSET	48	/* c4 = sample base note default	*/

/* convert unsigned char values to floats */
#define U8_TO_FLOAT(c) ((c)&128) ? -(256-(c)) : (c)


/* internal function prototypes */
static ALMicroTime __MusIntMain(void* node);
static void __MusIntProcessContinuousVolume(channel_t* cp);
static void __MusIntProcessContinuousPitchBend(channel_t* cp);
static void __MusIntGetNewNote(channel_t* cp, int x);
static void __MusIntFlushPending(channel_t* cp, int x);
static void __MusIntSetVolumeAndPan(channel_t* cp, int x);
static void __MusIntSetPitch(channel_t* cp, int x, float offset);
static void __MusIntInitEnvelope(channel_t* cp);
static void __MusIntProcessEnvelope(channel_t* cp);
static void __MusIntInitSweep(channel_t* cp);
static void __MusIntProcessSweep(channel_t* cp);
static float __MusIntProcessVibrato(channel_t* cp);
static float __MusIntProcessWobble(channel_t* cp);

/* Internal utility functions... */
static void __MusIntInitialiseChannel(channel_t* cp);
static void __MusIntRemapPtrBank(char* pptr, char* wptr);

static float __MusIntPowerOf2(float x);
static int  __MusIntRandom(int range);
static void __MusIntMemSet(void* dest, unsigned char value, unsigned long length);
static int __MusIntFindChannel(song_t* addr, int song_chan);
static unsigned long __MusIntStartEffect(channel_t* cp, int number, int volume, int pan, int priority);
static void __MusIntRemapPtrs(void* addr, void* offset, int count);

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
extern ALPlayer        plr_player;                /* synthesizer player        */
#ifdef	NU_SYSTEM
extern ALHeap	       nuAuHeap;                  /* audio heap                */
#else
extern ALHeap	       heap;                      /* audio heap                */
#endif
extern int             max_channels;              /* number of channels        */
extern ALVoice* mus_voices;               /* audio library voices      */
extern channel_t* mus_channels;             /* music player channels     */
extern unsigned char** mus_effects;             /* address of sound effects  */
extern int* mus_priority;             /* address of sfx prioritys  */
extern int             mus_vsyncs_per_second;     /* video refresh rate        */
extern ALMicroTime     mus_next_frame_time;	  /* time until next frame     */
extern unsigned short  mus_master_volume_effects; /* sound effect master value */
extern unsigned short  mus_master_volume_songs;   /* song master volume        */
extern unsigned long   mus_current_handle;        /* current handle number     */
extern long            mus_random_seed;           /* random number seed value  */
extern ptr_bank_t* mus_init_bank;		  /* sample bank to initialise */
extern ptr_bank_t* mus_default_bank;	  /* sample bank default       */

/* music player control flag */
extern unsigned long __muscontrol_flag;

// player_commands.c

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Fstop);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Fwave);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Fport);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Fportoff);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Fdefa);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Ftempo);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Fendit);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Fcutoff);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Fvibup);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Fvibdown);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Fviboff);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Flength);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Fignore);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Ftrans);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Fignore_trans);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Fdistort);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Fenvelope);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Fenvoff);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Fenvon);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Ftroff);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Ftron);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Ffor);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Fnext);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Fwobble);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Fwobbleoff);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Fvelon);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Fveloff);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Fvelocity);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Fpan);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Fstereo);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Fdrums);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Fdrumsoff);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Fprint);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Fgoto);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Freverb);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", FrandNote);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", FrandVolume);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", FrandPand);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Fvolume);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Fstartfx);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Fbendrange);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", Fsweep);

extern command_func_t jumptable[];

// player_api.c

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", MusInitialize);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", MusSetMasterVolume);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", MusStartSong);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", MusStartEffect);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", MusStartEffect2);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", MusStop);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", MusAsk);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", MusHandleStop);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", MusHandleAsk);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", MusHandleSetVolume);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", MusHandlePan);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", MusHandleSetFreqOffset);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", MusHandleSetTempo);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", MusHandleSetReverb);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", MusBankInitialize);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", MusBankStartSong);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", MusBankStartEffect);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", MusBankStartEffect2);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", MusHandleGetPtrAddr);

// player.c

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

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", __MusIntRemapPtrs);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", __MusIntStartEffect);
