/*======================================================================*/
/*		NuSYS										*/
/*		nucontpakopen.c								*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontpakopen.c,v 1.2 1997/12/05 13:03:43 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContPakOpen - Open the device of the controller pack 		*/
/* 	Open the device of the controller pack 					*/
/*	IN:	*file	The controller pack structure 				*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuContPakOpen(NUContPakFile *file, u32 contNo)
{
    OSMesgQueue		rtnMesgQ;
    OSMesg		rtnMesgBuf;
    NUContPakOpenMesg	openMesg;

    if(nuContPakCallBack.func == NULL){
#ifdef NU_DEBUG
	osSyncPrintf("nuContPakOpen: no contpak manager!!\n");
#endif /* NU_DEBUG */
	return;
    }
	
    osCreateMesgQueue(&rtnMesgQ, &rtnMesgBuf, 1);
    file->pfs = &nuContPfs[contNo];
    file->pfs->channel = contNo;

    openMesg.mesg     = NU_CONT_PAK_OPEN_MSG;
    openMesg.file     = file;
    openMesg.rtnMesgQ = &rtnMesgQ;

    osSendMesg(&nuSiMgrMesgQ, (OSMesg*)&openMesg, OS_MESG_BLOCK);
    osRecvMesg(&rtnMesgQ, NULL, OS_MESG_BLOCK);
    
}
