/*======================================================================*/
/*		NuSYS										*/
/*		nucontmgr.c									*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontmgr.c,v 1.9 1997/12/13 13:00:59 ohki Exp $			*/
/*======================================================================*/
#include <nusys.h>


OSMesgQueue		nuContWaitMesgQ;	 		/* The wait queue of the controller read  */
static OSMesg	nuContWaitMesgBuf;
OSMesgQueue		nuContDataMutexQ;
static OSMesg	nuContDataMutexBuf;		/* Semaphore for data access */
OSContStatus	nuContStatus[NU_CONT_MAXCONTROLLERS];
OSContPad		nuContData[NU_CONT_MAXCONTROLLERS];
u32			nuContNum;
u32			nuContDataLockKey;

NUContReadFunc	nuContReadFunc = NULL;	/* The call-back function at the */
										/* end of the controller read */
OSPfs		nuContPfs[4];		/* The pfs structure for the controller manager */
NUCallBackList	nuContCallBack;


/*----------------------------------------------------------------------*/
/*	nuContMgrInit - Initialize the controller manager 			*/
/*	Initialize conroller manager and register in SI manager.  For 	*/
/*	compatability w/ previous version, create and return bit pattern	*/
/*	IN:	None 										*/
/*	RET:	The bit pattern of the connected controller 			*/
/*----------------------------------------------------------------------*/
u8 nuContMgrInit(void)
{
    int	cnt;
    u8	pattern;
    u8	bitmask;

    if(nuContCallBack.func != NULL){
#ifdef NU_DEBUG
	osSyncPrintf("nuContMgrinit: controller manager already entry!!\n");
#endif /* NU_DEBUG */
	return 0;
    }

    /* Unlock the reading-stop flag of the controller data  */
    nuContDataUnLock();
    
    /* The wait queue of reading the controller data */
    osCreateMesgQueue(&nuContWaitMesgQ, &nuContWaitMesgBuf, 1);

    /* Create semaphore to access controller data */
    osCreateMesgQueue(&nuContDataMutexQ, &nuContDataMutexBuf, 1);
    
    /* Create the SI message queue and register the event */
    nuSiCallBackAdd(&nuContCallBack, nuContMgr);

    /* Check controller status */
    nuContQueryRead();

    /* Confirm number of controller connections */
    /* Just a confirmation */
    nuContNum = 0;
    bitmask = 1;
    pattern = 0;
    for(cnt = 0; cnt < NU_CONT_MAXCONTROLLERS; cnt++){

	/* Check errors.  Don't use if you find errors */
	if(nuContStatus[cnt].errno) continue;

	/* Verify that both flags of CONT_ABUSOLUTE and CONT_JOYPORT are on */
	if((nuContStatus[cnt].type & (CONT_ABSOLUTE | CONT_JOYPORT))
	   == (CONT_ABSOLUTE | CONT_JOYPORT)){
	    nuContNum++;
	    pattern |= bitmask;
	}
	bitmask <<= 1;
    }
    return pattern;
}
/*----------------------------------------------------------------------*/
/*	nuContMgrRemove - Remove control manager 					*/
/* 		Detach controller manager from SI manager 			*/
/*	IN:	None										*/
/*	RET:	None										*/
/*----------------------------------------------------------------------*/
void nuContMgrRemove(void)
{
    nuSiCallBackRemove(&nuContCallBack);
}

/*----------------------------------------------------------------------*/
/*	nuContDataClose - Close controller data 					*/
/*		nuContData access is disabled from other threads 		*/
/*	IN:	None										*/
/*	RET:	None										*/
/*----------------------------------------------------------------------*/
void nuContDataClose(void)
{
    osSendMesg(&nuContDataMutexQ, NULL, OS_MESG_BLOCK);

}

/*----------------------------------------------------------------------*/
/*	nuContDataOpen - Open controller data					*/
/*		Enable nuContData to be accessed from other treads		*/
/*	IN:	None										*/
/*	RET:	None										*/
/*----------------------------------------------------------------------*/
void nuContDataOpen(void)
{
    osRecvMesg(&nuContDataMutexQ, NULL, OS_MESG_BLOCK);
}
	
/*----------------------------------------------------------------------*/
/*	contReadData - Read the controller data (The internal function) 	*/
/*	IN:	lockflag	The flag to check the data lock 			*/
/*				0 No check (The forced reading mode) 		*/
/*				1 Check 							*/
/*	RET:	None										*/
/*----------------------------------------------------------------------*/
static void contReadData(OSContPad *pad,u32 lockflag)
{

    /* Start to read the controller */
    osContStartReadData(&nuSiMesgQ);
    
    /* Wait for the end of reading the controller (About 2ms) */
    osRecvMesg(&nuSiMesgQ, NULL, OS_MESG_BLOCK);

    /* Check if data has been locked  						*/
    /* The countermeasure for the case of that the osRecvMesg above 	*/
    /* switches threads and the thread locks the data 			*/
    if(lockflag & nuContDataLockKey) return;
    
    /* Get the controller data */
    nuContDataClose();
    osContGetReadData(pad);
    nuContDataOpen();
}

/*----------------------------------------------------------------------*/
/*	contQuery - Get the state of the controller 				*/
/*	IN:	*mesg	The pointer of the NUContQueryMesg type structure 	*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
static void contQuery(NUContQueryMesg* mesg)
{

    /* Start to get the state of the controller */
    osContStartQuery(&nuSiMesgQ);
    
    /* Wait for getting */
    osRecvMesg(&nuSiMesgQ, NULL, OS_MESG_BLOCK);
    
    /* Read the acquired data */
    osContGetQuery(nuContStatus);
    
    /* Send the end message of reading */
    osSendMesg(mesg->rtnMesgQ, NULL, OS_MESG_NOBLOCK);
}


/*----------------------------------------------------------------------*/
/*	nuContMgrThread - The controller manager thread 			*/
/* 		Read controller data							*/
/*	IN:	Message from SI manager							*/
/*	RET:	0:  Process continues, 1:  Process ends				*/
/*----------------------------------------------------------------------*/
s32 nuContMgr(void* mesg)
{
    
    NUContReadMesg*	readMesg;
    NUScMsg*		mesg_type;

    mesg_type = (NUScMsg*)mesg;
    
    /* Distribute processes by message */
    switch(*mesg_type){
    case NU_SC_RETRACE_MSG:	/* Reading by the retrace message */
	
	/* Don't read if data has been locked */
	if(nuContDataLockKey) break;
	
	/* Clear the wait message queue */
	osRecvMesg(&nuContWaitMesgQ, NULL, OS_MESG_NOBLOCK);
	
	/* Read the controller */
	contReadData(nuContData, 1);
	
	    /* Call the call-back function after the end of reading 		*/
	    /* Priority of this thread is high following the event handler;  	*/
	    /* thus, provide the process of the call-back function only about 	*/
	    /* the minimum requirement.  The argument is *msg_type			*/
	if(nuContReadFunc != NULL){
	    (*nuContReadFunc)(*mesg_type);
	}
	
	/* Send the end message of reading */
	osSendMesg(&nuContWaitMesgQ, NULL, OS_MESG_NOBLOCK);

	/* It won't end in the case of a retrace message */
	return NU_SI_CALLBACK_CONTINUE;	
	
    case NU_CONT_READ_MSG:	/* Require controller reading from the application */
	readMesg = (NUContReadMesg*) mesg_type;
	contReadData(readMesg->pad, 0);
	osSendMesg(readMesg->rtnMesgQ, NULL, OS_MESG_NOBLOCK);
	break;
	
    case NU_CONT_READ_NW_MSG:  /* On requiring controller reading, 		*/
				   	 /* the end message is the same as the one 	*/
	    			   	 /* of "retrace" 						*/
	
	/* Clear the wait message queue */
	osRecvMesg(&nuContWaitMesgQ, NULL, OS_MESG_NOBLOCK);
	/* Read the controller */
	contReadData(nuContData, 0);
	
	/* Call the call-back function after the end of reading 					*/
	/* Priority of this thread is high following the event handler; thus, 			*/
	/* provide the process of the call-back function only about the minimum requirement */
	if(nuContReadFunc != NULL){
	    (*nuContReadFunc)(*mesg_type);
	}
	
	
	/* Send the end message of reading */
	osSendMesg(&nuContWaitMesgQ, NULL, OS_MESG_NOBLOCK);
	
	break;
    case NU_CONT_QUERY_MSG:		/* Get the state of the controller */
	contQuery((NUContQueryMesg*)mesg_type);
	break;
    default:
	/* As message is unknown, leave it to other call-back function */
	return NU_SI_CALLBACK_CONTINUE;
    }
    
    /* Default: end call-back function loop */
    return NU_SI_CALLBACK_END;
}
