/*======================================================================*/
/*		NuSYS										*/
/*		nucontqueryread.c								*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontqueryread.c,v 1.4 1997/12/06 08:46:47 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>


/*----------------------------------------------------------------------*/
/*	nuContQueryRead - Get the state of the controller 			*/
/*	IN:	None 										*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuContQueryRead(void)
{

    NUContQueryMesg	queryMesg;
    OSMesgQueue		rtnMesgQ;
    OSMesg		rtnMesgBuf;

    if(nuContCallBack.func == NULL){
#ifdef NU_DEBUG    
	osSyncPrintf("nuContQueryRead: no controller manager!!\n");
#endif /* NU_DEBUG */
	return;
    }

    osCreateMesgQueue(&rtnMesgQ, &rtnMesgBuf, 1);

    queryMesg.mesg	= NU_CONT_QUERY_MSG;
    queryMesg.rtnMesgQ  = &rtnMesgQ;
    
    osSendMesg(&nuSiMgrMesgQ, (OSMesg*)&queryMesg, OS_MESG_BLOCK);
    osRecvMesg(&rtnMesgQ, NULL, OS_MESG_BLOCK);
}
    
