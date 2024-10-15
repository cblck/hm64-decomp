#include "common.h"
/*======================================================================*/
/*		NuSYS										*/
/*		nusched.c									*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*======================================================================*/  
/*	1997/01/23										*/
/*		Added the OS_TASK_LOADABLE flag for corresponding to 		*/
/*      gSPLoadUcode which was added to F3DEX Ver1.20.  			*/
/*	1997/02/03										*/
/*      Corrected  inconvenience of hanging-up if the value was larger 	*/
/*	  than NN_SC_GTASK_NUM and NN_SC_AUTASK_NUM in performance check. */
/*	1997/07/15										*/
/*	  Changed to nusched as the kernel of NuSYS 				*/
/*	  Added msgType to the argument of nuScAddClient and 			*/
/*	  improved the sent message to the choosable one. 			*/
/*	  Changed the NuSched structure to the internal variable. 		*/
/*	1997/09/28										*/
/*	  Set the critical section of the thread. 				*/
/*      1997/11/28									*/
/*		In order to be compatible with Z-Sort microcode, the flag	*/
/*		the flag that doesn't wait for RDP to end was added.		*/
/*	1997/12/13										*/
/*		Measurement of internal counter of DP was added.		*/
/*======================================================================*/
/* $Id: nusched.c,v 1.4 1997/12/13 09:33:04 ohki Exp $			*/
/*======================================================================*/

#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	DEFINE										*/
/*----------------------------------------------------------------------*/
#define TASK_YIELD	1		/* GFX TASK YILED FLAG */
#define TASK_YIELDED	2		/* GFX TASK YILEDED FLAG */

#define VIDEO_MSG		666		/* The retrace message */
#define RSP_DONE_MSG	667		/* End the ,q,r,o task */
#define RDP_DONE_MSG	668		/* End the ,q,c,o drawing */
#define PRE_NMI_MSG	669		/* The ,m,l,h message */


/*----------------------------------------------------------------------*/
/*	internal function									*/
/*----------------------------------------------------------------------*/
static void nuScEventHandler(void);
static void nuScEventBroadcast(NUScMsg *msg);
static void nuScExecuteAudio(void);
static void nuScExecuteGraphics(void);
static void nuScWaitTaskReady(NUScTask *task);

/*----------------------------------------------------------------------*/
/*	variable										*/
/*----------------------------------------------------------------------*/
static u64	nuScStack[NU_SC_STACK_SIZE/8];	  /* event thread stack */
static u64	nuScAudioStack[NU_SC_STACK_SIZE/8];	  /* audio thread stack */
static u64	nuScGraphicsStack[NU_SC_STACK_SIZE/8];/*gfx thread stack 	*/

NUSched		nusched;	/* The scheduler structure */
NUScPreNMIFunc	nuScPreNMIFunc = NULL;	/* PRE NMI callback function ptr */

u32		nuScRetraceCounter;    /* The retrace counter */

#ifdef NU_DEBUG
/*------------------------------*/
/*	Performanc		 	  */
/*------------------------------*/
NUDebTaskPerf		nuDebTaskPerf[3];
NUDebTaskPerf*		nuDebTaskPerfPtr;
volatile u32		nuDebTaskPerfCnt = 0;
volatile u32		nuDebTaskPerfEnd = 1;
static NUDebTaskPerf*	debTaskPerfPtr;
#endif /* NU_DEBUG */

/*----------------------------------------------------------------------*/
/* nuScCreateScheduler() -- Create the scheduler 				*/
/*												*/
/* IN:videoMode	The video mode to set 						*/
/* 	numFields	The retrace cycle (1=60Frame/sec,2=30Frame/sec...)	*/
/* RET:	None 										*/
/*----------------------------------------------------------------------*/
INCLUDE_ASM(const s32, "lib/nusys-1/nusched", nuScCreateScheduler);

// void nuScCreateScheduler(u8 videoMode, u8 numFields) {
// 
//   nusched.curGraphicsTask = NULL;
//   nusched.curAudioTask    = NULL;
//   nusched.graphicsTaskSuspended = NULL;
//   nusched.clientList      = NULL;
//   nusched.retraceMsg      = NU_SC_RETRACE_MSG;
//   nusched.prenmiMsg       = NU_SC_PRENMI_MSG;
//   nusched.retraceCount	  = numFields;
//     
//   osCreateMesgQueue(&nusched.retraceMQ, nusched.retraceMsgBuf,
// 		    NU_SC_MAX_MESGS);
//   osCreateMesgQueue(&nusched.rspMQ, nusched.rspMsgBuf, NU_SC_MAX_MESGS);
//   osCreateMesgQueue(&nusched.rdpMQ, nusched.rdpMsgBuf, NU_SC_MAX_MESGS);
//   osCreateMesgQueue(&nusched.graphicsRequestMQ, nusched.graphicsRequestBuf,
// 		    NU_SC_MAX_MESGS);
// 
//   osCreateMesgQueue(&nusched.audioRequestMQ, nusched.audioRequestBuf,
// 		    NU_SC_MAX_MESGS);
//   osCreateMesgQueue(&nusched.waitMQ, nusched.waitMsgBuf, NU_SC_MAX_MESGS);
// 
//   osCreateViManager(OS_PRIORITY_VIMGR);
//   osViSetMode(&osViModeTable[videoMode]);
//   osViBlack(TRUE);
// 
//   osViSetEvent(&nusched.retraceMQ, (OSMesg)VIDEO_MSG, numFields);    
//   osSetEventMesg(OS_EVENT_SP    , &nusched.rspMQ,     (OSMesg)RSP_DONE_MSG);
//   osSetEventMesg(OS_EVENT_DP    , &nusched.rdpMQ,     (OSMesg)RDP_DONE_MSG);
//   osSetEventMesg(OS_EVENT_PRENMI, &nusched.retraceMQ, (OSMesg)PRE_NMI_MSG);   
// 
//   osCreateThread(&nusched.schedulerThread, 19,
// 		 (void(*)(void*))nuScEventHandler,
// 		 (void *)&nusched, &nuScStack+NU_SC_STACK_SIZE/sizeof(u64),
// 		 NU_SC_HANDLER_PRI);
//   osStartThread(&nusched.schedulerThread);
// 
//   osCreateThread(&nusched.audioThread, 18,
// 		 (void(*)(void *))nuScExecuteAudio,
// 		 (void *)&nusched, &nuScAudioStack+NU_SC_STACK_SIZE/sizeof(u64),
// 		 NU_SC_AUDIO_PRI);
//   osStartThread(&nusched.audioThread);
// 
//   osCreateThread(&nusched.graphicsThread, 17,
// 		 (void(*)(void*))nuScExecuteGraphics,
// 		 (void *)&nusched, &nuScGraphicsStack+NU_SC_STACK_SIZE/sizeof(u64),
// 		 NU_SC_GRAPHICS_PRI);
//   osStartThread(&nusched.graphicsThread);
// 
// }

// nuScGetAudioMQ
INCLUDE_ASM(const s32, "lib/nusys-1/nusched", func_800FB354);

// nuScGetGfxMQ
INCLUDE_ASM(const s32, "lib/nusys-1/nusched", func_800FB360);

INCLUDE_ASM(const s32, "lib/nusys-1/nusched", nuScEventHandler);

// nuScAddClient
INCLUDE_ASM(const s32, "lib/nusys-1/nusched", nuScAddClient);

INCLUDE_ASM(const s32, "lib/nusys-1/nusched", nuScRemoveClient);

// nuScEventBroadcast
INCLUDE_ASM(const s32, "lib/nusys-1/nusched", func_800FB524);

INCLUDE_ASM(const s32, "lib/nusys-1/nusched", nuScExecuteAudio);

INCLUDE_ASM(const s32, "lib/nusys-1/nusched", nuScExecuteGraphics);

// nuScWaitTaskReady
INCLUDE_ASM(const s32, "lib/nusys-1/nusched", func_800FB800);
