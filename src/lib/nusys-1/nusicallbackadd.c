/*======================================================================*/
/*		NuSYS										*/
/*		nusicallbackadd.c								*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/
/* Ver 1.1	97/12/1		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nusicallbackadd.c,v 1.4 1997/12/05 12:59:57 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/* nuSiCallBackAdd - Allocate a call back function to SI manager 		*/
/*	IN:	*list	pointer of call back function list structure  		*/
/* 		*func	call back function pointer					*/
/*	RTN:none										*/
/*----------------------------------------------------------------------*/
void nuSiCallBackAdd(NUCallBackList* list, NUCallBackFunc func)
{
    OSIntMask		mask;
    NUCallBackList*	siCallBackListPtr;
    NUCallBackList*	LastPtr;

    /* Look for the end of the list*/
    siCallBackListPtr = &nuSiCallBackList;
    while(siCallBackListPtr){

	/* If same function is already allocated, it does not allocate it.   */
	if(siCallBackListPtr->func == func){
#ifdef NU_DEBUG
	    osSyncPrintf("nuSiCallBackAdd: CallBack is already added!!\n");
#endif /* NU_DEBUG */
	    return;
	}
	LastPtr = siCallBackListPtr;
	siCallBackListPtr = siCallBackListPtr->next;
    }
    
    mask = osSetIntMask(OS_IM_NONE);
    list->next = LastPtr->next;
    list->func = func;
    LastPtr->next = list;
    osSetIntMask(mask);
}
