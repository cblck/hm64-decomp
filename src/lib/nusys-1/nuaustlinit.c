/*======================================================================*/
/*		NuSYS										*/
/*		nuaustlinit.c								*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*======================================================================*/
#include <nusys.h>
#include <nualstl_n.h>

ALSynConfig	auSynConfig = {
    NU_AU_SYN_VVOICE_MAX,	/* number of virtual voices (not used)	*/
    NU_AU_SYN_PVOICE_MAX,	/* number of physical voices 			*/
    NU_AU_SYN_UPDATE_MAX,	/* maximum changed value 			*/
    0,
    NULL,				/* callback function for DMA initialization 	*/
    NULL,				/* heap structure						*/
    NU_AU_OUTPUT_RATE,		/* reset output frequency using program 		*/
    AL_FX_BIGROOM,		/* sound effect				 		*/
    0,				/* custom effect						*/
};

ALSeqpConfig	auSeqpConfig = {
	NU_AU_SEQ_VOICE_MAX,
	NU_AU_SEQ_EVENT_MAX,
	NU_AU_SEQ_CHANNEL_MAX,
	0,
	NULL,
	NULL,
	NULL,
	NULL
};
ALSndpConfig	anSndpConfig = {
    NU_AU_SND_SOUND_MAX,
    NU_AU_SND_EVENT_MAX,
    NULL
};

/*----------------------------------------------------------------------*/
/*	nuAuInit - Initialize audio							*/
/*	IN:	head of buffer								*/
/*	RET:	None										*/
/*----------------------------------------------------------------------*/
void nuAuStlInit(musConfig *c)
{
    unsigned char *addr;
    unsigned long length;

    auSynConfig.outputRate = NU_AU_OUTPUT_RATE;
    c->channels = NU_AU_SYN_PVOICE_MAX;
    c->thread_priority = 12;

    addr = (void*)NU_AU_HEAP_ADDR;
    length = NU_AU_HEAP_SIZE;
    while(length--)
    	*addr++ = 0;

    /* Initialize audio manager */
    nuAuStlMgrInit((void*)NU_AU_HEAP_ADDR, NU_AU_HEAP_SIZE, &auSynConfig);

    /* Initialize player */
    nuAuStlPlayerInit(c, 0x8000);

    /* Register PRENMI function */
    nuAuPreNMIFuncSet(nuAuPreNMIProc);
}
