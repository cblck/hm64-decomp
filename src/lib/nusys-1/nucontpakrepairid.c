/*======================================================================*/
/*		NuSYS										*/
/*		nucontpakrepairid.c							*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontpakrepairid.c,v 1.3 1997/12/05 13:03:49 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContPakRepairId - Repair the file system of the controller pack */
/*	Use it if PFS_ERR_ID_FATAL returns in osPfsInitPak. 			*/
/*	IN:	*file	The controller pack structure 				*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuContPakRepairId(NUContPakFile *file)
{
    OSMesgQueue		rtnMesgQ;
    OSMesg		rtnMesgBuf;
    NUContPakMesg 	repairMesg;

    if(nuContPakCallBack.func == NULL){
#ifdef NU_DEBUG
	osSyncPrintf("nuContPakRepairId: no contpak manager!!\n");
#endif /* NU_DEBUG */
	return;
    }
    
    osCreateMesgQueue(&rtnMesgQ, &rtnMesgBuf, 1);
    repairMesg.mesg       = NU_CONT_PAK_REPAIRID_MSG;
    repairMesg.file       = file;
    repairMesg.rtnMesgQ   = &rtnMesgQ;
    osSendMesg(&nuSiMgrMesgQ, (OSMesg*)&repairMesg, OS_MESG_BLOCK);
    osRecvMesg(&rtnMesgQ, NULL, OS_MESG_BLOCK);
}
