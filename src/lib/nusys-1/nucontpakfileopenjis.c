/*======================================================================*/
/*		NuSYS										*/
/*		nucontpakfileopenjis.c							*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontpakfileopenjis.c,v 1.3 1997/12/05 13:04:16 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContPakFileOpenJis - Open the controller pack note 			*/
/* 	Open the file of the controller pack 					*/
/*	IN:	*file	The controller pack structure  				*/
/*		*noteName The note name (JIS)	The termination-NULL		*/
/*			Converted JIS->N64code						*/
/*		*extName  The extension (JIS)	The termination-NULL		*/
/*			Converted JIS->N64code 						*/
/*		mode	NU_CONT_PAK_CREATE  Create the note if no note there	*/
/*			NU_CONT_PAK_NOCREATE The error if there is no note 	*/
/*		size	Specify the note size 						*/
/*			Valid only when NU_CONT_PAK_CREATE is specified 	*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuContPakFileOpenJis(NUContPakFile *file, u8* noteName, u8* extName,
		       u32 mode, s32 size)
{
    OSMesgQueue		rtnMesgQ;
    OSMesg		rtnMesgBuf;
    NUContPakFileOpenMesg fopenMesg;
    u8			noteNameN64[32];
    u8			extNameN64[4];
    u32			len;

    if(nuContPakCallBack.func == NULL){
#ifdef NU_DEBUG
	osSyncPrintf("nuContPakFileOpenJis: no contpak manager!!\n");
#endif /* NU_DEBUG */
	return;
    }
    
    	/* It doesn't provide the process if nuContPakOpen doesn't find the */
	/* controller pack.  This is for avoiding osPfsFileFind hanging up  */
	/* because of no controller pack. 						  */
    if(file->type == NU_CONT_PAK_TYPE_NONE){
	file->error == PFS_ERR_NOPACK;
	return;
    }
    
    bzero(noteNameN64, 32);
    bzero(extNameN64, 4);

    /* Get the number of characters */
    len = 0;
    while(noteName[len]) len++;

#ifdef NU_DEBUG
    if(len > 16){
	osSyncPrintf("nuContPakFileOpenJis: Gamenote name(%d) is too long.(max 16 byte)\n", len);
	return;
    }
#endif /* NU_DEBUG */
    
    /* The note name; converted JIS->N64code */
    nuContPakJisToN64(noteName, noteNameN64, len);

    /* The extension; converted JIS->N64code  */    
    nuContPakJisToN64(extName, extNameN64, 1);

    osCreateMesgQueue(&rtnMesgQ, &rtnMesgBuf, 1);
    fopenMesg.mesg     = NU_CONT_PAK_FOPEN_MSG;
    fopenMesg.file     = file;
    fopenMesg.rtnMesgQ = &rtnMesgQ;
    fopenMesg.noteName = noteNameN64;
    fopenMesg.extName  = extNameN64;
    fopenMesg.size     = size;
    fopenMesg.mode     = mode;
    osSendMesg(&nuSiMgrMesgQ, (OSMesg*)&fopenMesg, OS_MESG_BLOCK);
    osRecvMesg(&rtnMesgQ, NULL, OS_MESG_BLOCK);
}
