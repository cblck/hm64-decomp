/*======================================================================*/
/*		NuSYS										*/
/*		nuaustlbankset.c								*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*======================================================================*/
#include <nusys.h>
#include <nualstl.h>

extern ptr_bank_t	*mus_init_bank;
extern ptr_bank_t	*mus_default_bank;

/*----------------------------------------------------------------------------*/
/*	nuAuStlBankSet - Set sound bank							*/
/*	IN:	pbk_addr	ROM address of pointer that refers to wave table	*/
/*		pbk_size	size of pointer that refers to wave table			*/
/*		wbk_addr	ROM address of wave table data				*/
/*	RET:	None											*/
/*----------------------------------------------------------------------------*/
void nuAuStlBankSet(u8 *pbk_addr, u32 pbk_size, u8 *wbk_addr)
{

    nuAuSeqBank_ptr = alHeapAlloc(&nuAuHeap, 1, pbk_size);
    nuPiReadRom((u32)pbk_addr, nuAuSeqBank_ptr, pbk_size);

    MusBankInitialize(nuAuSeqBank_ptr, wbk_addr);
    mus_default_bank = mus_init_bank = (ptr_bank_t *)nuAuSeqBank_ptr;
}
