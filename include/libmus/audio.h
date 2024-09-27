/*====================================================================
 * audio.h
 *
 * Synopsis:
 *
 * Copyright 1995, Silicon Graphics, Inc.
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

#ifndef __audio__
#define __audio__

#include <ultra64.h>

#ifdef N_AUDIO
#include <n_libaudio_sc.h>
#include <n_libaudio_sn_sc.h>
#else
#include <libaudio.h>
#endif

#include <sched.h>

// for the PCROM samples are stored in RAM...
#ifdef PCROM
#define SAMPLES_IN_DRAM
#endif

// size of the audio heap...

// These can be changed to allow the audio heap to be reduced in size. Caution
// these can influence sound quality.
//
// EXTRA_SAMPLES:     Number of extra samples to download per frame. If using fifo
//                    versions of the graphic microcode it is not recommended to 
//                    reduce this value below 240.
//

#define EXTRA_SAMPLES           80

#define MAX_EVENTS              32
#define NUM_OUTPUT_BUFFERS      3      /* Need three of these */
#define MAX_MESGS               8
#define QUIT_MSG                10
#define NUM_ACMD_LISTS          2      /* two lists used by this example                */
#define FRAME_LAG               1      /* The number of frames to keep a dma buffer.    */
                                       /* Increasing this number causes buffers to not  */
                                       /* be deleted as quickly. This results in fewer  */
                                       /* DMA's but you need more buffers.              */

#ifdef  N_AUDIO
#define SAMPLES                 184
#endif


/****  type define's for structures unique to audiomgr ****/
typedef union {    

    struct {
        short     type;
    } gen;

    struct {
        short     type;
        struct    AudioInfo_s *info;
    } done;
    
    OSScMsg       app;
    
} AudioMsg;

typedef struct AudioInfo_s {
    short         *data;          /* Output data pointer */
    short         frameSamples;   /* # of samples synthesized in this frame */
    OSScTask      task;           /* scheduler structure */
    AudioMsg      msg;            /* completion message */
} AudioInfo;

typedef struct {
    Acmd          *ACMDList[NUM_ACMD_LISTS];
    AudioInfo     *audioInfo[NUM_OUTPUT_BUFFERS];
    OSThread      thread;
    OSMesgQueue   audioFrameMsgQ;
    OSMesg        audioFrameMsgBuf[MAX_MESGS];
    OSMesgQueue   audioReplyMsgQ;
    OSMesg        audioReplyMsgBuf[MAX_MESGS];
    ALGlobals     g;
} AMAudioMgr;

typedef struct 
{
    ALLink        node;
    u32           startAddr;
    u32           lastFrame;
    char          *ptr;
} AMDMABuffer;

typedef struct 
{
    u8            initialized;
    AMDMABuffer   *firstUsed;
    AMDMABuffer   *firstFree;
} AMDMAState;

typedef struct {
    u32       outputRate;
    u32       framesPerField;
    u32       maxACMDSize;
} amConfig;

/* create audio manager function needed by 'player.c' */
void amCreateAudioMgr(OSSched *sc, ALSynConfig *c, OSPri pri, amConfig *amc, int num_dmas, int length, int vsyncsperframe);

#endif

