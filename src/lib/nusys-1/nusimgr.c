#include "common.h"
/*======================================================================*/
/*		NuSYS										*/
/*		nusimgr.c									*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/
/* Ver 1.1	97/12/1		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nusimgr.c,v 1.3 1997/12/03 12:25:18 ohki Exp $			*/
/*======================================================================*/
#include <nusys.h>

extern /*TODO: Fix extern.*/ OSMesgQueue	nuSiMesgQ;			/* SI event message queue */
static OSMesg	nuSiMesgBuf[8];	/* SI event message buffer*/
static OSThread siMgrThread;
static u64	siMgrStack[NU_SI_STACK_SIZE/sizeof(u64)];
extern /*TODO: Fix extern.*/ OSMesgQueue	nuSiMgrMesgQ;		/* SI manager queue */
extern /*TODO: Fix extern.*/ NUCallBackList	nuSiCallBackList;	/* call back function list*/

INCLUDE_ASM(const s32, "lib/nusys-1/nusimgr", nuSiMgrInit);

INCLUDE_ASM(const s32, "lib/nusys-1/nusimgr", nuSiMgrThread);
