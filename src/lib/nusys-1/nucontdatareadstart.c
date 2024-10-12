/*======================================================================*/
/*		NuSYS										*/
/*		nucontdatareadstart.c							*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontdatareadstart.c,v 1.3 1997/12/06 08:36:08 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContDataReadStart - Start to read the controller data 		*/
/*	Send the message of data reading to the controller manager. 	*/
/* 	Because it is different from nuContDataRead and does not wait for	*/
/*	the reading end, be sure to check the reading end by 			*/
/* 	nuContDataReadWait.  It has used the wait message queue inside, 	*/
/*	which is the same as "retrace"; thus, you need to be careful of 	*/
/*	using nuContDataReadWait in separate threads. 				*/
/*	IN:	None 										*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuContDataReadStart(void)
{
    static NUScMsg	mesg =  NU_CONT_READ_NW_MSG;

    if(nuContCallBack.func == NULL){
#ifdef NU_DEBUG    
	osSyncPrintf("nuContDataReadStart: no controller manager!!\n");
#endif /* NU_DEBUG */
	return;
    }

    osSendMesg(&nuSiMgrMesgQ, (OSMesg*)&mesg, OS_MESG_BLOCK);
}
