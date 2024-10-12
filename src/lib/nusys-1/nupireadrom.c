/*======================================================================*/
/*		NuSYS										*/
/*		nupireadrom.c								*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*----------------------------------------------------------------------*/
/* $Id: nupireadrom.c,v *Modified version of 1.6.* $		           	*/
/*======================================================================*/
#include <nusys.h>



/*----------------------------------------------------------------------*/
/*	nuPiReadRom  - DMA-transfer the data from ROM 				*/
/*	DMA-transfer the data from ROM. 						*/
/*	The message queue is the local variable so that it can be used 	*/
/*	between threads.									*/
/*	Already changed to EPI	 							*/
/*												*/
/*	IN:	The head pointer of the heap buffer 				*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuPiReadRom(u32 rom_addr, void* buf_ptr, u32 size)
{
    OSIoMesg	dmaIoMesgBuf;
    OSMesgQueue dmaMesgQ;
    OSMesg	dmaMesgBuf;
    OSPiHandle*	cartHandle;
    
#ifdef	USE_EPI
    
    cartHandle = osCartRomInit();

    /* Create message queue */
    osCreateMesgQueue(&dmaMesgQ, &dmaMesgBuf, 1);

    /* Nullify the CPU cache */
    osInvalDCache((void*)buf_ptr, (s32)size);

    dmaIoMesgBuf.hdr.pri      = OS_MESG_PRI_NORMAL;
    dmaIoMesgBuf.hdr.retQueue = &dmaMesgQ;
    dmaIoMesgBuf.dramAddr     = buf_ptr;
    dmaIoMesgBuf.devAddr      = (u32)rom_addr;
    dmaIoMesgBuf.size         = size;

    osEPiStartDma(cartHandle, &dmaIoMesgBuf, OS_READ);


    /* End standby */
    (void)osRecvMesg(&dmaMesgQ, &dmaMesgBuf, OS_MESG_BLOCK);
    
#else

     /* Nullify the CPU cache */
    osInvalDCache((void*)buf_ptr, (s32)size);

    /* Create the message queue */
    osCreateMesgQueue(&dmaMesgQ, &dmaMesgBuf, 1);
    

    /* Read DMA */
    osPiStartDma(&dmaIoMesgBuf, OS_MESG_PRI_NORMAL, OS_READ, rom_addr,
		 buf_ptr, size, &dmaMesgQ);

    /* Wait for the end */
    (void)osRecvMesg(&dmaMesgQ, NULL, OS_MESG_BLOCK);
#endif /* USE_EPI */
}

void nuPiReadRom2(u32 rom_addr, void* buf_ptr, u32 size)
{
	OSIoMesg	dmaIoMesgBuf;
	OSMesgQueue dmaMesgQ;
	OSMesg	dmaMesgBuf;
	OSPiHandle* cartHandle;

#ifdef	USE_EPI

	cartHandle = osCartRomInit();

	/* Create message queue */
	osCreateMesgQueue(&dmaMesgQ, &dmaMesgBuf, 1);

	/* Nullify the CPU cache */
	osInvalDCache((void*)buf_ptr, (s32)size);

	dmaIoMesgBuf.hdr.pri = OS_MESG_PRI_NORMAL;
	dmaIoMesgBuf.hdr.retQueue = &dmaMesgQ;
	dmaIoMesgBuf.dramAddr = buf_ptr;
	dmaIoMesgBuf.devAddr = (u32)rom_addr;
	dmaIoMesgBuf.size = size;

	osEPiStartDma(cartHandle, &dmaIoMesgBuf, OS_READ);


	/* End standby */
	(void)osRecvMesg(&dmaMesgQ, &dmaMesgBuf, OS_MESG_BLOCK);

#else

	/* Nullify the CPU cache */
	osInvalDCache((void*)buf_ptr, (s32)size);

	/* Create the message queue */
	osCreateMesgQueue(&dmaMesgQ, &dmaMesgBuf, 1);


	/* Read DMA */
	osPiStartDma(&dmaIoMesgBuf, OS_MESG_PRI_NORMAL, OS_READ, rom_addr,
		buf_ptr, size, &dmaMesgQ);

	/* Wait for the end */
	(void)osRecvMesg(&dmaMesgQ, NULL, OS_MESG_BLOCK);
#endif /* USE_EPI */
}
