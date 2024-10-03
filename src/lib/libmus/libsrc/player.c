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

static unsigned char *Fstop(channel_t *cp, unsigned char *ptr)
{
  cp->pvolume = NULL;
  cp->ppitchbend = NULL;
  cp->song_addr = NULL;
  cp->IsFX = 0;
  cp->handle = 0;
  return (NULL);
}

static unsigned char *Fwave(channel_t *cp, unsigned char *ptr)
{
  unsigned short wave;

  wave = *ptr++;
  if(wave&0x80)
  {
    wave &= 0x7f;
    wave <<= 8;
    wave |= *ptr++;
  }
  cp->wave = wave;
  return (ptr);
}

static unsigned char *Fport(channel_t *cp, unsigned char *ptr)
{
  cp->port = *ptr++;
  if (cp->port)
    cp->port_base = cp->base_note;
  return (ptr);
}

static unsigned char *Fportoff(channel_t *cp, unsigned char *ptr)
{
  cp->port = 0;
  return (ptr);
}

static unsigned char *Fdefa(channel_t *cp, unsigned char *ptr)
{
  unsigned char value;

  // get envelope speed...
  value = *ptr++;
  if (value==0)	// cannot be zero!!!
    value=1;
  cp->env_speed = value;
  cp->env_speed_calc = 1024/value;
  
  // get envelope initial volume level...
  cp->env_init_vol = *ptr++;
  
  // get attack speed...
  value = *ptr++;
#ifdef _AUDIODEBUG
  if (value==0)
  {
    osSyncPrintf("PLAYER_COMMANDS.C: Fdefa attempting to set speed of zero.\n");
    value=1;      
  }
#endif
  cp->env_attack_speed = value;

  // get peak volume...
  cp->env_max_vol = *ptr++;

  // get attack precalc value...
  cp->env_attack_calc  = (1.0 / ((float)value)) * ((float)(cp->env_max_vol-cp->env_init_vol));

  // get decay speed...
  value = *ptr++;  
#ifdef _AUDIODEBUG
  if (value==0)
  {
    osSyncPrintf("PLAYER_COMMANDS.C: Fdefa attempting to set decay speed of zero.\n");
    value=1;      
  }
#endif
  cp->env_decay_speed = value;

  // get sustain volume level...
  cp->env_sustain_vol = *ptr++;

  // get sustain precalc value...
  cp->env_decay_calc = (1.0 / ((float)value)) * ((float)(cp->env_sustain_vol-cp->env_max_vol));

  // get release speed...
  value = *ptr++;
#ifdef _AUDIODEBUG
  if (value==0)
  {
    osSyncPrintf("PLAYER_COMMANDS.C: Fdefa attempting to set release speed of zero.\n");
    value=1;      
  }
#endif
  cp->env_release_speed = value;
  cp->env_release_calc = 1.0 / ((float)value);

     
  return (ptr);
}	

static unsigned char *Ftempo(channel_t *cp, unsigned char *ptr)
{
// tempo   = bpm
// fps     = mus_vsyncs_per_second
// 120 bpm = 96 fps
// therefore tempo = bmp(required)/120*96/mus_vsyncs_per_second

  channel_t *sp;
  int	i;
  int	temp, temp2;  

  temp	= (*ptr++)*256*96/120/mus_vsyncs_per_second;	
  temp2  = (temp*cp->temscale)>>7;
  if (cp->IsFX)
  {
    cp->channel_tempo=temp;
  }
  else
  {
    for	(i=0, sp=mus_channels;i<max_channels;i++,sp++)
    {
      if (sp->song_addr==cp->song_addr)
      {
	sp->channel_tempo_save=temp;
	sp->channel_tempo=temp2;
      }
    }
  }
  return (ptr);
}

static unsigned char *Fendit(channel_t *cp, unsigned char *ptr)
{
  cp->endit=*ptr++;
  cp->cutoff=0;
  return (ptr);
}

static unsigned char *Fcutoff(channel_t *cp, unsigned char *ptr)
{
  short tmp;

  tmp = (*ptr++)<<8;
  tmp |= *ptr++;

  cp->cutoff = tmp;
  cp->endit = 0;
  return (ptr);
}

static unsigned char *Fvibup(channel_t *cp, unsigned char *ptr)
{

  cp->vib_delay = *ptr++;
  cp->vib_speed = *ptr++;
  cp->vib_amount = ((float)*ptr++)/50.0;
  return (ptr);
}

static unsigned char *Fvibdown(channel_t *cp, unsigned char *ptr)
{
  cp->vib_delay = *ptr++;
  cp->vib_speed = *ptr++;
  cp->vib_amount = (-((float)*ptr++))/50.0;
  return (ptr);
}

static unsigned char *Fviboff(channel_t *cp, unsigned char *ptr)
{
  cp->vib_speed=0;
  cp->vibrato=0;        // if its been switched off you want it back to a normal note
  return (ptr);
}

static unsigned char *Flength(channel_t *cp, unsigned char *ptr)
{
  unsigned char value;

  value = *ptr++;
  if(value<128)
  {
    cp->fixed_length = value;
  }
  else
  {
    cp->fixed_length = ((int)(value&0x7f)*256);
    cp->fixed_length += (int)*ptr++;
  }
  return (ptr);
}

static unsigned char *Fignore(channel_t *cp, unsigned char *ptr)
{
  cp->ignore = 1;
  return (ptr);
}

static unsigned char *Ftrans(channel_t *cp, unsigned char *ptr)
{
  cp->transpose = *ptr++;
  return (ptr);
}

static unsigned char *Fignore_trans(channel_t *cp, unsigned char *ptr)
{
  cp->ignore_transpose = 1;
  return (ptr);
}

static unsigned char *Fdistort(channel_t *cp, unsigned char *ptr)
{
  int c;
  float f;
  
  c = (int)(*ptr++);
  if(c&0x80)
    c |= 0xffffff00;	// signed chars don't work
  f = (float)(c)/100.0;

  cp->freqoffset -= cp->distort;
  cp->freqoffset += f;
  cp->distort = f;
  return (ptr);
}

static unsigned char *Fenvelope(channel_t *cp, unsigned char *ptr)
{
  int tmp;

  tmp = *ptr++;
  if(tmp&0x80)
  {
    tmp &= 0x7f;
    tmp <<= 8;
    tmp |= *ptr++;
  }
  (void)Fdefa(cp, &cp->song_addr->EnvelopeData[tmp*7]);
  return (ptr);
}

static unsigned char *Fenvoff(channel_t *cp, unsigned char *ptr)
{
  cp->env_trigger_off = 1;
  return (ptr);
}

static unsigned char *Fenvon(channel_t *cp, unsigned char *ptr)
{
  cp->env_trigger_off = 0;
  return (ptr);
}

static unsigned char *Ftroff(channel_t *cp, unsigned char *ptr)
{
  cp->trigger_off = 1;
  return (ptr);
}

static unsigned char *Ftron(channel_t *cp, unsigned char *ptr)
{
  cp->trigger_off = 0;
  return (ptr);
}

static unsigned char *Ffor(channel_t *cp, unsigned char *ptr)
{
  int index;

  index = cp->for_stack_count;
  cp->for_count[index] = *ptr++;
  cp->for_stack[index] = ptr;
  cp->for_stackvol[index] = cp->pvolume;
  cp->for_stackpb[index] = cp->ppitchbend;
  cp->for_volume[index] = cp->volume;
  cp->for_pitchbend[index] = cp->pitchbend;
  cp->for_vol_count[index] = cp->cont_vol_repeat_count;
  cp->for_pb_count[index] = cp->cont_pb_repeat_count;
  cp->for_stack_count++;
  return (ptr);
}

static unsigned char *Fnext (channel_t *cp, unsigned char *ptr)
{
  int index;

  index = cp->for_stack_count-1;
  /* infinite loop? */
  if (cp->for_count[index]!=0xff)
  { /* still looping? */
    if (--(cp->for_count[index])==0)
    {
      cp->for_stack_count = index;
      index = -1;
    }
  }
  /* unstack pointers if necessary */
  if (index>-1)
  {
    ptr = cp->for_stack[index];
    cp->pvolume = cp->for_stackvol[index];
    cp->ppitchbend = cp->for_stackpb[index];
    cp->volume = cp->for_volume[index];
    cp->pitchbend = cp->for_pitchbend[index];
    cp->cont_vol_repeat_count = cp->for_vol_count[index];
    cp->cont_pb_repeat_count = cp->for_pb_count[index];
    cp->pitchbend_precalc = cp->pitchbend*cp->bendrange;
  }
  return (ptr);
}

static unsigned char *Fwobble(channel_t *cp, unsigned char *ptr)
{
  cp->wobble_amount = *ptr++;
  cp->wobble_on_speed = *ptr++;
  cp->wobble_off_speed = *ptr++;
  return(ptr);
}

static unsigned char *Fwobbleoff(channel_t *cp, unsigned char *ptr)
{
  cp->wobble_on_speed = 0;
  return (ptr);
}

static unsigned char *Fvelon(channel_t *cp, unsigned char *ptr)
{  
  cp->velocity_on = 1;
  return (ptr);
}

static unsigned char *Fveloff(channel_t *cp, unsigned char *ptr)
{
  cp->velocity_on = 0;
  return (ptr);
}

static unsigned char *Fvelocity(channel_t *cp, unsigned char *ptr)
{
  cp->default_velocity 	= /*velocity_map[*/*ptr++/*]*/;
  cp->velocity_on = 0;
  return (ptr);
}

static unsigned char *Fpan(channel_t *cp, unsigned char *ptr)
{
  cp->pan = (*ptr++)/2;
  return (ptr);
}

static unsigned char *Fstereo(channel_t *cp, unsigned char *ptr)
{
  return (ptr+2);
}

static unsigned char *Fdrums(channel_t *cp, unsigned char *ptr)
{
  unsigned long addr;

  addr = (unsigned long)cp->song_addr+(cp->song_addr->DrumData[*ptr++]);
  cp->pdrums = (unsigned char *)addr;
  return (ptr);
}

static unsigned char *Fdrumsoff(channel_t *cp, unsigned char *ptr)
{
  cp->pdrums = NULL;
  return (ptr);
}

static unsigned char *Fprint(channel_t *cp, unsigned char *ptr)
{
#ifdef _AUDIODEBUG
  osSyncPrintf("PLAYER_COMMANDS.C: Fprint -  %d (channel frame=%d)\n", *ptr++,  cp->channel_frame);
  return (ptr);
#else
  ptr++;
  return (ptr);
#endif
}

static unsigned char *Fgoto(channel_t *cp, unsigned char *ptr)
{
  int off, off1;

  /* 2 bytes for song offset */
  off1 = *ptr++<<8;
  off1 += *ptr++;

  /* get volume offset BEFORE updating pointer */
  /* 2 bytes for volume offset (never inside a run length bit) */
  off = *ptr++<<8;
  off += *ptr++;
  cp->pvolume = cp->pvolumebase+off;
  cp->cont_vol_repeat_count = 1;
  
  /* get pitchbend offset BEFORE updating pointer */
  /* 2 bytes for pitchbend offset (never inside a run length bit) */
  off = *ptr++<<8;
  off += *ptr++;
  cp->ppitchbend = cp->ppitchbendbase+off;
  cp->cont_pb_repeat_count = 1;
  
  return (cp->pbase+off1);
}

static unsigned char *Freverb(channel_t *cp, unsigned char *ptr)
{
  cp->reverb = *ptr++;
  return (ptr);
}

static unsigned char *FrandNote(channel_t *cp, unsigned char *ptr)
{	
  // rand_amount,rand_base  -- 20,-3 would give -3 to 16 as the value
  cp->transpose = __MusIntRandom(*ptr++);
  cp->transpose += *ptr++;
  return (ptr);
}

static unsigned char *FrandVolume(channel_t *cp, unsigned char *ptr)
{
  // rand_amount,base
  cp->volume = __MusIntRandom(*ptr++);
  cp->volume += *ptr++;
  return (ptr);
}

static unsigned char *FrandPan(channel_t *cp, unsigned char *ptr)
{
  // rand_amount,base
  cp->pan = __MusIntRandom(*ptr++);
  cp->pan += *ptr++;
  return (ptr);
}

static unsigned char *Fvolume(channel_t *cp, unsigned char *ptr)
{
  cp->volume = *ptr++;
  return (ptr);
}

static unsigned char *Fstartfx(channel_t *cp, unsigned char *ptr)
{
  int	i;
  int	num;
  channel_t *sp;
  long	search_handle;
  long	old_handle;

  num = *ptr++;
  if (num>=0x80)
    num = ((num&0x7f)<<8)+*ptr++;
  old_handle=cp->handle;
  /* change priority so child fx can never overwrite parent */
  search_handle = MusStartEffect2(num,cp->volume,cp->pan,0,cp->priority++);
  cp->priority--; /* change priority back! */
  if (!search_handle)
    return(ptr);
  for(i=0, sp=mus_channels; i<max_channels; i++, sp++)
  {
    if (sp->handle == search_handle)
    {
      sp->handle=cp->handle;
      sp->sample_bank = cp->sample_bank;
    }
  }
  return (ptr);
}

static unsigned char *Fbendrange(channel_t *cp, unsigned char *ptr)
{
  cp->bendrange = (float)(*ptr++)*(1.0/64.0);
  cp->pitchbend_precalc = cp->pitchbend*cp->pitchbend;
  return (ptr);
}

static unsigned char *Fsweep(channel_t *cp, unsigned char *ptr)
{
  cp->sweep_speed = *ptr++;
  return (ptr);
}

command_func_t jumptable[] =
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

static ALMicroTime __MusIntMain(void *node)
{
  int	x;
  channel_t *cp;
  unsigned long calc;
#ifdef SUPPORT_PROFILER
  unsigned long start=osGetCount();
#endif

  for(x=0, cp=mus_channels; x<max_channels; x++, cp++)
  {
    if(cp->pdata == NULL)
      continue;

    if (cp->pending)
      __MusIntFlushPending(cp,x);

    cp->channel_frame+=cp->channel_tempo;
    /* process until note or stop for this frame is found */
    if(cp->length != 0x7fff)
    {
      while (cp->note_end_frame<cp->channel_frame && cp->pdata!=NULL)
	__MusIntGetNewNote(cp, x);
      /* cancel processing if channel stopped */
      if(!cp->pdata)
	continue;
    }
    /* process volume and pitchbend continous data streams */
    if(cp->pvolume && cp->pitchbend_frame<cp->channel_frame)
      __MusIntProcessContinuousVolume(cp);
    if(cp->ppitchbend && cp->pitchbend_frame<cp->channel_frame)
      __MusIntProcessContinuousPitchBend(cp);

    /* process fade down and stop API command */
    if (cp->stopping != -1)
    {
      cp->stopping--;
      if (cp->stopping==-1)
      {
	cp->pdata = Fstop(cp, NULL);      
	if(cp->playing)
	{
	  cp->playing = 0;
#ifdef	NU_SYSTEM
	  alSynStopVoice(&auGlobal.drvr, mus_voices+x);
#else
	  alSynStopVoice(&__am.g.drvr, mus_voices+x);
#endif
	}
      }
    }

    /* process current note to generate sound */
    if(cp->playing)
    {
      float total;
      
      if (cp->env_phase)
	__MusIntProcessEnvelope(cp);
      if (cp->sweep_speed && cp->sweep_frame<cp->channel_frame)
	__MusIntProcessSweep(cp);

      total = cp->freqoffset;
#ifdef SUPPORT_EFFECTS
      if (cp->vib_speed || cp->specialvib_speed)
#else
      if (cp->vib_speed)
#endif
	total += __MusIntProcessVibrato(cp);
      if (cp->wobble_on_speed)
	total += __MusIntProcessWobble(cp);
      if (!cp->pending)
      {
	__MusIntSetPitch(cp, x, total);
	__MusIntSetVolumeAndPan(cp, x);
      }
    }
    /* increment channel counters */
    cp->count= (cp->channel_frame-cp->note_start_frame)>>8;
  }

#ifdef SUPPORT_PROFILER
  _mus_cpu_last = osGetCount()-start;
  if (_mus_cpu_last>_mus_cpu_worst)
    _mus_cpu_worst = _mus_cpu_last;
#endif
  
  /* set to call back next video frame */
  return (mus_next_frame_time);
}

static void __MusIntGetNewNote(channel_t *cp,  int x)
{
  unsigned char *ptr;
  unsigned char command;

  ptr = cp->pdata;
  while(ptr && (command=*ptr)>127)
  {
#ifdef _AUDIODEBUG
    if(command >= Clast)
    {
      osSyncPrintf("PLAYER.C: Channel %d is corrupt (command=%02x)\n",x,command);
      cp->pdata = NULL;
      break;
    }    
#endif
    /* Execute the relevant code for the token.  */
    ptr = (jumptable[command&0x7f].func)(cp, ptr+1);
  }
  cp->pdata = ptr;

  /* new note */
  if(ptr)
  {
    int note;

    cp->last_note = cp->port_base;
    note = *(cp->pdata++);

    if(cp->velocity_on)
      cp->velocity = /*velocity_map[*/*(cp->pdata++)/*]*/;
    else
      cp->velocity = cp->default_velocity;

    if (cp->fixed_length)
    {
      if (!cp->ignore)
	cp->length = cp->fixed_length;
      else
      {
	cp->ignore = 0;
	command = *(cp->pdata++);
	if (command < 128)
	  cp->length = command;
	else
	  cp->length = ((int)(command&0x7f)<<8)+*(cp->pdata++);
      }
    }
    else
    {
      command = *(cp->pdata++);
      if(command < 128)
	cp->length = command;
      else
	cp->length = ((int)(command&0x7f)<<8)+*(cp->pdata++);
    }

    /* set length and timer */
    cp->note_start_frame = cp->note_end_frame;
    cp->note_end_frame += cp->length*256;
    cp->count = 0;
    /* initialise wobble */
    cp->wobble_count = cp->wobble_off_speed;
    cp->wobble_current = 0;

#ifdef SUPPORT_SONGWAVELOOKUP
    /* if wave look up is -1 sample is silent - replace note with a rest */
    if (!cp->pdrums)
    {
      int wave;
      
      wave = cp->wave;
      if (cp->song_addr)
	wave = cp->song_addr->WaveLookup[wave];
      if (wave==-1)
	note = REST;
    }
#endif

    if (note!=REST)
    {
      int wave;
      ptr_bank_t *bank;

      /* get current sample bank */
      bank = cp->sample_bank;

      /* check for drums */
      if(cp->pdrums != NULL)
      {
	cp->wave = (*(cp->pdrums+(note)*4));
	cp->pan = (*(cp->pdrums+(note)*4+2))/2;
	(void) Fdefa(cp, &cp->song_addr->EnvelopeData[(*(cp->pdrums+(note)*4+1))*7]);
	note = (*(cp->pdrums+(note)*4+3));
      }

      /* initialise envelope */
      if(!cp->env_trigger_off)
	__MusIntInitEnvelope(cp);

      /* initialise sweep */
      if (cp->sweep_speed)
	__MusIntInitSweep(cp);

#ifdef _AUDIODEBUG
      /* validate wave number with sample bank */
      if (cp->wave>=bank->count)
      {
	osSyncPrintf("PLAYER.C: Wave number overflow!\n");
	cp->wave = 0;
      }
#endif
      /* get current wave number */
      wave = cp->wave;

#ifdef SUPPORT_SONGWAVELOOKUP
      /* lookup wave in songs lookup table */
      if (cp->song_addr)
	wave = cp->song_addr->WaveLookup[wave];
#endif

      /* start relevant sample if required */
      if(!cp->trigger_off)
      {
	ALWaveTable *wave_addr;

	wave_addr = bank->wave_list[wave];
#ifdef SUPPORT_WAVEMONITORING
	wave_addr->flags++; /* flags isn't needed by ALWaveTable honest! */
#endif
	cp->pending = wave_addr;
	if (cp->playing && cp->old_volume)
	{
	  cp->old_volume = 0;
#ifdef	NU_SYSTEM
	  alSynSetVol(&auGlobal.drvr, mus_voices+x, 0, mus_next_frame_time);
#else
	  alSynSetVol(&__am.g.drvr, mus_voices+x, 0, mus_next_frame_time);
#endif
	}
	else
	  __MusIntFlushPending(cp,x);
      }
      cp->base_note = (float)note+bank->detune[wave];

      /* set reverb level if required */
      if (cp->reverb!=cp->old_reverb)
      {
	unsigned char work;

	work = cp->reverb_base;
	work += ((128-work)*cp->reverb)>>7;
	cp->old_reverb = cp->reverb;
#ifdef	NU_SYSTEM
	alSynSetFXMix(&auGlobal.drvr, mus_voices+x, work);
#else
	alSynSetFXMix(&__am.g.drvr, mus_voices+x, work);
#endif
      }
    }
    else
    {
      /* rest allows previous notes release to finish */
      if (cp->env_phase<4)
      {
	cp->env_phase = 4; /* Start Release */
	cp->release_frame = cp->channel_frame;
	cp->env_count=1;
	cp->release_start_vol = cp->env_current;
      }
    }
  }
  else	/* must have hit a Cstop so stop its voice */
  {
    if(cp->playing)
    {
      cp->playing = 0;
#ifdef	NU_SYSTEM
      alSynSetVol(&auGlobal.drvr, mus_voices+x, 0, mus_next_frame_time);
      alSynStopVoice(&auGlobal.drvr, mus_voices+x);
#else
      alSynSetVol(&__am.g.drvr, mus_voices+x, 0, mus_next_frame_time);
      alSynStopVoice(&__am.g.drvr, mus_voices+x);
#endif
    }
  }
}

static void __MusIntFlushPending(channel_t *cp, int x)
{
  if (cp->playing)
#ifdef	NU_SYSTEM
    alSynStopVoice(&auGlobal.drvr, mus_voices+x);
#else
    alSynStopVoice(&__am.g.drvr, mus_voices+x);
#endif
  cp->playing = 1;
  /* start sample */
#ifdef	NU_SYSTEM
  alSynStartVoice(&auGlobal.drvr, mus_voices+x, cp->pending);
#else
  alSynStartVoice(&__am.g.drvr, mus_voices+x, cp->pending);
#endif
  cp->pending = NULL;
}

static void __MusIntSetVolumeAndPan(channel_t *cp, int x)
{
  u32	volume;

  /* process volume */
  volume = ((u32)(cp->volume)*(u32)(cp->env_current)*(u32)(cp->velocity)*(u32)(cp->volscale))>>13;
  if (volume>32767)
    volume = 32767;

  if (!cp->IsFX)
    volume *= mus_master_volume_songs;
  else
    volume *= mus_master_volume_effects;
  volume >>= 15;
  if (cp->stopping != -1)
    volume = (volume*cp->stopping)/cp->stopping_speed;

  if (volume!=cp->old_volume)
  {
    cp->old_volume = volume;
#ifdef	NU_SYSTEM
    alSynSetVol(&auGlobal.drvr, mus_voices+x, volume, mus_next_frame_time);
#else
    alSynSetVol(&__am.g.drvr, mus_voices+x, volume, mus_next_frame_time);
#endif
  }

  /* process pan */
  if (cp->pan!=cp->old_pan)
  {
    volume = ((cp->pan*cp->panscale)>>7)&0x7f;
    cp->old_pan = volume;
#ifdef	NU_SYSTEM
    alSynSetPan(&auGlobal.drvr, mus_voices+x, volume);
#else
    alSynSetPan(&__am.g.drvr, mus_voices+x, volume);
#endif
  }
}

static void __MusIntSetPitch(channel_t *cp, int x, float offset)
{
  float   frequency, temp;
  int     calc;

  /* base frequency */
  frequency = cp->base_note;

  /* incorporate portamento */
  if(cp->port != 0)	
  {
    if(cp->count <= cp->port)
    {
      temp = (frequency - cp->last_note)/(float)(cp->port);
      temp *= (float)cp->count;
      frequency = cp->last_note + temp;
    }
    cp->port_base = frequency;
  }  
  /* get transpose value */
  calc = cp->transpose*(1-cp->ignore_transpose);
  cp->ignore_transpose = 0;

  /* incorporate offsets */
  frequency += offset+cp->pitchbend_precalc+(float)calc;

  /* only output it if it's changed! */
  if (frequency==cp->old_frequency)
    return;
  cp->old_frequency = frequency;  
  frequency = __MusIntPowerOf2(frequency*(1.0/12.0));
#ifdef _AUDIODEBUG
  if(frequency <= 0)
  {
    osSyncPrintf("PLAYER.C: frequency underflow.\n");
    frequency= 1.0;  
  }
#endif
  if(frequency > 2.0)
  {
    frequency = 2.0;
    cp->velocity = 0;    
  }  
#ifdef	NU_SYSTEM
  alSynSetPitch(&auGlobal.drvr, mus_voices+x, frequency);
#else
  alSynSetPitch(&__am.g.drvr, mus_voices+x, frequency);
#endif
}

static void __MusIntInitEnvelope(channel_t *cp)
{   
  if(cp->length != 0x7fff)
  {
    if(cp->cutoff != 0) /* release time is from start of note */
      cp->release_frame = cp->note_start_frame + (cp->cutoff<<8);
    else		/* release time is from end of note */
      cp->release_frame = cp->note_end_frame - (cp->endit<<8);
  }
  else
  {
    cp->release_frame = 0x7fffffff; /* never release (well not this century) */
  }    
  cp->env_current = cp->env_init_vol;
  cp->env_count = cp->env_speed;
  cp->env_phase = 1;
}

static void __MusIntProcessEnvelope(channel_t *cp)
{
  float	temp;
  int env_phase_count;
 
  if(cp->channel_frame >= cp->release_frame && cp->env_phase<4)
  {
    /* Start Release */
    cp->env_phase = 4;
    cp->env_count=1;
    cp->release_start_vol = cp->env_current;
  }
  cp->env_count--;
  if(!cp->env_count)
  {
    cp->env_count = cp->env_speed;

    switch(cp->env_phase)
    {
	case 1:
	  {
	    /* Attack */
            env_phase_count = (((cp->channel_frame - cp->note_start_frame)>>8)*cp->env_speed_calc)>>10;
	    if (env_phase_count<cp->env_attack_speed)
	    {
	      cp->env_current = (int)cp->env_init_vol+(int)((float)(cp->env_attack_calc*(float)env_phase_count));
	      return;
	    }
	    else
	    {
	      cp->env_phase++;
	      cp->env_current = cp->env_max_vol;
	      return;
	    }
	  }
	case 2:
	  {
	    /* Decay */
	    env_phase_count = ((((cp->channel_frame - cp->note_start_frame)>>8) - cp->env_attack_speed)*cp->env_speed_calc)>>10;
	    if (env_phase_count<cp->env_decay_speed)
	    {
	      cp->env_current = (int)cp->env_max_vol+(int)((float)(cp->env_decay_calc*(float)env_phase_count));
	      return;
	    }
	    else
	    {
	      cp->env_phase++;
	      cp->env_current = cp->env_sustain_vol;
	      return;
	    }
	  }
	case 3:
	  {
	    /* Sustain */
	    return;
	  }
	case 4:
	  {
	    /* Do Release */
            env_phase_count = (((cp->channel_frame - cp->release_frame)>>8)*cp->env_speed_calc)>>10;
	    if (env_phase_count < cp->env_release_speed)
	    {
	      cp->env_current = (int)cp->release_start_vol-(int)((float)(cp->env_release_calc*(float)env_phase_count*(float)cp->release_start_vol));
	      return;
	    }
	    else
	    {
	      cp->env_phase++;
	      cp->env_current = 0;
	      return;
	    }
	  }
    }	
  }
}

static void __MusIntInitSweep(channel_t *cp)
{
  cp->sweep_frame = cp->note_start_frame;
  cp->sweep_timer = 0;
  cp->sweep_dir = cp->pan&0x40;
}

void __MusIntProcessSweep(channel_t *cp)
{
  unsigned long calc;

  do
  {
    cp->sweep_frame += 256;
    calc = cp->sweep_timer+cp->sweep_speed;
    if (calc<64)
    {
      cp->sweep_timer=calc;
      continue;
    }
    cp->sweep_timer=calc&63;
    calc>>=6;
    if (!cp->sweep_dir)
    {
      cp->pan += calc;
      if (cp->pan>0x7f)
      {
	cp->pan=0x7f;
	cp->sweep_dir = 1;
      }
    }
    else
    {
      cp->pan -= calc;
      if (cp->pan>=0x80 || cp->pan==0)
      {
	cp->pan=0;
	cp->sweep_dir = 0;
      }    
    }
  } while (cp->sweep_frame<cp->channel_frame);
}

static float __MusIntProcessWobble(channel_t *cp)
{
  cp->wobble_count--;
  if(!cp->wobble_count)
  {
    if(cp->wobble_current==0)
    {
      cp->wobble_current=cp->wobble_amount;
      cp->wobble_count=cp->wobble_on_speed;
    }
    else
    {
      cp->wobble_current=0;
      cp->wobble_count=cp->wobble_off_speed;
    }
  }
  return ((float)cp->wobble_current);
}

static float __MusIntProcessVibrato (channel_t *cp)
{
  int temp;
  float temp1;

#ifdef SUPPORT_EFFECTS
  if (cp->specialvib_speed)
  {
    if((temp = cp->count-cp->specialvib_delay) > 0)
    {
      temp1 = sinf( (float) temp/cp->specialvib_speed*2*3.1415926)*cp->specialvib_amount/*/50*/;
      cp->vibrato = temp1;
    }    
  }
  else
  {
#endif

  temp = cp->count-cp->vib_delay ;
  if(temp > 0)
  {
    temp1 = sinf( (float) temp/cp->vib_speed*2*3.1415926)*cp->vib_amount/*/50*/;
    cp->vibrato = temp1;
  }
#ifdef SUPPORT_EFFECTS
  }
#endif
  return (cp->vibrato);
}

static void __MusIntProcessContinuousVolume(channel_t *cp)
{
  unsigned char work_vol;

  do
  {    
    cp->volume_frame += 256;
    cp->cont_vol_repeat_count--;
    if(cp->cont_vol_repeat_count == 0)   /* already repeating? */
    {
      work_vol = *(cp->pvolume++);
      if(work_vol > 127)		    /* does count follow? */
      {
	/* yes  volume is followed by run length data */
#ifdef SUPPORT_EFFECTS
	cp->last_volume = 
#endif
	cp->volume = work_vol &0x7f;
	work_vol = *(cp->pvolume++);
	if(work_vol > 127)
	{
	  cp->cont_vol_repeat_count = ((int)(work_vol&0x7f)*256);
	  cp->cont_vol_repeat_count += (int)*(cp->pvolume++)+2;
	}
	else
	  cp->cont_vol_repeat_count = (int)work_vol+2;
      }
      else
      {
#ifdef SUPPORT_EFFECTS
	cp->last_volume = 
#endif
	cp->volume = work_vol;
	cp->cont_vol_repeat_count=1;
      }
    }
  } while (cp->volume_frame < cp->channel_frame);

#ifdef SUPPORT_EFFECTS
  __MusIntSpecialVol(cp);
#endif
}

static void __MusIntProcessContinuousPitchBend(channel_t *cp)
{
  unsigned char work_pb;

  do 
  {    
    cp->pitchbend_frame += 256;
    cp->cont_pb_repeat_count--;
    if(cp->cont_pb_repeat_count == 0)   /* already repeating? */
    {
      work_pb = *(cp->ppitchbend++);
      if(work_pb > 127)		    /* does count follow? */
      {
	/* yes  pitchbend is followed by run length data */
	cp->pitchbend = ((float)(work_pb &0x7f))-64.0;
	cp->pitchbend_precalc = cp->pitchbend*cp->bendrange;
	work_pb = *(cp->ppitchbend++);
	if(work_pb > 127)
	{
	  cp->cont_pb_repeat_count = ((int)(work_pb&0x7f)*256);
	  cp->cont_pb_repeat_count += (int)*(cp->ppitchbend++)+2;
	}
	else
	  cp->cont_pb_repeat_count = (int)work_pb+2;
      }
      else
      {
	cp->pitchbend = ((float)work_pb)-64.0;
	cp->pitchbend_precalc = cp->pitchbend*cp->bendrange;
	cp->cont_pb_repeat_count=1;
      }
    }
  } while (cp->pitchbend_frame < cp->channel_frame);

#ifdef SUPPORT_EFFECTS
  __MusIntSpecialPitchBend(cp);
#endif
}

static float __MusIntPowerOf2(float x)
{
  float x2;
  
  if(x == 0)
    return 1;
  
  if(x > 0)
  {
    x2 = x*x;
    return (1+(x*.693147180559945)
	    +(x2*.240226506959101)
	    +(x2*x* 5.55041086648216E-02)
	    +(x2*x2* 9.61812910762848E-03)
	    +(x2*x2*x* 1.33335581464284E-03)
	    +(x2*x2*x2* 1.54035303933816E-04)
	    );
  }	
  else	
  {
    x = -x;
    x2 = x*x;
    return (1/(1+(x*.693147180559945)
	       +(x2*.240226506959101)
	       +(x2*x* 5.55041086648216E-02)
	       +(x2*x2* 9.61812910762848E-03)
	       +(x2*x2*x* 1.33335581464284E-03)
	       +(x2*x2*x2* 1.54035303933816E-04)
	       ));
  }
}

static void __MusIntRemapPtrBank(char *pptr, char *wptr)
{
  int i,bankLen;
  u32 l;
  ptr_bank_t *ptrfile_addr;
  unsigned char *chardetune, charwork;
  float *floatdetune, floatwork;
  unsigned long base;

  ptrfile_addr = (ptr_bank_t *)pptr;
  /* return if already remapped */
  if (ptrfile_addr->flags&PTRFLAG_REMAPPED)
    return;
  /* set remapped flag */
  ptrfile_addr->flags |= PTRFLAG_REMAPPED;

  /* remap first set of pointers */
  __MusIntRemapPtrs(&ptrfile_addr->basenote, pptr, 3);
  /* remap wave list pointers */
  __MusIntRemapPtrs(&ptrfile_addr->wave_list[0], pptr, ptrfile_addr->count);

  /* now calculate detune values and remap wave list */
  for (i=0; i<ptrfile_addr->count; i++)
  {
    floatdetune = &ptrfile_addr->detune[i];
    chardetune = (unsigned char *)floatdetune;
    charwork = *chardetune;
    floatwork = U8_TO_FLOAT(charwork);
    *floatdetune = floatwork/100.0;

    charwork = ptrfile_addr->basenote[i]-BASEOFFSET;
    floatwork = U8_TO_FLOAT(charwork);
    *floatdetune += floatwork;

    /* remap pointers inside ALWaveTable structures */
    if(!ptrfile_addr->wave_list[i]->flags)
    {
      base = (unsigned long)ptrfile_addr->wave_list[i]->base;
      if ((base&0xff000000)!=0xff000000) /* not n64dd sample */
      {
	base += (unsigned long)wptr;
	ptrfile_addr->wave_list[i]->base = (u8 *)base;	
      }
      ptrfile_addr->wave_list[i]->flags = 1;
      
      if(ptrfile_addr->wave_list[i]->waveInfo.adpcmWave.loop)			
	ptrfile_addr->wave_list[i]->waveInfo.adpcmWave.loop = (ALADPCMloop *)((u32)(ptrfile_addr->wave_list[i]->waveInfo.adpcmWave.loop)+(u32)(pptr));
      if(ptrfile_addr->wave_list[i]->type == AL_ADPCM_WAVE)			
	ptrfile_addr->wave_list[i]->waveInfo.adpcmWave.book = (ALADPCMBook *)((u32)(ptrfile_addr->wave_list[i]->waveInfo.adpcmWave.book)+(u32)(pptr));			
    }	
  }
  /* flush data cache so the new sample pointers are visible to the RSP */
  osWritebackDCacheAll();
}

static int __MusIntRandom (int range)
{
  long temp;
  int x;
  float f;
  
  for (x=0;x<8;x++)
  {
    temp = mus_random_seed & 0x48000000;
    mus_random_seed = mus_random_seed<<1;
    if (temp==0x48000000 || temp == 0x08000000)
      mus_random_seed = mus_random_seed | 1;
  }
  f = (float)(mus_random_seed)/(1<<16);
  f /= (1<<16);
  return ((int)((float)(range)*f));
}	

static void __MusIntInitialiseChannel(channel_t *cp)
{
  unsigned char old_playing, *work_ptr;
  int i;

  /* disable channel processing 1st!!! */
  cp->pdata = NULL;
  old_playing = cp->playing;

  /* zero out channel */
  work_ptr = (unsigned char *)cp;
  for (i=0; i<sizeof(channel_t); i++)
    *work_ptr++ = 0;

  /* set none zero values */
  cp->old_volume    	= 0xffff;
  cp->old_reverb    	= 0xff;
  cp->old_pan       	= 0xff;
  cp->old_frequency 	= 99.9;

  cp->channel_tempo = cp->channel_tempo_save = 96*256/mus_vsyncs_per_second;

  cp->length        	= 1;

  cp->velocity_on   	= 0; /* now defaults to off! (was 1) */;
  cp->default_velocity 	= /*velocity_map[*/127/*]*/;
  cp->volume		= 127;
  cp->bendrange		= 2*(1.0/64.0);
  cp->pan		= 64;

  cp->cont_vol_repeat_count = 1;
  cp->cont_pb_repeat_count = 1;

  cp->stopping  = -1;

  /* new volume and pan scales */
  cp->volscale = 0x80;
  cp->panscale = 0x80;
  cp->temscale = 0x80;
  
  /* setup a default envelope */
  cp->env_speed = 1;
  cp->env_attack_speed = 1;
  cp->env_attack_calc = 1.0F;
  cp->env_max_vol = 127;
  cp->env_decay_speed = 255;
  cp->env_decay_calc = 1.0/255.0;
  cp->env_sustain_vol = 127;
  cp->env_release_speed = 15;
  cp->env_release_calc = 1.0/15.0;

  /* set current sample bank */
  cp->sample_bank = mus_init_bank;

  /* restore channel status flag */
  cp->playing = old_playing;
}

static int __MusIntFindChannel(song_t *addr, int song_chan)
{
  channel_t *cp;
  int i,current,current_channel;

  /* 1st scan for empty channel */
  for (i=0, cp=mus_channels; i<max_channels; i++, cp++)
    if (!cp->pdata)
      return (i);

  /* 2nd scan for sfx channel to override */
  for (i=0, cp=mus_channels, current=0x7fffffff, current_channel=-1; i<max_channels; i++, cp++)
  {
    if (cp->IsFX)
    {
      if (cp->priority<=current)
      {
	current=cp->priority;
	current_channel=i;
      }
    }
  }
  if (current_channel!=-1)
    return (current_channel);

  /* 3rd scan for song channel (not this song) */
  for (i=0, cp=mus_channels; i<max_channels; i++)
    if (!cp->IsFX && cp->song_addr!=addr)
      return (i);

  /* 4th scan for same tune, same channel */
  for (i=0, cp=mus_channels; i<max_channels; i++, cp++)
    if (cp->song_addr==addr && addr->ChannelData[song_chan]==cp->pbase)
      return (i);

  /* get any channel */
  return (song_chan%max_channels);
}

static void __MusIntMemSet(void *dest, unsigned char value, unsigned long length)
{
  unsigned char *a;

  a = dest;
  while (length--)
    *a++=value;
}

static void __MusIntRemapPtrs(void *addr, void *offset, int count)
{
  unsigned long *dest, add;
  int i;

  dest = (unsigned long *)addr;
  add = (unsigned long)offset;
  for (i=0; i<count; i++)
    if (dest[i])
      dest[i]+=add;
}

static unsigned long __MusIntStartEffect (channel_t *cp, int number, int volume, int pan, int priority)
{
  u8 old_playing;

  __MusIntInitialiseChannel(cp);

  cp->IsFX	= number;
  cp->volscale	= volume;
  cp->panscale	= pan;
  cp->handle	= mus_current_handle++;
  cp->priority	= priority;
  
  /* pdata must be set last to avoid processing clash */
  cp->pdata = cp->pbase = mus_effects[number];

  return (cp->handle);
}
