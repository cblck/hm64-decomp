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


/*static*/extern OSThread	auMgrThread;
/*static*/extern u64		auMgrStack[NU_AU_STACK_SIZE];

/*static*/extern Acmd*    auCmdList_ptr[2]; /* pointer of command list */
/*static*/extern NUScTask	auTask[2];		/* audio task buffer */
/*static*/extern s16*     auBuffer_ptr[3];	/* audio buffer */


/*static*/extern OSMesgQueue	auDmaMesgQ;
/*static*/extern OSMesg		auDmaMesgBuf[NU_AU_DMA_QUEUE_NUM];
/*static*/extern OSIoMesg		auDmaIOMesgBuf[NU_AU_DMA_QUEUE_NUM];
/*static*/extern NUDMAState	auDmaState;
/*static*/extern NUDMABuffer	auDmaBuf[NU_AU_DMA_BUFFER_NUM];
/*static*/extern s32			auDmaNext;

static ALDMAproc auDmaNew(NUDMAState** state);
static void nuAuMgr(void* arg);

u32			    nuAuFrameCounter = 0;	/* flame counter */
ALHeap		    nuAuHeap;			/* Heap structure */
ALGlobals		auGlobal;
NUAuSeqPlayer	nuAuSeqPlayer[2];
ALBankFile*     nuAuSeqBank_ptr;
extern ALSeqFile*      nuAuSeqFile_ptr;
extern ALSndPlayer		nuAuSndPlayer;
ALBankFile*     nuAuSndBank_ptr = NULL;
ALSndId*        nuAuSndId = NULL;
u8			    nuAuTaskStop = 1;
u8			    nuAuPreNMI = 0;
NUAuPreNMIFunc	nuAuPreNMIFunc = NULL;

INCLUDE_ASM(const s32, "lib/nusys-1/nuaustlmgr", auDmaCallBack);

INCLUDE_ASM(const s32, "lib/nusys-1/nuaustlmgr", auDmaNew);

INCLUDE_ASM(const s32, "lib/nusys-1/nuaustlmgr", auCleanDMABuffers);

INCLUDE_ASM(const s32, "lib/nusys-1/nuaustlmgr", nuAuStlMgrInit);

INCLUDE_ASM(const s32, "lib/nusys-1/nuaustlmgr", nuAuMgr);
