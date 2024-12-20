/*======================================================================*/
/*		NuSYS										*/
/*		nuauprenmifuncset.c							*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*======================================================================*/
#include <nusys.h>
#include <nualstl_n.h>

/*----------------------------------------------------------------------*/
/*	nuAuPreNMIFuncSet - Register PRE NMI processing callback function	*/
/*	IN:	pointer of callback function						*/
/*	RET:	None										*/
/*----------------------------------------------------------------------*/
void nuAuPreNMIFuncSet(NUAuPreNMIFunc func)
{
    OSIntMask	mask;
    
    mask = osSetIntMask(OS_IM_NONE);		/* Interruption not allowed */
    nuAuPreNMIFunc = func;
    osSetIntMask(mask);
}
