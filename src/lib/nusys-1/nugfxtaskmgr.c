/*======================================================================*/
/*		NuSYS										*/
/*		nugfxtaskmgr.c								*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nugfxtaskmgr.c,v *Modified version 1.3.* $						*/
/*======================================================================*/
#include <nusys.h>


static NUScTask* nuGfxTask_ptr;

static short	taskDoneMsg;
static short	swapBufMsg;
static OSThread	GfxTaskMgrThread;		/* gfx taskmgr thread */
static s32 D_801317B8[2]; // Required 0x08 byte filler to match
static u64	GfxTaskMgrStack[NU_GFX_TASKMGR_STACK_SIZE/8];
static OSMesg	nuGfxTaskMgrMesgBuf[NU_GFX_TASKMGR_MESGS];


NUUcode*	nuGfxUcode;
NUScTask	nuGfxTask[NU_GFX_TASK_NUM];	/* The graphic task structure   */
volatile u32	nuGfxTaskSpool;		/* The spool number of the task */
OSMesgQueue	nuGfxTaskMgrMesgQ;



u16**		nuGfxCfb;		/* The pointer of the frame buffer */
u32		nuGfxCfbNum = 1;	/* The number of frame buffers     */
u16*		nuGfxCfb_ptr;	/* The frame buffer for drawing    */
u16*		nuGfxZBuffer;	/* The pointer of the Z-buffer     */
u32		nuGfxDisplay;	/* Display-ON/OFF */
u32		nuGfxCfbCounter;

NUGfxSwapCfbFunc nuGfxSwapCfbFunc = NULL; /* swapbuf callback function ptr */
NUGfxTaskEndFunc nuGfxTaskEndFunc = NULL; /* task end callback  ptr */

/*----------------------------------------------------------------------*/
/*	nuGfxTaskMgr - initialize task manager 					*/
/*	IN:	*arg	The flag at the task creation (Nothing in particular)	*/
/*												*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuGfxTaskMgr(void *arg)
{
    NUScMsg*	mesg_type;
    NUScTask*	gfxTask;
    OSIntMask	mask;
    
    /* Create the message queue of the task manager */
    osCreateMesgQueue(&nuGfxTaskMgrMesgQ, nuGfxTaskMgrMesgBuf, NU_GFX_MESGS);

    /* Provide the process after accepting the end message of the */
    /* graphic task 								*/
    while(1){

	(void)osRecvMesg(&nuGfxTaskMgrMesgQ,(OSMesg*)&gfxTask, OS_MESG_BLOCK);
	/* Get the message type */
	mesg_type = gfxTask->msg;

	/* Branch the process depending on messages */
	switch(*mesg_type){
	case NU_SC_SWAPBUFFER_MSG:/* When using the graphics function, please be careful about the following */
	    
	    /* Call the function of the application side by the switch-	*/
	    /*  message of the frame buffer 					*/
	    /* Register the switch-function of the frame buffer in default */
	    if(nuGfxSwapCfbFunc != NULL){
		(*nuGfxSwapCfbFunc)((void*)gfxTask);
	    }

	    if(nuGfxDisplay == NU_GFX_DISPLAY_ON){
		osViBlack(FALSE);
	    }
	    mask = osSetIntMask(OS_IM_NONE);
	    nuGfxTaskSpool--;
	    osSetIntMask(mask);
	    break;
	    
	case NU_SC_GTASKEND_MSG:	/* End the task */

	    /* Call the function of the application side by ending the graphic task */
	    /* NULL in default 				*/
	    if(nuGfxTaskEndFunc != NULL){
		(*nuGfxTaskEndFunc)((void*)gfxTask);
	    }
	    mask = osSetIntMask(OS_IM_NONE);
	    nuGfxTaskSpool--;
	    osSetIntMask(mask);
	    break;
	}

    }
}



/*----------------------------------------------------------------------*/
/*	nuGfxTaskMgrInit - Initialize the task manager 				*/
/*	IN:	None 										*/
/*												*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuGfxTaskMgrInit(void)
{
    u32	cnt;

    taskDoneMsg	= NU_SC_GTASKEND_MSG;
    swapBufMsg	= NU_SC_SWAPBUFFER_MSG;
    nuGfxTaskSpool = 0;
    nuGfxDisplayOff();		/* Display-OFF of the screen */

    /* Activate the thread of the graphic task manager */
    osCreateThread(&GfxTaskMgrThread, NU_GFX_TASKMGR_THREAD_ID, nuGfxTaskMgr,
		   (void*)NULL,
		   (GfxTaskMgrStack + NU_GFX_TASKMGR_STACK_SIZE/8),
		   NU_GFX_TASKMGR_THREAD_PRI );
    osStartThread(&GfxTaskMgrThread);
    
    /* Initialize the task structure */
    /* Set the constant, first 	 */
    for(cnt = 0; cnt < NU_GFX_TASK_NUM; cnt++){
	 nuGfxTask[cnt].next			= &nuGfxTask[cnt+1];
	 nuGfxTask[cnt].msgQ			= &nuGfxTaskMgrMesgQ;
	 nuGfxTask[cnt].list.t.type		= M_GFXTASK;
	 nuGfxTask[cnt].list.t.flags		= 0x00;
	 nuGfxTask[cnt].list.t.ucode_boot	= (u64*)rspbootTextStart;
	 nuGfxTask[cnt].list.t.ucode_boot_size 	= ((s32) rspbootTextEnd 
						    - (s32) rspbootTextStart);
	 nuGfxTask[cnt].list.t.ucode_size 	= SP_UCODE_SIZE;
	 nuGfxTask[cnt].list.t.ucode_data_size 	= SP_UCODE_DATA_SIZE;
	 nuGfxTask[cnt].list.t.dram_stack	= (u64*) nuDramStack;
	 nuGfxTask[cnt].list.t.dram_stack_size 	= SP_DRAM_STACK_SIZE8;
	 nuGfxTask[cnt].list.t.output_buff	=  (u64 *)&nuRDPOutputBuf[0];
	 nuGfxTask[cnt].list.t.output_buff_size 	=
	     (u64 *)(nuRDPOutputBuf + NU_GFX_RDP_OUTPUTBUFF_SIZE);
	 nuGfxTask[cnt].list.t.yield_data_ptr	= (u64 *) nuYieldBuf;
	 nuGfxTask[cnt].list.t.yield_data_size	= OS_YIELD_DATA_SIZE;
	 nuGfxTask[cnt].msgQ			= &nuGfxTaskMgrMesgQ;	 
     }
     nuGfxTask[NU_GFX_TASK_NUM-1].next = &nuGfxTask[0];
     nuGfxTask_ptr	= &nuGfxTask[0];

 }

 /*---------------------------------------------------------------------*/
 /*	nuGfxTaskStart - Activate the graphic task 				*/
 /*	IN:	gfxListptr	The pointer of the display list 			*/
 /*		gfxListSize The size of the display list				*/
 /*		ucode		The micro code 						*/
 /*												*/
 /*	RET:	None 										*/
 /*---------------------------------------------------------------------*/
 void nuGfxTaskStart(Gfx *gfxList_ptr, u32 gfxListSize, u32 ucode, u32 flag)
 {
     u32	framebuf_no;
     OSIntMask	mask;
     
     nuGfxTask_ptr->list.t.data_ptr	= (u64*)gfxList_ptr;
     nuGfxTask_ptr->list.t.data_size	= gfxListSize;
     nuGfxTask_ptr->list.t.ucode 	= nuGfxUcode[ucode].ucode;
     nuGfxTask_ptr->list.t.ucode_data	= nuGfxUcode[ucode].ucode_data;
     nuGfxTask_ptr->flags		= flag;
     nuGfxTask_ptr->framebuffer		= (u16*)nuGfxCfb_ptr;
     
     /* Set the end message of the task */
     if(flag & NU_SC_SWAPBUFFER){
	 /* In the case of ending the task for one screen */
	 nuGfxTask_ptr->msg         	= (OSMesg)&swapBufMsg;
	 
	 nuGfxCfbCounter++;
	 framebuf_no = nuGfxCfbCounter % nuGfxCfbNum;
	 nuGfxCfb_ptr = nuGfxCfb[framebuf_no];
     } else {
	 /* If the task for one screen does not end */
	 nuGfxTask_ptr->msg         = (OSMesg)&taskDoneMsg;
     }
     mask = osSetIntMask(OS_IM_NONE);
     nuGfxTaskSpool++;
     osSetIntMask(mask);
     
     /* Activate the graphic task */
     osWritebackDCacheAll();
     osSendMesg(&nusched.graphicsRequestMQ,
		(OSMesg*)nuGfxTask_ptr, OS_MESG_BLOCK);
     
     
     /* Change the task buffer to next pointer */
     nuGfxTask_ptr = nuGfxTask_ptr->next;
     
}
