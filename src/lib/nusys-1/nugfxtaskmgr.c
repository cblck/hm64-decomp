#include "common.h"
/*======================================================================*/
/*		NuSYS										*/
/*		nugfxtaskmgr.c								*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nugfxtaskmgr.c,v 1.3 1997/12/11 00:36:10 ohki Exp ohki $		*/
/*======================================================================*/
#include <nusys.h>


// static NUScTask* nuGfxTask_ptr;
// 
// static short	taskDoneMsg;
// static short	swapBufMsg;
// static OSThread	GfxTaskMgrThread;		/* gfx taskmgr thread */
// static u64	GfxTaskMgrStack[NU_GFX_TASKMGR_STACK_SIZE/8];
// static OSMesg	nuGfxTaskMgrMesgBuf[NU_GFX_TASKMGR_MESGS];
// 
// 
// NUUcode*	nuGfxUcode;
// NUScTask	nuGfxTask[NU_GFX_TASK_NUM];	/* The graphic task structure   */
// volatile u32	nuGfxTaskSpool;		/* The spool number of the task */
// OSMesgQueue	nuGfxTaskMgrMesgQ;



//u16**		nuGfxCfb;		/* The pointer of the frame buffer */
u32		nuGfxCfbNum = 1;	/* The number of frame buffers     */
// u16*		nuGfxCfb_ptr;	/* The frame buffer for drawing    */
// u16*		nuGfxZBuffer;	/* The pointer of the Z-buffer     */
// u32		nuGfxDisplay;	/* Display-ON/OFF */
// u32		nuGfxCfbCounter;

NUGfxSwapCfbFunc nuGfxSwapCfbFunc = NULL; /* swapbuf callback function ptr */
NUGfxTaskEndFunc nuGfxTaskEndFunc = NULL; /* task end callback  ptr */

INCLUDE_ASM(const s32, "lib/nusys-1/nugfxtaskmgr", nuGfxTaskMgr);

INCLUDE_ASM(const s32, "lib/nusys-1/nugfxtaskmgr", nuGfxTaskMgrInit);

INCLUDE_ASM(const s32, "lib/nusys-1/nugfxtaskmgr", nuGfxTaskStart);
