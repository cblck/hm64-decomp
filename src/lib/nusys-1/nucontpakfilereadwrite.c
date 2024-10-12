/*======================================================================*/
/*		NuSYS										*/
/*		nucontpakfilereadwrite.c						*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontpakfilereadwrite.c,v 1.3 1997/12/05 13:04:11 ohki Exp $									*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContPakFileReadWrite - Read/write the controller pack file 	*/
/* 	Read/write the file of the controller pack 				*/
/*	IN:	*file	The controller pack structure 				*/
/*		offset	The offset position to read/write 			*/
/*		size	The size to read/write 						*/
/*		buf	Buffers to store the read data				*/
/*			or have the data to write					*/
/*		mode	Specify writing or reading 					*/
/*			NU_CONT_PAK_READ	Read data  					*/
/*			NU_CONT_PAK_WRITE	Write data  				*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuContPakFileReadWrite(NUContPakFile *file,
			    s32 offset, s32 size, u8* buf, u32 mode)
{
    OSMesgQueue	rtnMesgQ;
    OSMesg	rtnMesgBuf;
    NUContPakFileRWMesg rwMesg;

#ifdef NU_DEBUG
    if(size & 0x1f){
	osSyncPrintf("nuContPakFileReadWrite: data size(%X) must be  multiple 32.\n", size);
	return;
    }
    if(offset & 0x1f){
	osSyncPrintf("nuContPakFileReadWrite: data offset(%X) must be multiple 32.\n");
	return;
    }
#endif /* NU_DEBUG */
    if(nuContPakCallBack.func == NULL){
#ifdef NU_DEBUG
	osSyncPrintf("nuContPakFileReadWrite: no contpak manager!!\n");
#endif /* NU_DEBUG */
	return;
    }

    osCreateMesgQueue(&rtnMesgQ, &rtnMesgBuf, 1);
    rwMesg.mesg     = NU_CONT_PAK_READWRITE_MSG;
    rwMesg.file     = file;
    rwMesg.rtnMesgQ = &rtnMesgQ;
    rwMesg.offset   = offset;
    rwMesg.size     = size;
    rwMesg.mode     = mode;
    rwMesg.buf      = buf;

    osSendMesg(&nuSiMgrMesgQ, (OSMesg*)&rwMesg, OS_MESG_BLOCK);
    osRecvMesg(&rtnMesgQ, NULL, OS_MESG_BLOCK);
}
