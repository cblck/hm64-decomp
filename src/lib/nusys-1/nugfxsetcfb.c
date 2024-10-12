/*======================================================================*/
/*		NuSYS										*/
/*		nugfxsetcfb.c								*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/  
#include <nusys.h>

/*----------------------------------------------------------------------------*/
/*	Set the frame buffer 									*/
/*	IN:	**framebuf The pointer of the pointer array of the frame buffer	*/
/*		framebufnum The number of frame buffers 					*/
/*----------------------------------------------------------------------------*/
void nuGfxSetCfb(u16** framebuf, u32 framebufnum)
{
    nuGfxCfb	= framebuf;
    nuGfxCfbNum	= framebufnum;
    nuGfxCfbCounter = 0;
    nuGfxCfb_ptr = nuGfxCfb[0];
}
