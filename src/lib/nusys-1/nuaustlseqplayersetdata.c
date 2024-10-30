#include "common.h"
/*======================================================================*/
/*		NuSYS										*/
/*		nuaustlseqplayersetdata.c						*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*======================================================================*/
#include <nusys.h>
#include <nualstl_n.h>

/*----------------------------------------------------------------------*/
/*	nuAuStlSeqPlayerSet - Read sequence of sequence player		*/
/*	IN:	seq_addr	ROM address of sequende data				*/
/*		seq_size	sequence data size					*/
/*	RET:	None										*/
/*----------------------------------------------------------------------*/
void nuAuStlSeqPlayerSetData(u8 *seq_addr, u32 seq_size)
{
    /* If size is odd, it can't be transferred by PI, so make it even */
    if(seq_size & 0x00000001) seq_size++;

    nuPiReadRom((u32)seq_addr, nuAuSeqPlayer[NU_AU_SEQ_PLAYER0].data_ptr, seq_size);
}

void nuAuStlSeqPlayerDataSet(u8 player_no, u8* seq_addr, u32 seq_size)
{
	/* If size is odd, it can't be transferred by PI, so make it even */
	if (seq_size & 0x00000001) seq_size++;

	nuPiReadRom((u32)seq_addr, (void*)nuAuSeqPlayer[player_no].data_ptr, seq_size);
}