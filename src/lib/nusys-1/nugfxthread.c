#include "common.h"
/*======================================================================*/
/*		NuSYS										*/
/*		nugfxthread.c								*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
#include <nusys.h>

NUGfxFunc	nuGfxFunc = NULL;
NUGfxPreNMIFunc	nuGfxPreNMIFunc = NULL;
extern /*TODO: Fix extern.*/ OSMesgQueue	nuGfxMesgQ;

static OSMesg	nuGfxMesgBuf[NU_GFX_MESGS];
static OSThread	GfxThread;			/* graphic thread */
static u64	GfxStack[NU_GFX_STACK_SIZE/8];	/* gfx thread stack  */

INCLUDE_ASM(const s32, "lib/nusys-1/nugfxthread", nuGfxThreadStart);

INCLUDE_ASM(const s32, "lib/nusys-1/nugfxthread", nuGfxThread);
