/*======================================================================*/
/*		NuSYS										*/
/*		nucontpakfilestate.c							*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontpakfilestate.c,v 1.3 1997/12/05 13:03:39 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContPakFileState - 								*/
/* 	Get the state of the file of the controller pack 			*/
/*	IN:	*File	The controller pack structure 				*/
/*		*state	The pointer of the OSPfsState structure 		*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuContPakFileState(NUContPakFile *file, OSPfsState* state)
{
    OSMesgQueue		rtnMesgQ;
    OSMesg		rtnMesgBuf;
    NUContPakFileStateMesg stateMesg;

    if(nuContPakCallBack.func == NULL){
#ifdef NU_DEBUG
	osSyncPrintf("nuContPakFileState: no contpak manager!!\n");
#endif /* NU_DEBUG */
	return;
    }
    
    osCreateMesgQueue(&rtnMesgQ, &rtnMesgBuf, 1);
    stateMesg.mesg     = NU_CONT_PAK_FILESTATE_MSG;
    stateMesg.file     = file;
    stateMesg.rtnMesgQ = &rtnMesgQ;
    stateMesg.state    = state;
    
    osSendMesg(&nuSiMgrMesgQ, (OSMesg*)&stateMesg, OS_MESG_BLOCK);
    osRecvMesg(&rtnMesgQ, NULL, OS_MESG_BLOCK);
}
