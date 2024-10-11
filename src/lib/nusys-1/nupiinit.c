/*======================================================================*/
/*		NuSYS										*/
/*		nupiint.c									*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*----------------------------------------------------------------------*/
/* $Id: nupiinit.c,v 1.2 1997/11/17 06:13:47 ohki Exp $									*/
/*======================================================================*/
#include <nusys.h>

static OSMesgQueue PiMesgQ;			/* PI message queue  */
static OSMesg PiMesgBuf[NU_PI_MESG_NUM];	/* PI message buffer */

/*----------------------------------------------------------------------*/
/*	nuPiInit  - Initialize PI and around 					*/
/*	DMA-transfer the data from ROM. 						*/
/*	The message queue is the local variable to be able to be used  	*/
/*  between threads.                               				*/
/*	IN:	The head pointer of the heap buffer 				*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuPiInit(void)
{
            /* Create the PI manager  */
    osCreatePiManager((OSPri)OS_PRIORITY_PIMGR, &PiMesgQ, PiMesgBuf,
		      NU_PI_MESG_NUM);
}
