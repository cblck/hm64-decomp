/*======================================================================*/
/*		NuSYS										*/
/*		nucontpakgetfree.c							*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontpakgetfree.c,v 1.3 1997/12/05 13:00:04 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContPakGetFree - Get the space capacity of the controller pack 	*/
/* 	Get the space capacity of the controller pack. 				*/
/*	If it is the error, the error code enters into "error" 		*/
/*	of NUContPakFile.									*/
/*	IN:	*file	The controller pack structure 				*/
/*	RET:	Return the space capacity. 						*/
/*----------------------------------------------------------------------*/
s32 nuContPakGetFree(NUContPakFile *file)
{
    OSMesgQueue		rtnMesgQ;
    OSMesg		rtnMesgBuf;
    NUContPakFreeMesg	freeMesg;

    if(nuContPakCallBack.func == NULL){
#ifdef NU_DEBUG
	osSyncPrintf("nuContPakGetFree: no contpak manager!!\n");
#endif /* NU_DEBUG */
	return 0;
    }
	
    osCreateMesgQueue(&rtnMesgQ, &rtnMesgBuf, 1);
    
    freeMesg.mesg     = NU_CONT_PAK_FREE_MSG;
    freeMesg.file     = file;
    freeMesg.rtnMesgQ = &rtnMesgQ;
    osSendMesg(&nuSiMgrMesgQ, (OSMesg*)&freeMesg, OS_MESG_BLOCK);
    osRecvMesg(&rtnMesgQ, NULL, OS_MESG_BLOCK);
    return freeMesg.size;
}
