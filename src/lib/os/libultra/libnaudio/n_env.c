#include "common.h"

/*====================================================================
 *
 * Copyright 1993, Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Silicon Graphics,
 * Inc.; the contents of this file may not be disclosed to third
 * parties, copied or duplicated in any form, in whole or in part,
 * without the prior written permission of Silicon Graphics, Inc.
 *
 * RESTRICTED RIGHTS LEGEND:
 * Use, duplication or disclosure by the Government is subject to
 * restrictions as set forth in subdivision (c)(1)(ii) of the Rights
 * in Technical Data and Computer Software clause at DFARS
 * 252.227-7013, and/or in similar or successor clauses in the FAR,
 * DOD or NASA FAR Supplement. Unpublished - rights reserved under the
 * Copyright Laws of the United States.
 *====================================================================*/

#include "n_synthInternals.h"
#include <os.h>
#include <stdio.h>
#include <math.h>

#ifdef _DEBUG
#include <assert.h>
#endif

#ifdef AUD_PROFILE
extern u32 cnt_index, env_num, env_cnt, env_max, env_min, lastCnt[];
extern u32 rate_num, rate_cnt, rate_max, rate_min;
extern u32 vol_num, vol_cnt, vol_max, vol_min;
#endif

#define N_EQPOWER_LENGTH 128

s16 n_eqpower[N_EQPOWER_LENGTH] = {
    32767,  32764,  32757,  32744,  32727,  32704,
    32677,  32644,  32607,  32564,  32517,  32464,
    32407,  32344,  32277,  32205,  32127,  32045,
    31958,  31866,  31770,  31668,  31561,  31450,
    31334,  31213,  31087,  30957,  30822,  30682,
    30537,  30388,  30234,  30075,  29912,  29744,
    29572,  29395,  29214,  29028,  28838,  28643,
    28444,  28241,  28033,  27821,  27605,  27385,
    27160,  26931,  26698,  26461,  26220,  25975,
    25726,  25473,  25216,  24956,  24691,  24423,
    24151,  23875,  23596,  23313,  23026,  22736,
    22442,  22145,  21845,  21541,  21234,  20924,
    20610,  20294,  19974,  19651,  19325,  18997,
    18665,  18331,  17993,  17653,  17310,  16965,
    16617,  16266,  15913,  15558,  15200,  14840,
    14477,  14113,  13746,  13377,  13006,  12633,
    12258,  11881,  11503,  11122,  10740,  10357,
    9971,   9584,   9196,   8806,   8415,   8023,
    7630,   7235,   6839,   6442,   6044,   5646,
    5246,   4845,   4444,   4042,   3640,   3237,
    2833,   2429,   2025,   1620,   1216,    810,
    405,      0
};

#ifndef N_MICRO
extern	f64	__pow(f64, f64);
extern f64 _frexpf(f64 value, s32* eptr);
extern f64 _ldexpf(f64 in, s32 ex);
#endif

/*
 * prototypes for private enveloper functions
 */
static  Acmd* _n_pullSubFrame(N_PVoice* pv, s16* inp, s16* outp, s32 outCount,
    Acmd* p);

static  s16 _n_getRate(f64 vol, f64 tgt, s32 count, u16* ratel);

#ifndef N_MICRO
static  f32 _n_getVol(f32 ivol, s32 samples, s16 ratem, u16 ratel);
#else
static  s16 _n_getVol(s16 ivol, s32 samples, s16 ratem, u16 ratel);
#endif
     
/***********************************************************************
  * Enveloper filter public interfaces
 ***********************************************************************/
Acmd *n_alEnvmixerPull(N_PVoice *filter, s32 sampleOffset, Acmd *p) 
{
  Acmd        *ptr = p;

  
  N_PVoice	*e = (N_PVoice *)filter;
  
  s16         inp;
  s32         lastOffset;
  s32         thisOffset = sampleOffset;
  s32         samples;
  s16         loutp = 0;
  s32         fVol;
  ALParam     *thisParam;
  s32 outCount = FIXED_SAMPLE;
  
#ifdef AUD_PROFILE
  lastCnt[++cnt_index] = osGetCount();
#endif
  
  /*
   * Force the input to be the resampler output
   */
#ifndef N_MICRO
  inp = AL_RESAMPLER_OUT;
#else
  inp = N_AL_RESAMPLER_OUT;
#endif
  
  while (e->em_ctrlList != 0) {
    
    lastOffset = thisOffset;
    thisOffset = e->em_ctrlList->delta;

#ifdef FINAL_ROUND
    samples    = SAMPLE184(thisOffset - lastOffset);
    if(!samples)
      thisOffset = lastOffset;
#else
    samples    = thisOffset - lastOffset;
#endif

    if (samples > outCount)
      break;
    
#ifdef _DEBUG
    assert(samples >= 0);
#ifndef N_MICRO
    assert(samples <= AL_MAX_RSP_SAMPLES);
#else
    assert(samples <= FIXED_SAMPLE);
#endif
#endif /* _DEBUG */

    switch (e->em_ctrlList->type) {
    case (AL_FILTER_START_VOICE_ALT):
      {                  
	ALStartParamAlt *param = (ALStartParamAlt *)e->em_ctrlList;
	ALFilter     *f = (ALFilter *) e;
	s32 tmp;
	
	if (param->unity) {
	  e->rs_upitch = 1;
	}
	
	n_alLoadParam(e, AL_FILTER_SET_WAVETABLE, param->wave);
	e->em_motion = AL_PLAYING;
	e->em_first  = 1;
	e->em_delta  = 0;

#ifdef FINAL_ROUND
	e->em_segEnd = SAMPLE184(param->samples);
#else
	e->em_segEnd = param->samples;
#endif

	tmp = ((s32)param->volume * (s32)param->volume) >> 15;


	e->em_volume = (s16) tmp;
	e->em_pan    = param->pan;
	e->em_dryamt = n_eqpower[param->fxMix];
	e->em_wetamt = n_eqpower[N_EQPOWER_LENGTH - param->fxMix - 1];
	
	if (param->samples) {
	  e->em_cvolL  = 1;
	  e->em_cvolR  = 1;
	} else {
	  /*
	   * Attack time is zero. Simply set the
	   * volume. We don't want an attack segment.
	   */
	  e->em_cvolL = (e->em_volume * n_eqpower[e->em_pan]) >> 15;
	  e->em_cvolR = (e->em_volume *
			 n_eqpower[N_EQPOWER_LENGTH - e->em_pan - 1]) >> 15;
	}
	e->rs_ratio = param->pitch;
      }
      
      break;
      
    case (AL_FILTER_SET_FXAMT):
    case (AL_FILTER_SET_PAN):
    case (AL_FILTER_SET_VOLUME):
      ptr = _n_pullSubFrame(e, &inp, &loutp, samples, ptr);
      
      if (e->em_delta >= e->em_segEnd){

	/*
	 * We should have reached our target, calculate
	 * target in case e->em_segEnd was 0
	 */
	e->em_ltgt = (e->em_volume * n_eqpower[e->em_pan]) >> 15;
	e->em_rtgt = (e->em_volume *
		      n_eqpower[N_EQPOWER_LENGTH - e->em_pan - 1]) >> 15;
	e->em_delta = e->em_segEnd;   /* To prevent overflow */
	e->em_cvolL = e->em_ltgt;
	e->em_cvolR = e->em_rtgt;
      } else {
	/* 
	 * Estimate the current volume
	 */
	e->em_cvolL = _n_getVol(e->em_cvolL, e->em_delta, e->em_lratm, e->em_lratl);
	e->em_cvolR = _n_getVol(e->em_cvolR, e->em_delta, e->em_rratm, e->em_rratl);
      }
      
      /*
       * We can't have volume of zero, because the envelope
       * would never go anywhere from there
       */
      if( e->em_cvolL == 0 ) e->em_cvolL = 1;
      if( e->em_cvolR == 0 ) e->em_cvolR = 1;
      
      if (e->em_ctrlList->type == AL_FILTER_SET_PAN)
	
	/*
	 * This should result in a change to the current
	 * segment rate and target
	 */
	e->em_pan = (s16) e->em_ctrlList->data.i;
      
      if (e->em_ctrlList->type == AL_FILTER_SET_VOLUME){
	
	/*
	 * Switching to a new segment
	 */
	e->em_delta = 0;
	
	/*
	 * Map volume non-linearly to give something close to
	 * loudness
	 */
	fVol = (e->em_ctrlList->data.i);
	fVol = (fVol*fVol)>>15;
	e->em_volume = (s16) fVol;
	
#ifdef FINAL_ROUND
	e->em_segEnd = SAMPLE184(e->em_ctrlList->moredata.i);
#else
	e->em_segEnd = e->em_ctrlList->moredata.i;
#endif
	
      }
      
      if (e->em_ctrlList->type == AL_FILTER_SET_FXAMT){
	e->em_dryamt = n_eqpower[e->em_ctrlList->data.i];
	e->em_wetamt = n_eqpower[N_EQPOWER_LENGTH - e->em_ctrlList->data.i - 1];
      }
      
      /*
       * Force a volume update
       */
      e->em_first = 1;
      break;
      
    case (AL_FILTER_START_VOICE):
      {
	ALStartParam *p = (ALStartParam *)e->em_ctrlList;
	
	/*
	 * Changing to PLAYING (since the previous state was
	 * persumable STOPPED, we'll just bump the output
	 * pointer rather than pull a subframe of zeros).
	 */
	if (p->unity) {
	  e->rs_upitch = 1;
	}
	
	n_alLoadParam(e, AL_FILTER_SET_WAVETABLE, p->wave);
	e->em_motion = AL_PLAYING;
      }
      break;
    case (AL_FILTER_STOP_VOICE):
      {
	/*
	 * Changing to STOPPED and reset the filter
	 */
	ptr = _n_pullSubFrame(e, &inp, &loutp, samples, ptr);
	n_alEnvmixerParam(e, AL_FILTER_RESET, 0);
      }
      break;
      
    case (AL_FILTER_FREE_VOICE):
      {
	N_ALFreeParam *param = (N_ALFreeParam *)e->em_ctrlList;
	param->pvoice->offset = 0;
	_n_freePVoice((N_PVoice *)param->pvoice);
      }
      break;
#if 1
    case (AL_FILTER_SET_PITCH):
      ptr = _n_pullSubFrame(e, &inp, &loutp, samples, ptr);
      e->rs_ratio = e->em_ctrlList->data.f;
      break;
    case (AL_FILTER_SET_UNITY_PITCH):
      ptr = _n_pullSubFrame(e, &inp, &loutp, samples, ptr);
      e->rs_upitch = 1;
      break;
    case (AL_FILTER_SET_WAVETABLE):
      ptr = _n_pullSubFrame(e, &inp, &loutp, samples, ptr);
      n_alLoadParam(e, AL_FILTER_SET_WAVETABLE,
		       (void *)e->em_ctrlList->data.i);
      break;
#endif
    default:
      /*
       * Pull the reuired number of samples and then pass the message
       * on down the chain
       */
      ptr = _n_pullSubFrame(e, &inp, &loutp, samples, ptr);
      n_alEnvmixerParam(e, e->em_ctrlList->type,
		      (void *) e->em_ctrlList->data.i);
      break;
    }
    loutp  += (samples<<1);
    outCount -= samples;
    
    /*
     * put the param record back on the free list
     */
    thisParam = e->em_ctrlList;
    e->em_ctrlList = e->em_ctrlList->next;
    if (e->em_ctrlList == 0)
      e->em_ctrlTail = 0;
    
    _n_freeParam(thisParam);
    
  }
  
  ptr = _n_pullSubFrame(e, &inp, &loutp, outCount, ptr);
  
  /*
   * Prevent overflow in e->em_delta
   */
  if (e->em_delta > e->em_segEnd)
    e->em_delta = e->em_segEnd;
  
#ifdef AUD_PROFILE
  PROFILE_AUD(env_num, env_cnt, env_max, env_min);
#endif
  return ptr;
}

s32
  n_alEnvmixerParam(N_PVoice *filter, s32 paramID, void *param)
{
  N_PVoice	*e =  filter;
  
  switch (paramID) {
  case (AL_FILTER_ADD_UPDATE):
    if (e->em_ctrlTail) {
      e->em_ctrlTail->next = (ALParam *)param;
    } else {
      e->em_ctrlList = (ALParam *)param;
    }            
    e->em_ctrlTail = (ALParam *)param;
    break;
  case (AL_FILTER_RESET):
    e->em_first = 1;
    e->em_motion = AL_STOPPED;
    e->em_volume = 1;
    e->em_segEnd = 0;
    e->rs_delta  = 0.0;
    e->rs_first  = 1;
    e->rs_upitch = 0;
    n_alLoadParam(e, AL_FILTER_RESET, param);
    break;
  case (AL_FILTER_START):
    e->em_motion = AL_PLAYING;
    break;
  default:
#if 1
    n_alLoadParam(e, paramID, param);
#else
    n_alResampleParam(e, paramID, param);
#endif
    break;
  }
  return 0;
}

INCLUDE_ASM(const s32, "lib/os/libultra/libnaudio/n_env", _n_pullSubFrame);

INCLUDE_ASM(const s32, "lib/os/libultra/libnaudio/n_env", _n_getRate);

INCLUDE_ASM(const s32, "lib/os/libultra/libnaudio/n_env", _n_getVol);
