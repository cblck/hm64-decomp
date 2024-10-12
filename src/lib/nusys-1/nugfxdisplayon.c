/*======================================================================*/
/*		NuSYS										*/
/*		nugfxdisplayon.c								*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	DISPLAY-ON										*/
/*	IN:	None										*/
/*	RET:	None										*/
/*----------------------------------------------------------------------*/
void nuGfxDisplayOn(void)		
{
    nuGfxDisplay = NU_GFX_DISPLAY_ON;
}
