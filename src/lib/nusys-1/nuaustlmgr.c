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


extern /*static*/ OSThread	auMgrThread;
extern /*static*/ u64		auMgrStack[NU_AU_STACK_SIZE];

extern /*static*/ Acmd*	auCmdList_ptr[2]; /* pointer of command list */
extern /*static*/ NUScTask	auTask[2];		/* audio task buffer */
extern /*static*/ s16*		auBuffer_ptr[3];	/* audio buffer */


extern /*static*/ OSMesgQueue	auDmaMesgQ;
extern /*static*/ OSMesg		auDmaMesgBuf[NU_AU_DMA_QUEUE_NUM];
extern /*static*/ OSIoMesg		auDmaIOMesgBuf[NU_AU_DMA_QUEUE_NUM];
extern /*static*/ NUDMAState		auDmaState;
extern /*static*/ NUDMABuffer	auDmaBuf[NU_AU_DMA_BUFFER_NUM];
extern /*static*/ s32			auDmaNext;

static ALDMAproc auDmaNew(NUDMAState **state);
static void nuAuMgr(void* arg);

extern /*TODO: Fix extern*/ u32			nuAuFrameCounter; //= 0;	/* flame counter */
extern /*TODO: Fix extern*/ ALHeap		nuAuHeap;			/* Heap structure */
extern /*TODO: Fix extern*/ ALGlobals		auGlobal;
extern /*TODO: Fix extern*/ NUAuSeqPlayer	nuAuSeqPlayer[2];
extern /*TODO: Fix extern*/ ALBankFile*		nuAuSeqBank_ptr;
extern /*TODO: Fix extern*/ ALSeqFile*		nuAuSeqFile_ptr;
extern /*TODO: Fix extern*/ ALSndPlayer		nuAuSndPlayer;
extern /*TODO: Fix extern*/ ALBankFile*		nuAuSndBank_ptr; //= NULL;
extern /*TODO: Fix extern*/ ALSndId*       	nuAuSndId; //= NULL;
extern /*TODO: Fix extern*/ u8			nuAuTaskStop; //= 1;
extern /*TODO: Fix extern*/ u8			nuAuPreNMI; //= 0;
extern /*TODO: Fix extern*/ NUAuPreNMIFunc	nuAuPreNMIFunc; //= NULL;

INCLUDE_ASM(const s32, "lib/nusys-1/nuaustlmgr", auDmaCallBack);

INCLUDE_ASM(const s32, "lib/nusys-1/nuaustlmgr", auDmaNew);

INCLUDE_ASM(const s32, "lib/nusys-1/nuaustlmgr", auCleanDMABuffers);

INCLUDE_ASM(const s32, "lib/nusys-1/nuaustlmgr", nuAuStlMgrInit);

INCLUDE_ASM(const s32, "lib/nusys-1/nuaustlmgr", nuAuMgr);
