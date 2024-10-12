/*======================================================================*/
/*		NuSYS										*/
/*		nucontpakfilenum.c							*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontpakfilenum.c,v 1.3 1997/12/05 13:04:24 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContPakFileNum - Get the maximum and using note numbers of the 	*/
/*				controller pack 						*/
/*	IN:	*file	The controller pack structure 				*/
/*		*max_files	The maximum note number of the pack 		*/
/*		*used_files	The using note number of the pack 			*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuContPakFileNum(NUContPakFile *file, s32 *max_files, s32 *used_files)
{
    OSMesgQueue		rtnMesgQ;
    OSMesg		rtnMesgBuf;
    NUContPakFileNumMesg numMesg;

    if(nuContPakCallBack.func == NULL){
#ifdef NU_DEBUG
	osSyncPrintf("nuContPakFileNum: no contpak manager!!\n");
#endif /* NU_DEBUG */
	return;
    }
    
    osCreateMesgQueue(&rtnMesgQ, &rtnMesgBuf, 1);
    numMesg.mesg       = NU_CONT_PAK_FILENUM_MSG;
    numMesg.file       = file;
    numMesg.rtnMesgQ   = &rtnMesgQ;
    numMesg.max_files  = max_files;
    numMesg.used_files = used_files;
    osSendMesg(&nuSiMgrMesgQ, (OSMesg*)&numMesg, OS_MESG_BLOCK);
    osRecvMesg(&rtnMesgQ, NULL, OS_MESG_BLOCK);
}
