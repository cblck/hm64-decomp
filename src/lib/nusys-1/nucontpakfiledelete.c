/*======================================================================*/
/*		NuSYS										*/
/*		nucontpakfiledelete.c							*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontpakfiledelete.c,v 1.2 1997/12/05 13:03:08 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContPakFileDelete - Delete the controller pack note 		*/
/* 	Open the file of the controller pack 					*/
/*	IN:	*file	The controller pack structure 				*/
/*		noteName 	The note name 						*/
/*		extName		The extension  					*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuContPakFileDelete(NUContPakFile *file, u8* noteName, u8* extName)
{
    OSMesgQueue		rtnMesgQ;
    OSMesg		rtnMesgBuf;
    NUContPakFileOpenMesg fopenMesg;
    

    if(nuContPakCallBack.func == NULL){
#ifdef NU_DEBUG
	osSyncPrintf("nuContPakFileDelete: no contpak manager!!\n");
#endif /* NU_DEBUG */
	return;
    }
	
    osCreateMesgQueue(&rtnMesgQ, &rtnMesgBuf, 1);
    
    fopenMesg.mesg     = NU_CONT_PAK_FILEDELETE_MSG;
    fopenMesg.file     = file;
    fopenMesg.rtnMesgQ = &rtnMesgQ;
    fopenMesg.noteName = noteName;
    fopenMesg.extName  = extName;
    osSendMesg(&nuSiMgrMesgQ, (OSMesg*)&fopenMesg, OS_MESG_BLOCK);
    osRecvMesg(&rtnMesgQ, NULL, OS_MESG_BLOCK);
}
