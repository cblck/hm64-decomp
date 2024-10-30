/*======================================================================*/
/*		NuSYS										*/
/*		nuaustlsndplayerplay.c							*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*======================================================================*/
#include <nusys.h>
#include <nualstl_n.h>

/*----------------------------------------------------------------------*/
/*	nuAuStlSndPlayerPlay - Reproduce sound					*/
/*	IN:	sndNo		number of sounds to reproduce				*/
/*	RET:	sound handler								*/
/*----------------------------------------------------------------------*/
u32 nuAuStlSndPlayerPlay(u32 sndNo)
{
    return( MusStartEffect(sndNo) );
}

/*----------------------------------------------------------------------*/
/*	nuAuStlSndPlayerPlay2 - Reproduce sound(specify parameter)		*/
/*	IN:	sndNo		number of sounds to reproduce				*/
/*		volume	volume scale (0 - 0x100)				*/
/*		pan		pan range	 (0 - 0xFF)					*/
/*		restartflag	Flag to sound again overwritting or 		*/
/*				not when the same sound number is specified	*/
/*				(0:  don't overwrite;  other than 0:  overwrite	*/
/*		priority	priority of sound						*/
/*	RET:	sound handler								*/
/*----------------------------------------------------------------------*/
u32 nuAuStlSndPlayerPlay2(u32 sndNo, s32 volume, s32 pan, s32 restartflag, s32 priority)
{
    return( MusStartEffect2(sndNo, volume, pan , restartflag, priority) );
}
