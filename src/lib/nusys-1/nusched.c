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
void nuScCreateScheduler(u8 videoMode, u8 numFields)
{

  /* Initialize the variable */
  nusched.curGraphicsTask = NULL;
  nusched.curAudioTask    = NULL;
  nusched.graphicsTaskSuspended = NULL;
  nusched.clientList      = NULL;
  nusched.retraceMsg      = NU_SC_RETRACE_MSG;
  nusched.prenmiMsg       = NU_SC_PRENMI_MSG;
  nusched.retraceCount	  = numFields;

#ifdef	NU_DEBUG
  debTaskPerfPtr = &nuDebTaskPerf[0];
  debTaskPerfPtr->retraceTime = 0;
  debTaskPerfPtr->auTaskCnt = 0;
  debTaskPerfPtr->gfxTaskCnt = 0;
  nuDebTaskPerfPtr = debTaskPerfPtr;
  osDpSetStatus(DPC_CLR_TMEM_CTR | DPC_CLR_PIPE_CTR | DPC_CLR_CMD_CTR | DPC_CLR_CLOCK_CTR);
#endif /* NU_DEBUG */

  /* Create the message queue */
  osCreateMesgQueue(&nusched.retraceMQ, nusched.retraceMsgBuf,
		    NU_SC_MAX_MESGS);
  osCreateMesgQueue(&nusched.rspMQ, nusched.rspMsgBuf, NU_SC_MAX_MESGS);
  osCreateMesgQueue(&nusched.rdpMQ, nusched.rdpMsgBuf, NU_SC_MAX_MESGS);
  osCreateMesgQueue(&nusched.graphicsRequestMQ, nusched.graphicsRequestBuf,
		    NU_SC_MAX_MESGS);

  osCreateMesgQueue(&nusched.audioRequestMQ, nusched.audioRequestBuf,
		    NU_SC_MAX_MESGS);
  osCreateMesgQueue(&nusched.waitMQ, nusched.waitMsgBuf, NU_SC_MAX_MESGS);

  /* Set the video mode */
  osCreateViManager(OS_PRIORITY_VIMGR);
  osViSetMode(&osViModeTable[videoMode]);
  osViBlack(TRUE);

  /* Register the event handler */
  osViSetEvent(&nusched.retraceMQ, (OSMesg)VIDEO_MSG, numFields);    
  osSetEventMesg(OS_EVENT_SP    , &nusched.rspMQ,     (OSMesg)RSP_DONE_MSG);
  osSetEventMesg(OS_EVENT_DP    , &nusched.rdpMQ,     (OSMesg)RDP_DONE_MSG);
  osSetEventMesg(OS_EVENT_PRENMI, &nusched.retraceMQ, (OSMesg)PRE_NMI_MSG);   


  /* Activate the scheduler thread */
  osCreateThread(&nusched.schedulerThread, 19,
		 (void(*)(void*))nuScEventHandler,
		 (void *)&nusched, nuScStack+NU_SC_STACK_SIZE/sizeof(u64),
		 NU_SC_HANDLER_PRI);
  osStartThread(&nusched.schedulerThread);

  osCreateThread(&nusched.audioThread, 18,
		 (void(*)(void *))nuScExecuteAudio,
		 (void *)&nusched, nuScAudioStack+NU_SC_STACK_SIZE/sizeof(u64),
		 NU_SC_AUDIO_PRI);
  osStartThread(&nusched.audioThread);

  osCreateThread(&nusched.graphicsThread, 17,
		 (void(*)(void*))nuScExecuteGraphics,
		 (void *)&nusched, nuScGraphicsStack+NU_SC_STACK_SIZE/sizeof(u64),
		 NU_SC_GRAPHICS_PRI);
  osStartThread(&nusched.graphicsThread);
  
}
/*----------------------------------------------------------------------*/
/* nuScGetAudioMQ() -- Get the audio message queue 				*/
/*												*/
/* IN:	None 										*/
/* RET:	OSMesgQueue*	:The pointer of the message queue 		*/
/*----------------------------------------------------------------------*/
OSMesgQueue *nuScGetAudioMQ(void)
{
  return( &nusched.audioRequestMQ );
}

/*----------------------------------------------------------------------*/
/* nuScGetGfxMQ() -- Get the graphic message queue 				*/
/*												*/
/* IN:	None 										*/
/* RET:	OSMesgQueue*	:The pointer of the message queue 		*/
/*----------------------------------------------------------------------*/
OSMesgQueue *nuScGetGfxMQ(void)
{
  return( &nusched.graphicsRequestMQ );
}

/*----------------------------------------------------------------------*/
/*  nuScEventHandler() -- The process of the system event 			*/
/*												*/
/* IN:	None 										*/
/* RET:	None 										*/
/*----------------------------------------------------------------------*/
static void nuScEventHandler(void)
{
    OSMesg	msg;


    nuScRetraceCounter = 0;
    
    while(1) {
	/* Wait for RETRACE and PRENMI events */
	osRecvMesg(&nusched.retraceMQ, &msg, OS_MESG_BLOCK);
	
	/* Broadcast the event message */
	switch ( (int)msg ) {
	case VIDEO_MSG:		/* The process of the retrace signal */
	    nuScRetraceCounter++;
#ifdef NU_DEBUG
	    if(nuDebTaskPerfEnd){
		debTaskPerfPtr->retraceTime = OS_CYCLES_TO_USEC(osGetTime());
		nuDebTaskPerf[nuDebTaskPerfCnt].auTaskCnt = 0;
		nuDebTaskPerfEnd = 0;
	    }
#endif /* NU_DEBUG */
	    
	    nuScEventBroadcast(&nusched.retraceMsg );
	    break;
	case PRE_NMI_MSG:		/* The process of the ,m,l,h signal */
	    nuScEventBroadcast(&nusched.prenmiMsg );

	    /* The PRE NMI event call-back function */
	    if(nuScPreNMIFunc != NULL){
		(*nuScPreNMIFunc)();
	    }
	    break;
	}
    }
}

/*----------------------------------------------------------------------*/
/* nuScAddClient() -- Register the client 			 		*/
/*												*/
/* IN:  *client	The pointer of client structure of registered client 	*/
/* 	  *msgQ	The queue which recieves the message		 	*/
/*	  msgType	The type of the message RETRACE = NU_SC_RETRACE_MSG	*/
/*				 			PRENMI  = NU_SC_PRENMI_MSG	*/
/* RET:	None										*/
/*----------------------------------------------------------------------*/
void nuScAddClient(NUScClient *client, OSMesgQueue *msgQ, NUScMsg msgType)
{
    OSIntMask	mask;
    

    mask = osSetIntMask(OS_IM_NONE);

    client->msgQ = msgQ;
    client->next = nusched.clientList;
    client->msgType = msgType;
    nusched.clientList = client;
    
    osSetIntMask(mask);

}
/*----------------------------------------------------------------------*/
/* nuScRemoveClient() -- Delete the client 					*/
/*												*/
/* IN:  c	The pointer of the client structure of the deleted client 	*/
/* RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuScRemoveClient(NUScClient *c)
{
    NUScClient*	client	= nusched.clientList; 
    NUScClient*	prev	= 0;
    OSIntMask	mask;

    mask = osSetIntMask(OS_IM_NONE);
    while(client != 0){
	if(client == c){
	    if(prev){
		prev->next = c->next;
	    } else {
		nusched.clientList = c->next;
	    }
	    break;
	}
	prev   = client;
	client = client->next;
    }
    osSetIntMask(mask);
}

/*----------------------------------------------------------------------*/
/* nuScEventBroadcast() -- Transfer the message to the client 		*/
/*												*/
/* IN:	*msg	The message to transfer 					*/
/*----------------------------------------------------------------------*/
static void nuScEventBroadcast(NUScMsg *msg)
{
    NUScClient *client;
  
    /* Post the client which requires the retrace message */
    for(client = nusched.clientList; client != 0; client = client->next){
	
	/* Post with seeing msgType of the registered client */
	if(client->msgType & *msg){
	    osSendMesg(client->msgQ, (OSMesg *)msg, OS_MESG_NOBLOCK);
	}
    }
}

/*----------------------------------------------------------------------*/
/*  nuScExecuteAudio() -- Execute the audio task. 				*/
/*												*/
/*	IN:	None 										*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
static void nuScExecuteAudio(void)
{

    NUScTask*	gfxTask;
    NUScTask*	audioTask;
    OSMesg 	msg;
    u32		yieldFlag;
#ifdef NU_DEBUG
    OSIntMask	mask;
#endif /* NU_DEBUG */
    
    while(1) {
	/* Wait for the request of executing the audio task */
	osRecvMesg(&nusched.audioRequestMQ, (OSMesg *)&audioTask, OS_MESG_BLOCK);
	osWritebackDCacheAll();	/* Flush the cache */
	
	/* Check the current RSP status */
	yieldFlag = 0;
	gfxTask = nusched.curGraphicsTask;
	
	/* Yield the task if the graphic task is executing */
	if( gfxTask ) {
	    
	    /* Wait for the end (yield) of the graphics task */
	    osSpTaskYield();		/* Yield the task */
	    osRecvMesg(&nusched.rspMQ, &msg, OS_MESG_BLOCK);
	    
	    /* Check if the task has been yielded actually */
	    if (osSpTaskYielded(&gfxTask->list)){
		
		/* It has been yielded */
		yieldFlag = TASK_YIELD;
	    } else {
		
		/* The task ended at the same time as "Yield" */
		yieldFlag = TASK_YIELDED;
	    }
	}
#ifdef NU_DEBUG
	mask = osSetIntMask(OS_IM_NONE);
	if(debTaskPerfPtr->auTaskCnt < NU_DEB_PERF_AUTASK_CNT){
	    debTaskPerfPtr->auTaskTime[debTaskPerfPtr->auTaskCnt].rspStart =
		OS_CYCLES_TO_USEC(osGetTime());
	}
	osSetIntMask(mask);
#endif /* NU_DEBUG */
	
	/* Execute the audio task*/
	nusched.curAudioTask = audioTask;
	osSpTaskStart(&audioTask->list);
	
	/* Wait for the end of the RSP task */
	osRecvMesg(&nusched.rspMQ, &msg, OS_MESG_BLOCK);
	nusched.curAudioTask = (NUScTask *)NULL;

#ifdef NU_DEBUG
	mask = osSetIntMask(OS_IM_NONE);
	if(debTaskPerfPtr->auTaskCnt < NU_DEB_PERF_AUTASK_CNT){
	   debTaskPerfPtr->auTaskTime[debTaskPerfPtr->auTaskCnt].rspEnd =
	       OS_CYCLES_TO_USEC(osGetTime());
	   debTaskPerfPtr->auTaskCnt++;
	}
	osSetIntMask(mask);
#endif /* NU_DEBUG */

	/* Check if the graphic task is waiting for executing the task, */
	/* if it is waiting, send the message */
	if( nusched.graphicsTaskSuspended )
	    osSendMesg( &nusched.waitMQ, &msg, OS_MESG_BLOCK );
	
	/* Resume the yielded graphic task */
	if( yieldFlag == TASK_YIELD ) {
	    osSpTaskStart(&gfxTask->list);    
	} else if ( yieldFlag == TASK_YIELDED ) {
	    osSendMesg(&nusched.rspMQ, &msg, OS_MESG_BLOCK);
	}

	/* Post the end of the task to the thread which has activated the audio task */
	osSendMesg(audioTask->msgQ, audioTask->msg, OS_MESG_BLOCK);
    }
}

/*----------------------------------------------------------------------*/
/*  nuScExecuteGrapchics() -- Execute the graphic task 			*/
/*												*/
/*	IN:	None 										*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
static void nuScExecuteGraphics(void)
{
    OSMesg	msg;
    NUScTask*	gfxTask;
    OSIntMask	mask;
    
    while(1) {

	/* Wait for the request of executing the graphic task */
	osRecvMesg(&nusched.graphicsRequestMQ, (OSMesg *)&gfxTask, OS_MESG_BLOCK);

	/* Wait for the frame buffer becomes available*/
	nuScWaitTaskReady(gfxTask);

	/* Check if the audio task is executing, */
	/* if it is executing, wait for the end message */

	mask = osSetIntMask(OS_IM_NONE);
	if( nusched.curAudioTask ) {
	    nusched.graphicsTaskSuspended = gfxTask;
	    osSetIntMask(mask);
	    osRecvMesg( &nusched.waitMQ, &msg, OS_MESG_BLOCK );
	    mask = osSetIntMask(OS_IM_NONE);
	    nusched.graphicsTaskSuspended = (NUScTask *)NULL;
	}
	osSetIntMask(mask);

	
#ifdef NU_DEBUG
	mask = osSetIntMask(OS_IM_NONE);
	if(debTaskPerfPtr->gfxTaskCnt < NU_DEB_PERF_GFXTASK_CNT){
	    debTaskPerfPtr->gfxTaskTime[debTaskPerfPtr->gfxTaskCnt].rspStart =
		OS_CYCLES_TO_USEC(osGetTime());
	}
	osSetIntMask(mask);
#endif /* NU_DEBUG */
	    
	/* Execute the graphic task*/
	mask = osSetIntMask(OS_IM_NONE);
	nusched.curGraphicsTask = gfxTask;
	osSetIntMask(mask);
	
	osSpTaskStart(&gfxTask->list);        /* Execute the task */

	/* Wait for the RSP task ends */
	osRecvMesg(&nusched.rspMQ, &msg, OS_MESG_BLOCK);

	mask = osSetIntMask(OS_IM_NONE);
	nusched.curGraphicsTask = (NUScTask *)NULL;
	osSetIntMask(mask);

#ifdef NU_DEBUG
	mask = osSetIntMask(OS_IM_NONE);
	if(debTaskPerfPtr->gfxTaskCnt < NU_DEB_PERF_GFXTASK_CNT){
	    debTaskPerfPtr->gfxTaskTime[debTaskPerfPtr->gfxTaskCnt].rspEnd =
		OS_CYCLES_TO_USEC(osGetTime());
	}
	osSetIntMask(mask);
#endif /* NU_DEBUG */

	/* Check NU_SC_NORDP flag, whether or not RDP end wanted */
	if(!(gfxTask->flags & NU_SC_NORDP)){
	    /* Wait for the RDP task ends */
	    osRecvMesg(&nusched.rdpMQ, &msg, OS_MESG_BLOCK);
	}

#ifdef NU_DEBUG
	mask = osSetIntMask(OS_IM_NONE);
	if(debTaskPerfPtr->gfxTaskCnt < NU_DEB_PERF_GFXTASK_CNT){
	    if(gfxTask->flags & NU_SC_NORDP){
		
		/* Set with start time so as not to display */
		/* a bar that doesn't use RDP	*/
		debTaskPerfPtr->gfxTaskTime[debTaskPerfPtr->gfxTaskCnt].rdpEnd
		    = debTaskPerfPtr->gfxTaskTime[debTaskPerfPtr->gfxTaskCnt].rspStart;
		debTaskPerfPtr->gfxTaskTime[debTaskPerfPtr->gfxTaskCnt].dpCnt[0] = 0;
	    } else {
		debTaskPerfPtr->gfxTaskTime[debTaskPerfPtr->gfxTaskCnt].rdpEnd
		    = OS_CYCLES_TO_USEC(osGetTime());
		osDpGetCounters(debTaskPerfPtr->gfxTaskTime[debTaskPerfPtr->gfxTaskCnt].dpCnt);
		osDpSetStatus(DPC_CLR_TMEM_CTR | DPC_CLR_PIPE_CTR | DPC_CLR_CMD_CTR | DPC_CLR_CLOCK_CTR);
	    }
	    debTaskPerfPtr->gfxTaskCnt++;
	}

	if(gfxTask->flags & NU_SC_SWAPBUFFER){
	    nuDebTaskPerfPtr = debTaskPerfPtr;
	    nuDebTaskPerfCnt++;
	    nuDebTaskPerfCnt %= 3;
	    nuDebTaskPerf[nuDebTaskPerfCnt].gfxTaskCnt = 0;
	    nuDebTaskPerf[nuDebTaskPerfCnt].retraceTime = 0;
	    debTaskPerfPtr = &nuDebTaskPerf[nuDebTaskPerfCnt];
	    nuDebTaskPerfEnd++;
	}
	osSetIntMask(mask);
#endif /* NU_DEBUG */

	/* Post end of the task to thread which has activated graphic task */
	osSendMesg(gfxTask->msgQ, (OSMesg*)gfxTask, OS_MESG_BLOCK);
    }
}
/*----------------------------------------------------------------------*/
/*  nuScWaitTaskReady() -- Wait for the frame buffer becomes available. */
/*												*/
/*  IN:	*task	The pointer of the graphic task structure 		*/
/*----------------------------------------------------------------------*/
void nuScWaitTaskReady(NUScTask *task)
{
    NUScClient	client;
    void*	fb	= task->framebuffer;
    
    nuScAddClient(&client, &nusched.waitMQ , NU_SC_RETRACE_MSG);  
    /* Wait for next retrace if the frame buffer is not available */
    while( osViGetCurrentFramebuffer() == fb
	   || osViGetNextFramebuffer() == fb ){
	osRecvMesg( &nusched.waitMQ, NULL, OS_MESG_BLOCK );
    }
    nuScRemoveClient(&client );
}
