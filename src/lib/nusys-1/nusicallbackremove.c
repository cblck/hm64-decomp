/*======================================================================*/
/*		NuSYS										*/
/*		nusicallbackremove.c							*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/
/* Ver 1.1	97/12/3		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
/* $Id: nusicallbackremove.c,v 1.2 1997/12/05 12:36:41 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>
/*----------------------------------------------------------------------*/
/* nuSiCallBackRemove - Delete call back function from SI manager   	*/
/* Deletes call back function allocated to call back function list from */
/* list.  If not allocated, it does nothing.  		    			*/
/*	IN:	*list    Pointer of call back function list structure		*/
/*	RTN:	none										*/
/*----------------------------------------------------------------------*/
void nuSiCallBackRemove(NUCallBackList* list)
{
    OSIntMask		mask;
    NUCallBackList*	siCallBackListPtr;
    NUCallBackList*	forwardPtr;


    /* Look for the function in the list*/
    siCallBackListPtr = nuSiCallBackList.next;
    forwardPtr = &nuSiCallBackList;

    while(siCallBackListPtr){

	if(siCallBackListPtr->func == list->func){
	    mask = osSetIntMask(OS_IM_NONE);
	    forwardPtr->next = list->next;
	    list->next = NULL;
	    list->func = NULL;
	    osSetIntMask(mask);
	    break;
	}
	forwardPtr = siCallBackListPtr;
	siCallBackListPtr = siCallBackListPtr->next;
    }
}
