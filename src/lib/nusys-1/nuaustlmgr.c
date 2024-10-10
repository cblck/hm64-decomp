#include "common.h"
/*======================================================================*/
/*		NuSYS										*/
/*		nuaustlmgr.c								*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*======================================================================*/
/* $Id: nuaustlmgr.c,v 1.2 1997/12/12 08:25:43 ohki Exp $			*/
/*======================================================================*/
#include <nusys.h>
#include <nualstl.h>

/*--------------------------------------*/
/* DMA BUFFER STRUCT		  	    */
/*--------------------------------------*/
typedef struct {
    ALLink	node;		/* link list pointer */
    int	startAddr;	/* address of data in ROM */
    u32	lastFrame;	/* value of last frame counter used */
    char*	ptr;		/* pointer value for buffer */
} NUDMABuffer;
/*----------------------------------------------------*/
/* DMA callback for sequence player				*/
/*   DMA status to be used in routine			*/
/* Structure to store status of use of DMABuffer	*/
/*----------------------------------------------------*/
typedef struct {
    u8		initialized;  /* flag to show if has already been intialised */
    NUDMABuffer*  firstUsed;    /* head of DMABuffer link list in use */
    NUDMABuffer*  firstFree;    /* head of free DMABuffer link list */
} NUDMAState;

static ALDMAproc auDmaNew(NUDMAState **state);
static void nuAuMgr(void* arg);

INCLUDE_ASM(const s32, "lib/nusys-1/nuaustlmgr", auDmaCallBack);

INCLUDE_ASM(const s32, "lib/nusys-1/nuaustlmgr", auDmaNew);

INCLUDE_ASM(const s32, "lib/nusys-1/nuaustlmgr", auCleanDMABuffers);

INCLUDE_ASM(const s32, "lib/nusys-1/nuaustlmgr", nuAuStlMgrInit);

INCLUDE_ASM(const s32, "lib/nusys-1/nuaustlmgr", nuAuMgr);
