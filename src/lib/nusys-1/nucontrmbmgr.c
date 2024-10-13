/*======================================================================*/
/*		NuSYS										*/
/*		nucontrmbmgr.c								*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontrmbmgr.c,v 1.8 1997/12/06 07:54:52 ohki Exp $			*/
/*======================================================================*/
#include <nusys.h>

extern /*TODO: Fix extern.*/ NUContRmbCtl	nuContRmbCtl[4];
u32		nuContRmbSearchTime = NU_CONT_RMB_AUTO_SEARCHTIME;

extern /*TODO: Fix extern.*/ NUCallBackList	nuContRmbCallBack;

/*----------------------------------------------------------------------*/
/*	nuContRmbMgrInit - Initialize the oscillating pack manager 		*/
/*	IN:	None 										*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuContRmbMgrInit(void)
{
    u32 cnt;


    if(nuContRmbCallBack.func != NULL){
#ifdef NU_DEBUG
	osSyncPrintf("nuContRmbMgrInit: RUMBLE manager already entry!!\n");
#endif /* NU_DEBUG */
	return;
    }
    
    /* Initialize the control structure of the oscillating pack */
    for(cnt = 0; cnt <NU_CONT_MAXCONTROLLERS; cnt++){
	nuContRmbCtl[cnt].state = NU_CONT_RMB_STATE_STOPPED;
	nuContRmbCtl[cnt].mode	= NU_CONT_RMB_MODE_DISABLE;
	nuContRmbCtl[cnt].counter = cnt;
    }
    nuSiCallBackAdd(&nuContRmbCallBack, nuContRmbMgr);
}
/*----------------------------------------------------------------------*/
/*	nuContRmbMgrRemove - Remove Rumble Pak manager				*/
/*	IN:	None										*/
/*	RET:	None										*/
/*----------------------------------------------------------------------*/
void nuContRmbMgrRemove(void)
{
    nuSiCallBackRemove(&nuContRmbCallBack);
}
/*----------------------------------------------------------------------*/
/*	contRmbMgr - The oscillating pack manager  				*/
/*	IN:	None 										*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
static int contRmbControl(NUContRmbCtl* rmbCtl, u32 contNo)
{
    s32	rtn = 0;
    u32	integer;

    /* Control the state of the oscillating pack */
    switch(rmbCtl->state){
    case NU_CONT_RMB_STATE_STOPPED:	/* The state of the motor stop */
	break;
    case NU_CONT_RMB_STATE_STOPPING:
	/* Provide the process for stopping the motor, which requires the */
	/*	execution of three frames of osMotorStop.   			*/
	if(rmbCtl->counter > 0){
	    rtn = osMotorStop(&nuContPfs[contNo]);
	} else {
	    rmbCtl->state = NU_CONT_RMB_STATE_STOPPED;
	}
	rmbCtl->counter--;
	break;
    case NU_CONT_RMB_STATE_RUN:	/* Make the motor rotate adapting to the frequency */
	if(rmbCtl->frame >0){
	    rmbCtl->counter += rmbCtl->freq;
	    integer = rmbCtl->counter >> 8;
	    rmbCtl->counter &= 0x00ff;
	    if( integer > 0){
		rtn = osMotorStart(&nuContPfs[contNo]);
	    } else {
		rtn = osMotorStop(&nuContPfs[contNo]);
	    }
	} else {
	    rtn = osMotorStop(&nuContPfs[contNo]);
	    rmbCtl->state = NU_CONT_RMB_STATE_STOPPING;
	    rmbCtl->counter = 2;
	}
	rmbCtl->frame--;
	break;
    case NU_CONT_RMB_STATE_FORCESTOP:	/* Force to stop */
	rtn = osMotorInit(&nuSiMesgQ, &nuContPfs[contNo], contNo);
	if(!rtn){
	    osMotorStop(&nuContPfs[contNo]);
	}
	rmbCtl->state = NU_CONT_RMB_STATE_STOPPING;
	rmbCtl->counter = 2;
	break;
    }
    return rtn;
}
/*----------------------------------------------------------------------*/
/*	contRmbRetrace - Control the oscillating pack 				*/
/*	IN:	None 										*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void contRmbRetrace(void)
{
    u32		cnt;
    s32		rtn;
    NUContRmbCtl*  rmbCtl;
    u32 	counter;
    
    counter = nuScRetraceCounter  % nuContRmbSearchTime;
    for(cnt = 0; cnt < NU_CONT_MAXCONTROLLERS; cnt++){
	rmbCtl = &nuContRmbCtl[cnt];
	
	switch(rmbCtl->mode){
	case NU_CONT_RMB_MODE_DISABLE:	/* Disable the oscillating pack  */
	    break;
	    
	case NU_CONT_RMB_MODE_ENABLE:	/* The state of enabling the oscillating pack  */
	    rtn = contRmbControl(rmbCtl, cnt);	/* Control the oscillating pack  */

	    /* Don't try to use if errors occur */
	    if(rtn){
		rmbCtl->mode = NU_CONT_RMB_MODE_DISABLE;
	    }
	    break;
	    
	case NU_CONT_RMB_MODE_AUTORUN:  /* The auto distinction of the oscillating pack */

	    if(rmbCtl->autorun == NU_CONT_RMB_AUTO_SEARCH){
		/* In the case of the search mode, when it becomes the counter */
		/* value, check if there is the oscillating pack */
		counter =  rmbCtl->counter % nuContRmbSearchTime;
		if(!counter){
		    rtn = osMotorInit(&nuSiMesgQ, &nuContPfs[cnt], cnt);
		    if(!rtn) {
			rmbCtl->autorun = NU_CONT_RMB_AUTO_FIND;
			rmbCtl->type = NU_CONT_PAK_TYPE_RUMBLE;
		    }
		}
		rmbCtl->counter++;
	    } else {
		/* If there is the oscillating pack, it provides the same process */
		/*	as the enabled state.  However, if there are any errors, move */
		/*	to the search mode. */
		    rtn = contRmbControl(rmbCtl, cnt);
		if(rtn){
		    rmbCtl->counter = cnt;
		    rmbCtl->autorun = NU_CONT_RMB_AUTO_SEARCH;
		    rmbCtl->type = NU_CONT_PAK_TYPE_NONE;
		}
	    }
	    break;
	case (NU_CONT_RMB_MODE_ENABLE | NU_CONT_RMB_MODE_PAUSE):
	case (NU_CONT_RMB_MODE_AUTORUN | NU_CONT_RMB_MODE_PAUSE):
	    if(rmbCtl->type == NU_CONT_PAK_TYPE_RUMBLE){
		rtn = contRmbControl(rmbCtl, cnt);
	    }
	    break;
	}
    }
}

/*----------------------------------------------------------------------*/
/*	contRmbCheckMesg - Check the oscillating pack 				*/
/*	IN:	mesg	The message pointer 						*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
static void contRmbCheckMesg(NUContRmbMesg* mesg)
{
    s32		rtn;
    
    rtn = osMotorInit(&nuSiMesgQ, &nuContPfs[mesg->contNo], mesg->contNo);
    mesg->error = rtn;
    osSendMesg(mesg->rtnMesgQ, NULL, OS_MESG_BLOCK);
}

/*----------------------------------------------------------------------*/
/*	contRmbForceStopMesg - Force to stop the oscillating pack 		*/
/*	IN:	mesg	The message pointer 						*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
static void contRmbForceStopMesg(NUContRmbMesg* mesg)
{
    u32 cnt;
    for(cnt = 0; cnt < NU_CONT_MAXCONTROLLERS; cnt++){
	nuContRmbCtl[cnt].state = NU_CONT_RMB_STATE_FORCESTOP;
	nuContRmbCtl[cnt].mode |= NU_CONT_RMB_MODE_PAUSE;
    }
    osSendMesg(mesg->rtnMesgQ, NULL, OS_MESG_BLOCK);
}
/*----------------------------------------------------------------------*/
/*	contRmbForceStopEndMesg - Cancel forced stop of oscillating pack	*/
/*	IN:	mesg	The message pointer 						*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
static void contRmbForceStopEndMesg(NUContRmbMesg* mesg)
{
    u32 cnt;
    for(cnt = 0; cnt < NU_CONT_MAXCONTROLLERS; cnt++){
	nuContRmbCtl[cnt].mode &= 0x7f;
    }
    osSendMesg(mesg->rtnMesgQ, NULL, OS_MESG_BLOCK);
}

/*----------------------------------------------------------------------*/
/*	contRmbStartMesg - Start the operation of the oscillating pack. 	*/
/*	IN:	mesg	The message pointer 						*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
static void contRmbStartMesg(NUContRmbMesg* mesg)
{
    NUContRmbCtl*	rmbCtlData;
    
    rmbCtlData = mesg->data;
    nuContRmbCtl[mesg->contNo].state   = rmbCtlData->state;
    nuContRmbCtl[mesg->contNo].frame   = rmbCtlData->frame;
    nuContRmbCtl[mesg->contNo].freq    = rmbCtlData->freq;
    nuContRmbCtl[mesg->contNo].counter = 0;
    osSendMesg(mesg->rtnMesgQ, NULL, OS_MESG_BLOCK);
}

/*----------------------------------------------------------------------*/
/*	contRmbMgr - The oscillating pack manager 				*/
/*	IN:	None										*/
/*	RET:	0:  Process continues, 1:  Process ends				*/
/*----------------------------------------------------------------------*/
s32 nuContRmbMgr(void* mesg)
{
    NUScMsg*	mesg_type;

    mesg_type = (NUScMsg*)mesg;

    /* Divide the process depending on messages */
    switch(*mesg_type){

    case NU_SC_RETRACE_MSG:		/* The retrace message */
	contRmbRetrace();		/* The process of each retrace of the oscillating pack */
	/* In case of retrace message, will not end */
	return NU_SI_CALLBACK_CONTINUE;
	
    case NU_CONT_RMB_CHECK_MSG:		/* Check if the oscillating pack is put in */
	contRmbCheckMesg((NUContRmbMesg*)mesg);
	break;
    case NU_CONT_RMB_START_MSG:		/* Start the operation of the oscillating pack */
	contRmbStartMesg((NUContRmbMesg*)mesg);
	break;
    case NU_SC_PRENMI_MSG:		/* The PRE NMI message */
    case NU_CONT_RMB_FORCESTOP_MSG:	/* Force to stop */
	contRmbForceStopMesg((NUContRmbMesg*)mesg);
	break;
    case NU_CONT_RMB_FORCESTOPEND_MSG:	/* Cancel the forced stop */
	contRmbForceStopEndMesg((NUContRmbMesg*)mesg);
	break;
    default:
	/* As message is unknown, leave for other callback function */
	return NU_SI_CALLBACK_CONTINUE;
    }
    
    /* Default:  end callback function loop */
    return NU_SI_CALLBACK_END;
    
}
