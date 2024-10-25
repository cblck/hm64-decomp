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
STATIC int             max_channels;              /* number of channels        */
//STATIC ALVoice         *mus_voices;               /* audio library voices      */
STATIC channel_t       *mus_channels;             /* music player channels     */
//STATIC unsigned char   **mus_effects;             /* address of sound effects  */
STATIC int             *mus_priority;             /* address of sfx prioritys  */
STATIC int             mus_vsyncs_per_second;     /* video refresh rate        */
//STATIC ALMicroTime     mus_next_frame_time;	  /* time until next frame     */
STATIC unsigned short  mus_master_volume_effects; /* sound effect master value */
STATIC unsigned short  mus_master_volume_songs;   /* song master volume        */
STATIC unsigned long   mus_current_handle;        /* current handle number     */
//STATIC long            mus_random_seed;           /* random number seed value  */
STATIC ptr_bank_t      *mus_init_bank;		  /* sample bank to initialise */
STATIC ptr_bank_t      *mus_default_bank;	  /* sample bank default       */

/* music player control flag */
//unsigned long __muscontrol_flag;


/* C files included directly (to avoid lots of global variables in the library)*/
/* these files are separated just for readability                              */
//#include "player_commands.c"
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

void MusSetMasterVolume(unsigned long flags, int volume)
{
  if (flags&MUSFLAG_EFFECTS)
    mus_master_volume_effects = volume;
  if (flags&MUSFLAG_SONGS)
    mus_master_volume_songs = volume;
}

unsigned long MusStartSong(void *addr)
{
  song_t *song_addr;
  unsigned long handle, *work_addr;
  int i, count;
  channel_t *cp;


  song_addr = addr;
  work_addr = addr;
  count = *work_addr++;
#ifdef SUPPORT_SONGWAVELOOKUP
  work_addr++;
#endif
  /* remap song if necessary */
  if (*work_addr<0x400)
  {
    /* convert main offsets to pointers */
    __MusIntRemapPtrs(work_addr, addr, SONGTPTRS);
    /* convert channel, volume and pitch bend offsets to pointers */
    __MusIntRemapPtrs(song_addr->ChannelData,    addr, count);
    __MusIntRemapPtrs(song_addr->VolumeData,     addr, count);
    __MusIntRemapPtrs(song_addr->PitchBendData,  addr, count);
  }

  /* get next handle */
  handle = mus_current_handle++;

  for(i=0; i<count; i++)
  {
    if(song_addr->ChannelData[i] != NULL)
    {
      cp=mus_channels+__MusIntFindChannel(song_addr,i);
      __MusIntInitialiseChannel(cp);
      cp->song_addr = song_addr;
      cp->pvolume = cp->pvolumebase = song_addr->VolumeData[i];
      cp->ppitchbend = cp->ppitchbendbase = song_addr->PitchBendData[i];
      /* pdata must be set last to avoid processing clash */
      cp->pdata = cp->pbase = song_addr->ChannelData[i];
      cp->handle = handle;

    }
  }
  return (handle);
}

unsigned long MusStartEffect(int number)
{
  channel_t *cp, *current_cp;
  int i, priority, current_priority;
	
  priority = mus_priority[number];
  current_priority = priority+1;
  for(i=0, cp=mus_channels; i<max_channels; i++, cp++)
  {
    if (cp->pdata==NULL)
      return (__MusIntStartEffect(cp, number, 0x80, 0x80, priority));
    if (cp->IsFX && cp->priority<current_priority)
    {
      current_priority = cp->priority;
      current_cp = cp;
    }
  }
  if (current_priority<priority)
      return (__MusIntStartEffect(current_cp, number, 0x80, 0x80, priority));
  return (0);
}

unsigned long MusStartEffect2(int number, int volume, int pan , int restartflag, int priority)
{
  channel_t *cp, *current_cp;
  int i, current_priority;
	
  if (priority==-1)
    priority = mus_priority[number];

  if (restartflag)
  {
    for(i=0, cp=mus_channels; i<max_channels; i++, cp++)
      if (cp->IsFX == number)
	return (__MusIntStartEffect(cp, number, volume, pan, priority));
  }
  current_priority = priority+1;
  for(i=0, cp=mus_channels; i<max_channels; i++, cp++)
  {
    if (cp->pdata==NULL)
      return (__MusIntStartEffect(cp, number, volume, pan, priority));
    if (cp->IsFX && cp->priority<current_priority)
    {
      current_priority = cp->priority;
      current_cp = cp;
    }
  }
  if (current_priority<priority)
      return (__MusIntStartEffect(current_cp, number, volume, pan, priority));
  return (0);
}

void MusStop(unsigned long flags, int speed)
{
  int i, speed2;
  channel_t *cp;

#ifdef _AUDIODEBUG
  if (speed<0)
    osSyncPrintf("PLAYER.C: Stop all function called with invalid speed setting.\n");
  speed = 0;
#endif
  speed2 = speed ? speed : 1;
  for (i=0, cp=mus_channels; i<max_channels; i++, cp++)
  {
    if ((cp->IsFX && flags&MUSFLAG_EFFECTS) || (!cp->IsFX && flags&MUSFLAG_SONGS))
    {
      cp->stopping = speed;
      cp->stopping_speed = speed2;
    }
  }
}

int MusAsk(unsigned long flags)
{
  int i, count;
  channel_t *cp;

  for (i=0, cp=mus_channels, count=0; i<max_channels; i++, cp++)
  {
    if (cp->pdata)
    {
      if ((cp->IsFX && flags&MUSFLAG_EFFECTS) || (!cp->IsFX && flags&MUSFLAG_SONGS))
	count++;
    }
  }
  return (count);
}

int MusHandleStop(unsigned long handle, int speed)
{
  int i,speed2, count;
  channel_t *cp;

  if (!handle)
    return(0);

#ifdef _AUDIODEBUG
  if (speed<0)
    osSyncPrintf("PLAYER.C: MusHandleStop called with invalid speed setting.\n");
  speed = 0;
#endif
  speed2 = speed ? speed : 1;
  for (i=0, cp=mus_channels, count=0; i<max_channels; i++, cp++)
  {
    if (cp->handle==handle)
    {
      cp->stopping = speed;
      cp->stopping_speed = speed2;
      count++;
    }
  }
  return (count);
}

int MusHandleAsk(unsigned long handle)
{
  channel_t *cp;
  int i, count;

  if (!handle)
    return (0);
  
  for(i=0, cp=mus_channels, count=0; i<max_channels; i++, cp++)
    if (cp->handle==handle)
      count++;
  return(count);
}

int MusHandleSetVolume(unsigned long handle, int volume)
{
  channel_t *cp;
  int i, count;

  if (!handle)
    return (0);

  for(i=0, cp=mus_channels, count=0; i<max_channels; i++, cp++)
  {
    if (cp->handle==handle)
    {
      cp->volscale = volume;
      count++;
    }
  }
  return (count);
}

int MusHandleSetPan (unsigned long handle, int pan)
{
  channel_t *cp;
  int i, count;

  if (!handle)
    return (0);

  for(i=0, cp=mus_channels, count=0; i<max_channels; i++, cp++)
  {
    if (cp->handle==handle)
    {
      cp->panscale = pan;
      cp->old_pan = 0xff;
      count++;
    }
  }
  return (count);
}

int MusHandleSetFreqOffset(unsigned long handle, float offset)
{
  channel_t *cp;
  int i, count;

  if (!handle)
    return (0);

  for(i=0, cp=mus_channels, count=0; i<max_channels; i++, cp++)
  {
    if (cp->handle==handle)
    {
      cp->freqoffset = offset+cp->distort;
      count++;
    }
  }
  return (count);
}

int MusHandleSetTempo(unsigned long handle, int tempo)
{
  channel_t *cp;
  int i, count;

  if (!handle)
    return (0);

  if (tempo<1)
    tempo=1;
  else if (tempo>256)
    tempo=256;

  for(i=0, cp=mus_channels, count=0; i<max_channels; i++, cp++)
  {
    if (cp->handle==handle)
    {
      cp->temscale = tempo;
      cp->channel_tempo = (cp->channel_tempo_save*tempo)>>7;
      count++;
    }
  }
  return (count);
}

int MusHandleSetReverb(unsigned long handle, int reverb)
{
  channel_t *cp;
  int i, count;

  if (!handle)
    return (0);

  if (reverb<0)
    reverb=0;
  else if (reverb>127)
    reverb=127;

  for(i=0, cp=mus_channels, count=0; i<max_channels; i++, cp++)
  {
    if (cp->handle==handle)
    {
      cp->reverb_base = reverb;
      cp->old_reverb = 0xff;
      count++;
    }
  }
  return (count);
}

void MusBankInitialize(void *pbank, void *wbank)
{
  __MusIntRemapPtrBank(pbank, wbank);
}

unsigned long MusBankStartSong(void *ipbank, void *addr)
{
  unsigned long handle;
  ptr_bank_t *pptr;

  if (ipbank)
  {
    pptr = (ptr_bank_t *)ipbank;
    if (pptr->flags&PTRFLAG_REMAPPED)
      mus_init_bank = pptr;    
#ifdef _AUDIODEBUG
    else
      osSyncPrintf("PLAYER_API.C: Calling MusBankStartSong with invalid pointer bank.\n");
#endif
  }
  handle = MusStartSong(addr);
  mus_init_bank = mus_default_bank;
  return (handle);
}

unsigned long MusBankStartEffect(void *ipbank, int number)
{
  unsigned long handle;
  ptr_bank_t *pptr;

  if (ipbank)
  {
    pptr = (ptr_bank_t *)ipbank;
    if (pptr->flags&PTRFLAG_REMAPPED)
      mus_init_bank = pptr;    
  }
  
  handle = MusStartEffect(number);
  mus_init_bank = mus_default_bank;
  return (handle);
}

unsigned long MusBankStartEffect2(void *ipbank, int number, int volume, int pan , int restartflag, int priority)
{
  unsigned long handle;
  ptr_bank_t *pptr;

  if (ipbank)
  {
    pptr = (ptr_bank_t *)ipbank;
    if (pptr->flags&PTRFLAG_REMAPPED)
      mus_init_bank = pptr;    
  }
  
  handle = MusStartEffect2(number, volume, pan, restartflag, priority);
  mus_init_bank = mus_default_bank;
  return (handle);
}

void *MusHandleGetPtrAddr(int handle)
{
  channel_t *cp;
  int i, count;

  if (!handle)
    return (0);

  for(i=0, cp=mus_channels, count=0; i<max_channels; i++, cp++)
    if (cp->handle==handle)
      return (cp->sample_bank);
  return (0);
}

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
