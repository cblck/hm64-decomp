#include "common.h"
/*======================================================================*/
/*		NuSYS										*/
/*		nucontpakmgr.c								*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nucontpakmgr.c,v *Modified version 1.10.* $						*/
/*======================================================================*/

#include <nusys.h>
u16		nuContPakCompanyCode = 0;/* The company code of the controller pack */
u32		nuContPakGameCode = 0;	 /* The game code of the controller pack */

NUCallBackList	nuContPakCallBack = {NULL, NULL};

/*----------------------------------------------------------------------*/
/*	nuContPakMgrInit - Initialize the controller pack manager 		*/
/*	IN:	None 										*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuContPakMgrInit(void)
{

    if(nuContPakCallBack.func != NULL){
#ifdef NU_DEBUG
	osSyncPrintf("nuContPakMgrInit: controller pak manager already entry!!\n");
	return;
#endif /* NU_DEBUG */
    }

    /* Register the controller pack manager in SI manager */
    nuSiCallBackAdd(&nuContPakCallBack, nuContPakMgr);
}
/*----------------------------------------------------------------------*/
/*	nuContPakMgrRemove - Remove Controller Pak manager			*/
/* 		Detach Controller Pak manager from SI manager			*/
/*	IN:	None										*/
/*	RET:	None										*/
/*----------------------------------------------------------------------*/
void nuContPakMgrRemove(void)
{
    nuSiCallBackRemove(&nuContPakCallBack);
}

/*----------------------------------------------------------------------*/
/*	contPakOpen - Open the controller pack 					*/
/* 		Check device which sticks in connecter of controller and 	*/
/*		initialize to be able to use the device. 				*/
/*	IN:	*mesg	The pointer of the NUContPakOpenMesg structure 		*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
static void contPakOpen(NUContPakOpenMesg* mesg)
{

    s32			rtn;
    NUContPakFile*	file;
    
    file = mesg->file;
    file->type = NU_CONT_PAK_TYPE_NONE;
    
    /* Initialize the file handle */
    /* The time of osPfsInitPak */
    /* The controller pack-232ms, the oscillating pack-10ms and none-2ms */
    rtn = osPfsInitPak(&nuSiMesgQ, file->pfs, file->pfs->channel);
    
    if(rtn == 0) {
	/* The controller pack, in the case of no errors */
	file->type = NU_CONT_PAK_TYPE_PAK;
	
    }
    // HM64: Matching does not contain this check. Likely predates v1.10.
    // else if(rtn == PFS_ERR_ID_FATAL)
    // {
    //     /* Check, in case of PFS_ERR_ID_FATAL; it may be oscillating pack	*/
    //     /* time of osMotorInit */
    //     /* The oscillating pack-4ms and none-2ms */
    //     rtn = osMotorInit(&nuSiMesgQ, file->pfs, file->pfs->channel);
    //     if(rtn == 0){
    //         /* The oscillating pack */
    //         file->type = NU_CONT_PAK_TYPE_RUMBLE;
    //     } else if(rtn == PFS_ERR_DEVICE){
    //         /* If it is not oscillating pack, return PFS_ERR_ID_FATAL, */
    //         /* because it is PFS_ERR_ID_FATAL of controller pack	   */
    //         rtn = PFS_ERR_ID_FATAL;
    //     }
    //}

    file->error = rtn;
    /* Send the end message */
    osSendMesg(mesg->rtnMesgQ, NULL, OS_MESG_BLOCK);
}

/*----------------------------------------------------------------------*/
/*	contPakFree - Get the space capacity of the controller pack 	*/
/*	IN:	*mesg	The pointer of the NUContPakFreeMesg structure 		*/
/*	RET:	None 									*/
/*----------------------------------------------------------------------*/
static void contPakFree(NUContPakFreeMesg* mesg)
{
    s32 rtn;
    mesg->size = 0;
    rtn = osPfsFreeBlocks(mesg->file->pfs, &mesg->size);
    mesg->file->error = rtn;
    osSendMesg(mesg->rtnMesgQ, NULL, OS_MESG_BLOCK);
}

/*----------------------------------------------------------------------*/
/*	contPakFileReadWrite - Get space capacity of controller pack 	*/
/*	IN:	*mesg	Pointer of NUContPakFileReadWriteMesg structure 	*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
static void contPakFileReadWrite(NUContPakFileRWMesg* mesg)
{
    NUContPakFile*	file;
    s32			rtn;

    file = mesg->file;

    rtn = osPfsReadWriteFile(file->pfs, file->file_no, mesg->mode,
			     mesg->offset, mesg->size, mesg->buf);
    file->error = rtn;
    osSendMesg(mesg->rtnMesgQ, NULL, OS_MESG_BLOCK);
}

/*----------------------------------------------------------------------*/
/*	contPakFileOpen - Open the file of the controller pack 		*/
/*	IN:	*mesg	The pointer of the NUContPakFileOpenMesg structure 	*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
static void contPakFileOpen(NUContPakFileOpenMesg* mesg)
{

    s32			rtn;
    NUContPakFile*	file;

    file = mesg->file;

    rtn = osPfsFindFile(file->pfs, nuContPakCompanyCode, nuContPakGameCode,
		  mesg->noteName, mesg->extName, &file->file_no);

    /* In the case of that there are no files and "mode" is NU_CONT_PAK_MODE_CRATE */
    if((rtn == PFS_ERR_INVALID) && (mesg->mode == NU_CONT_PAK_MODE_CREATE)){
	rtn = osPfsAllocateFile(file->pfs,
				nuContPakCompanyCode, nuContPakGameCode,
				mesg->noteName, mesg->extName, mesg->size,
				&file->file_no);
    }
    file->error = rtn;	
    osSendMesg(mesg->rtnMesgQ, NULL, OS_MESG_BLOCK);
}

/*----------------------------------------------------------------------*/
/*	contPakFileDelte - Delete the file 						*/
/*	IN:	*mesg	The pointer of the NUContPakFileOpenMesg structure 	*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
static void contPakFileDelete(NUContPakFileOpenMesg* mesg)
{
    s32			rtn;
    NUContPakFile*	file;

    file = mesg->file;
    
    rtn = osPfsDeleteFile(file->pfs,
			  nuContPakCompanyCode, nuContPakGameCode,
			  mesg->noteName, mesg->extName);
    file->error = rtn;	
    osSendMesg(mesg->rtnMesgQ, NULL, OS_MESG_BLOCK);
}

/*----------------------------------------------------------------------*/
/*	contPakFileState - Get file information 					*/
/*	IN:	*mesg	The pointer of the NUContPakFileState structure 	*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
static void contPakFileState(NUContPakFileStateMesg* mesg)
{
    s32			rtn;
    NUContPakFile*	file;

    file = mesg->file;
    rtn = osPfsFileState(file->pfs, file->file_no, mesg->state);
    
    file->error = rtn;	
    osSendMesg(mesg->rtnMesgQ, NULL, OS_MESG_BLOCK);
}

/*----------------------------------------------------------------------*/
/*	contPakFileNum - Get the note number of the pack 			*/
/*	IN:	*mesg	The pointer of the NUContPakFileNumMesg structure	*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
static void contPakFileNum(NUContPakFileNumMesg* mesg)
{
    s32	rtn;
    NUContPakFile*	file;

    file = mesg->file;
    rtn = osPfsNumFiles(file->pfs, mesg->max_files, mesg->used_files);
    file->error = rtn;	
    osSendMesg(mesg->rtnMesgQ, NULL, OS_MESG_BLOCK);
}

/*----------------------------------------------------------------------*/
/*	contPakRepairId - Repair the file system of the pack 			*/
/*	IN:	*mesg	The pointer of the NUContPakMesg structure 		*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
static void contPakRepairId(NUContPakMesg* mesg)
{
    s32			rtn;
    NUContPakFile*	file;

    file = mesg->file;
    rtn = osPfsRepairId(file->pfs);
    file->error = rtn;	
    osSendMesg(mesg->rtnMesgQ, NULL, OS_MESG_BLOCK);
}

#if 0
/*----------------------------------------------------------------------*/
/*	contPakReSize - Change the note size of the pack 			*/
/*	IN:	*mesg	The pointer of the NUContPakFileOpenMesg structure 	*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
static void contPakReSize(NUContPakFileOpenMesg* mesg)
{
    s32			rtn;
    NUContPakFile*	file;

    file = mesg->file;
    rtn = osPfsReSizeFile(file->pfs, nuContPakCompanyCode, nuContPakGameCode,
			  mesg->noteName, mesg->extName, mesg->size);
    file->error = rtn;
    osSendMesg(mesg->rtnMesgQ, NULL, OS_MESG_BLOCK);
}
#endif

/*----------------------------------------------------------------------*/
/*	contPakMgr - The controller pack manager 					*/
/*	IN:	None 										*/
/*	RET:	0:  Process continued, 1:  Process ended				*/
/*----------------------------------------------------------------------*/
s32 nuContPakMgr(void* mesg)
{
    NUScMsg*	mesg_type;

    mesg_type = (NUScMsg*)mesg;
    
    /* Divide the process depending on messages */
    switch(*mesg_type){
    case NU_CONT_PAK_OPEN_MSG:	/* Open the controller pack */
	contPakOpen((NUContPakOpenMesg*)mesg_type);
	break;
    case NU_CONT_PAK_FREE_MSG:	/* Get the space capacity of the controller pack */
	contPakFree((NUContPakFreeMesg*)mesg_type);
	break;
    case NU_CONT_PAK_FOPEN_MSG:	/* Open the file */
	contPakFileOpen((NUContPakFileOpenMesg*)mesg_type);
	break;
    case NU_CONT_PAK_READWRITE_MSG: /* Read/write the file */
	contPakFileReadWrite((NUContPakFileRWMesg*)mesg_type);
	break;
    case NU_CONT_PAK_FILEDELETE_MSG: /* Delete the file */
	contPakFileDelete((NUContPakFileOpenMesg*)mesg_type);
	break;
    case NU_CONT_PAK_FILESTATE_MSG:	/* Get file information */
	contPakFileState((NUContPakFileStateMesg*)mesg_type);
	break;
    case NU_CONT_PAK_FILENUM_MSG:	/* Get note information of the pack */
	contPakFileNum((NUContPakFileNumMesg*)mesg_type);
	break;
    case NU_CONT_PAK_REPAIRID_MSG:	/* Repair the file system of the pack */
	contPakRepairId((NUContPakMesg*)mesg_type);
	break;
#if 0
    case NU_CONT_PAK_RESIZE_MSG:	/* Change the file size */
	contPakReSize((NUContPakFileOpenMesg*)mesg_type);
	break;
#endif
    default:
	/* As message is unkown, leave it to other callback function */
	return NU_SI_CALLBACK_CONTINUE;

    }
    /* Default:  end callback function loop */
    return NU_SI_CALLBACK_END;

}
