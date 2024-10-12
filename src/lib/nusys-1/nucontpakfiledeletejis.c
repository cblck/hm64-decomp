/*======================================================================*/
/*		NuSYS										*/
/*		nucontpakfiledeletejis.c						*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontpakfiledeletejis.c,v *Modified version of 1.3* $          */
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContPakFileDeleteJis - Delete the controller pack note 		*/
/* 	Open the file of the controller pack 					*/
/*	IN:	*file	The controller pack structure 				*/
/*		*noteName The note name (JIS)	The termination-NULL		*/
/*			Converted JIS->N64code 						*/
/*		*extName  The extension (JIS)	The termination-NULL		*/
/*			Converted JIS->N64code 						*/
/*			Converted ASCII->N64code inside 				*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuContPakFileDeleteJis(NUContPakFile *file, u8* noteName, u8* extName)
{
    OSMesgQueue		rtnMesgQ;
    OSMesg		    rtnMesgBuf;
    NUContPakFileOpenMesg fopenMesg;
    u8			noteNameN64[32];
    u8			extNameN64[4];
    u32			len;

    if(nuContPakCallBack.func == NULL){
#ifdef NU_DEBUG
	osSyncPrintf("nuContPakFileDeleteJis: no contpak manager!!\n");
#endif /* NU_DEBUG */
	return;
    }
    
    bzero(noteNameN64, 32);
    bzero(extNameN64, 4);

    /* Get the number of characters */
    len = 0;
    while(noteName[len]) len++;
    
    /* The note name; converted JIS->N64code */
    nuContPakJisToN64(noteName, noteNameN64, len);

    /* The extension; converted JIS->N64code  */    
    nuContPakJisToN64(extName, extNameN64, 1);

    osCreateMesgQueue(&rtnMesgQ, &rtnMesgBuf, 1);
    fopenMesg.mesg     = NU_CONT_PAK_FILEDELETE_MSG;
    fopenMesg.file     = file;
    fopenMesg.rtnMesgQ = &rtnMesgQ;
    fopenMesg.noteName = noteNameN64;
    fopenMesg.extName  = extNameN64;
    
    osSendMesg(&nuSiMgrMesgQ, (OSMesg*)&fopenMesg, OS_MESG_BLOCK);
    osRecvMesg(&rtnMesgQ, NULL, OS_MESG_BLOCK);
}
