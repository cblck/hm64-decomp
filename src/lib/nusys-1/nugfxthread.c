#include "common.h"
/*======================================================================*/
/*		NuSYS										*/
/*		nugfxthread.c								*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Doesn't match Ver 1.0, 1.2, or 1.3 :(                        		*/
/*======================================================================*/
#include <nusys.h>

NUGfxFunc	nuGfxFunc = NULL;
NUGfxPreNMIFunc	nuGfxPreNMIFunc = NULL;
OSMesgQueue	nuGfxMesgQ;

static OSMesg	nuGfxMesgBuf[NU_GFX_MESGS];
static OSThread	GfxThread;			/* graphic thread */
static u64	GfxStack[NU_GFX_STACK_SIZE/8];	/* gfx thread stack  */

static void nuGfxThread(void* arg);

/*----------------------------------------------------------------------*/
/*	nuGfxThreadStart - Activate the graphic thread 				*/
/*	IN:	None 										*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuGfxThreadStart(void)
{
    /* Activate the graphic thread */
    osCreateThread(&GfxThread, NU_GFX_THREAD_ID, nuGfxThread, (void*)NULL,
		   (GfxStack + NU_GFX_STACK_SIZE/8), NU_GFX_THREAD_PRI);
    osStartThread(&GfxThread);
}

INCLUDE_ASM(const s32, "lib/nusys-1/nugfxthread", nuGfxThread);
