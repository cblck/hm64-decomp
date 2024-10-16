/*======================================================================*/
/*		NuSYS										*/
/*		nuaustlplayerinit.c							*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*======================================================================*/
/* Modified version to match HM64.                                      */
#include <nusys.h>
#include <nualstl.h>

extern void* D_8018965C; // TODO: Find real name.
extern void* D_801897E0; // TODO: Find real name.

/*----------------------------------------------------------------------*/
/*	nuAuStlPlayerInit - Initialize sequence player				*/
/*	IN:	*config	N64 Sound Tools player set structure		*/
/*		size	    	buffer size to state sequence data			*/
/*	RET:	None										*/
/*----------------------------------------------------------------------*/
void nuAuStlPlayerInit(musConfig *c, u32 size)
{
    MusInitialize(c);
    nuAuSeqPlayer[NU_AU_SEQ_PLAYER0].data_ptr = alHeapAlloc(&nuAuHeap, 1, size);
    nuAuSeqPlayer[NU_AU_SEQ_PLAYER1].data_ptr = alHeapAlloc(&nuAuHeap, 1, size); // HM64: Added this.
	D_8018965C = alHeapAlloc(&nuAuHeap, 1, size); // HM64: Added this.
	D_801897E0 = alHeapAlloc(&nuAuHeap, 1, size); // HM64: Added this.
}
