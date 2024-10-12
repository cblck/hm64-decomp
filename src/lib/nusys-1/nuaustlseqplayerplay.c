#include "common.h"
/*======================================================================*/
/*		NuSYS										*/
/*		nuaustlseqplayerplay.c							*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*======================================================================*/
#include <nusys.h>
#include <nualstl.h>

/*----------------------------------------------------------------------------*/
/*	nuAuStlSeqPlayerPlay -Sequence reproduction of sequence data player	*/
/*	IN:	None											*/
/*	RET:	sequence handler									*/
/*----------------------------------------------------------------------------*/
u32 nuAuStlSeqPlayerPlay(void)
{
    return( MusStartSong(nuAuSeqPlayer[NU_AU_SEQ_PLAYER0].data_ptr) );
}

/*----------------------------------------------------------------------*/
/*	nuAuStlSeqPlayerPlay2 - Plays Sequence Player sequences	*/
/*	IN:	player_no		Sequence Player number		*/
/*	RET:	sequence handler					*/
/*----------------------------------------------------------------------*/
musHandle nuAuStlSeqPlayerPlay2(u8 player_no)
{
    return(MusStartSong(nuAuSeqPlayer[player_no].data_ptr));
}
