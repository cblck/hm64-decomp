int MusInitialize(musConfig *config)
{
#ifdef	NU_SYSTEM
  ALVoiceConfig vc;  
  int i;

  /* main control flag */
  __muscontrol_flag = config->control_flag;

  /* copy player settings first */
  max_channels = config->channels;
  mus_effects  = config->fxs;
  mus_priority = config->priority;

  /* get video refresh rate */
  if (osTvType==0)
    mus_vsyncs_per_second = 50;
  else
    mus_vsyncs_per_second = 60;
  mus_next_frame_time = 1000000/mus_vsyncs_per_second;

  /* claim and clear memory for syn voices and mus channels */
  mus_voices = alHeapAlloc(&nuAuHeap, 1, max_channels*sizeof(ALVoice));
  mus_channels = alHeapAlloc(&nuAuHeap, 1, max_channels*sizeof(channel_t));
  __MusIntMemSet(mus_voices,0,max_channels*sizeof(ALVoice));
  __MusIntMemSet(mus_channels,0,max_channels*sizeof(channel_t));

  /* set volumes to maxiumum level */
  MusSetMasterVolume(MUSFLAG_EFFECTS|MUSFLAG_SONGS, 0x7fff);
  /* initialise player vars */
  mus_current_handle = 1;
  mus_random_seed = 0x12345678;

  /*
   * Sign into the synthesis driver so that the next time
   * alAudioFrame is called, the __MusIntMain function will be
   * called.
   */

  plr_player.next       = NULL;
  plr_player.handler    = __MusIntMain;
  plr_player.clientData = &plr_player;
  alSynAddPlayer(&auGlobal.drvr, &plr_player);
  
  for(i=0; i<max_channels; i++)
  {    
    mus_channels[i].playing = 0;
    __MusIntInitialiseChannel(&mus_channels[i]);
    
    vc.unityPitch = 0;
    vc.priority = config->thread_priority;
    vc.fxBus = 0;
    
    alSynAllocVoice(&auGlobal.drvr, &mus_voices[i], &vc);
  }
  return (nuAuHeap.cur-nuAuHeap.base);

#else

  ALVoiceConfig vc;  
  ALSynConfig c;
  amConfig amc;
  int i;

  /* main control flag */
  __muscontrol_flag = config->control_flag;

  /* copy player settings first */
  max_channels = config->channels;
  mus_effects  = config->fxs;
  mus_priority = config->priority;

  /* get video refresh rate */
  if (osTvType==0)
    mus_vsyncs_per_second = 50;
  else
    mus_vsyncs_per_second = 60;
  mus_next_frame_time = 1000000/mus_vsyncs_per_second;

  /* initialise heap */
  __MusIntMemSet(config->heap, 0, config->heap_length);
  alHeapInit(&heap, config->heap, config->heap_length);

  /* claim and clear memory for syn voices and mus channels */
  mus_voices = alHeapAlloc(&heap, 1, max_channels*sizeof(ALVoice));
  mus_channels = alHeapAlloc(&heap, 1, max_channels*sizeof(channel_t));
  __MusIntMemSet(mus_voices,0,max_channels*sizeof(ALVoice));
  __MusIntMemSet(mus_channels,0,max_channels*sizeof(channel_t));

  /* initialse samples */
  __MusIntRemapPtrBank(config->ptr, config->wbk);
  mus_default_bank = mus_init_bank = (ptr_bank_t *)config->ptr;

  /*
   * Create the Audio Manager
   */

  c.maxVVoices = max_channels;
  c.maxPVoices = max_channels;
  c.maxUpdates = config->syn_updates;
  c.dmaproc    = 0;                  /* audio mgr will fill this in */
  c.fxType     = AL_FX_BIGROOM;
  c.outputRate = 0;                  /* audio mgr will fill this in */
  c.heap       = &heap;

  amc.outputRate = config->syn_output_rate;
  amc.framesPerField = config->syn_retraceCount;
  amc.maxACMDSize = config->syn_rsp_cmds;

  amCreateAudioMgr(config->sched, &c, config->thread_priority, &amc, 
		   config->syn_num_dma_bufs, config->syn_dma_buf_size, 
		   mus_vsyncs_per_second);

  /* set volumes to maxiumum level */
  MusSetMasterVolume(MUSFLAG_EFFECTS|MUSFLAG_SONGS, 0x7fff);
  /* initialise player vars */
  mus_current_handle = 1;
  mus_random_seed = 0x12345678;

  /*
   * Sign into the synthesis driver so that the next time
   * alAudioFrame is called, the __MusIntMain function will be
   * called.
   */

  plr_player.next       = NULL;
  plr_player.handler    = __MusIntMain;
  plr_player.clientData = &plr_player;
  alSynAddPlayer(&__am.g.drvr, &plr_player);
  
  for(i=0; i<max_channels; i++)
  {    
    mus_channels[i].playing = 0;
    __MusIntInitialiseChannel(&mus_channels[i]);
    
    vc.unityPitch = 0;
    vc.priority = config->thread_priority;
    vc.fxBus = 0;
    
    alSynAllocVoice(&__am.g.drvr, &mus_voices[i], &vc);
  }
  return (heap.cur-heap.base);

#endif	/* NU_SYSTEM */
}	

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

#ifdef SUPPORT_SONGWAVELOOKUP
int MusHandleWaveCount(int handle)
{
	channel_t* cp;
	int i, count;

	if (!handle)
		return (0);

	for (i = 0, cp = mus_channels, count = 0; i < max_channels; i++, cp++)
	{
		if (cp->handle == handle)
		{
			if (cp->song_addr)
				return (cp->song_addr->number_of_waves);
		}
	}
	return (0);
}
#endif

#ifdef SUPPORT_SONGWAVELOOKUP
short* MusHandleWaveAddress(int handle)
{
	channel_t* cp;
	int i, count;

	if (!handle)
		return (NULL);

	for (i = 0, cp = mus_channels, count = 0; i < max_channels; i++, cp++)
	{
		if (cp->handle == handle)
		{
			if (cp->song_addr)
				return (cp->song_addr->WaveLookup);
		}
	}
	return (NULL);
}
#endif

