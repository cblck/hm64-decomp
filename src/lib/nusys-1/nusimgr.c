/*======================================================================*/
/*		NuSYS										*/
/*		nusimgr.c									*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/
/* Ver 1.1	97/12/1		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nusimgr.c,v 1.3 1997/12/03 12:25:18 ohki Exp $		        	*/
/*======================================================================*/
#include <nusys.h>

extern /*TODO: Fix extern.*/ OSMesgQueue	nuSiMesgQ;			/* SI event message queue */
static OSMesg	nuSiMesgBuf[8];	/* SI event message buffer*/
static OSThread siMgrThread;
static u64	siMgrStack[NU_SI_STACK_SIZE/sizeof(u64)];
extern /*TODO: Fix extern.*/ OSMesgQueue	nuSiMgrMesgQ;		/* SI manager queue */
extern /*TODO: Fix extern.*/ NUCallBackList	nuSiCallBackList;	/* call back function list*/

static void nuSiMgrThread(void* arg);

/*----------------------------------------------------------------------*/
/*	nuSiMgrInit - Initialize SI manager						*/
/*	IN:none										*/
/*	RET:	bit pattern for controller						*/
/*----------------------------------------------------------------------*/
u8 nuSiMgrInit(void)
{
    u8 			pattern;
    OSContStatus	status[NU_CONT_MAXCONTROLLERS];
    
    nuSiCallBackList.next = NULL;
    nuSiCallBackList.func = NULL;
    
    /* Create SI message queue and event allocation */
    osCreateMesgQueue(&nuSiMesgQ, nuSiMesgBuf, 8);
    osSetEventMesg(OS_EVENT_SI, &nuSiMesgQ, (OSMesg)0);

    /* Initialize the controller-related */
    /* Normally they should be initialized using the controller manager, but */
    /* since this function is special, calling it here is easier */
    osContInit(&nuSiMesgQ, &pattern, status);
    
    /* Activate controller thread*/
    osCreateThread(&siMgrThread, NU_SI_THREAD_ID, nuSiMgrThread,
		   (void*)NULL,
		   (siMgrStack + NU_SI_STACK_SIZE/8), NU_SI_THREAD_PRI);
    osStartThread(&siMgrThread);
    
    return pattern;
}

/*----------------------------------------------------------------------*/
/*	nuSiMgrThread - SI manager thread						*/
/* 	SI manager.  When a message is received, it will execute the	*/
/* 			 allocated call back functions in order			*/	
/*	IN:	*arg	Argument of thread creating. None				*/
/*	RET:	None										*/
/*----------------------------------------------------------------------*/
static void nuSiMgrThread(void* arg)
{
    NUScClient		siClient;
    OSMesg		siMgrMesgBuf[NU_SI_MESG_MAX];
    NUScMsg*		mesgType;
    NUCallBackList*	siCallBackListPtr;
	s32			ret;

	/* Create SI message queue */
	osCreateMesgQueue(&nuSiMgrMesgQ, siMgrMesgBuf, NU_SI_MESG_MAX);

	/* Allocate RETRACE client to the scheduler.   */
	nuScAddClient(&siClient, &nuSiMgrMesgQ, NU_SC_RETRACE_MSG | NU_SC_PRENMI_MSG);
	/* SI manager loop */
    while(1){
	(void)osRecvMesg(&nuSiMgrMesgQ,(OSMesg*)&mesgType, OS_MESG_BLOCK);

    /* Call the function allocated to the call back function list. */
	siCallBackListPtr = nuSiCallBackList.next;
	while(siCallBackListPtr){
	    if(siCallBackListPtr->func){
		ret = (*(siCallBackListPtr->func))((void*)mesgType);
		
        /* If the return value is,P, end the loop.  */
		if(ret) break;
		
	    }
	    siCallBackListPtr = siCallBackListPtr->next;
	}
    }
}
