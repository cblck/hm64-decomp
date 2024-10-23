/*======================================================================*/
/*		NuSYS										                    */
/*		nugfxthread.c								                    */
/*												                        */
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				            */
/*												                        */
/*----------------------------------------------------------------------*/    
/* Modified version 1.0; added nuGfxpending functionality.          	*/
/*======================================================================*/
#include <nusys.h>

NUGfxFunc	    nuGfxFunc = NULL;
NUGfxPreNMIFunc	nuGfxPreNMIFunc = NULL;
OSMesgQueue	    nuGfxMesgQ;

static OSMesg	nuGfxMesgBuf[NU_GFX_MESGS];
static OSThread	GfxThread;			            /* graphic thread */
static u64	    GfxStack[NU_GFX_STACK_SIZE/8];	/* gfx thread stack  */

/*-----------------------------------------------------------------------*/
/*	nuGfxThread - The graphic thread for the application 			     */
/*	IN:	*arg	The argument of osCreateThread.  Nothing in particular   */
/*	RET:	None										                 */
/*-----------------------------------------------------------------------*/
static void nuGfxThread(void *arg)
{
    u8          nuGfxpending;   // HM64: Added this variable and its handling.
    NUScClient	gfx_client;
    NUScMsg*	mesg_type;

    nuGfxpending = 0;
    nuGfxFunc = NULL;
    nuGfxPreNMIFunc = NULL;
    
    osCreateMesgQueue(&nuGfxMesgQ, nuGfxMesgBuf, NU_GFX_MESGS);

    /* Register as the RETRACE client to the scheduler */
    nuScAddClient(&gfx_client, &nuGfxMesgQ, NU_SC_RETRACE_MSG | NU_SC_PRENMI_MSG);

    while(1)
    {
	(void)osRecvMesg(&nuGfxMesgQ, (OSMesg*)&mesg_type, OS_MESG_BLOCK);
    
    switch(*mesg_type)
    {
	case NU_SC_RETRACE_MSG:	/* The retrace message */
    {
        if(!(nuGfxpending & 1))
        {
            /* Call the graphic function of the application side by the retrace message */
    	    /* nuGfxpending is the interruption over-flag. */
    	    if(nuGfxFunc != NULL)
            {
    		    (*nuGfxFunc)(nuGfxTaskSpool);
    	    }

            nuGfxpending = 0;
        }
        
        nuGfxpending++;
    }
	break;
	case NU_SC_PRENMI_MSG: /* The PRENMI message */
    {
        /* Call the call-back function in PRE NMI functions */
	    if(nuGfxPreNMIFunc != NULL)
        {
		    (*nuGfxPreNMIFunc)();
            
	    }
    }
	break;
	default:
		break;
	}
    }
}

/*----------------------------------------------------------------------*/
/*	nuGfxThreadStart - Activate the graphic thread 				*/
/*	IN:	None 										*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuGfxThreadStart(void)
{
    /* Activate the graphic thread */
    osCreateThread(&GfxThread, NU_GFX_THREAD_ID, nuGfxThread, (void*)NULL,
		           (GfxStack + NU_GFX_STACK_SIZE/8), NU_GFX_THREAD_PRI);
    osStartThread(&GfxThread);
}
