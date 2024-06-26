#include "nusys.h"

extern OSThread idleThread;
extern OSThread mainThread;

extern u64	nuMainStack[NU_MAIN_STACK_SIZE / sizeof(u64)];
extern u64	IdleStack[NU_IDLE_STACK_SIZE / sizeof(u64)];

void (*nuIdleFunc)(void);

void idle(void* arg);

extern void mainproc(void* arg);


//INCLUDE_ASM(const s32, "ramMain", nuBoot);

static void nuBoot(void) {
	osInitialize();
	osCreateThread(&idleThread, NU_IDLE_THREAD_ID, &idle, (void*)NULL, (IdleStack + NU_IDLE_STACK_SIZE / 8), NU_MAIN_THREAD_PRI);
	osStartThread(&idleThread);
}

//INCLUDE_ASM(const s32, "ramMain", idle);

static void idle(void* arg) {

	nuIdleFunc = NULL;

	nuPiInit();
	nuScCreateScheduler(OS_VI_NTSC_LAN1, 1);
	osViSetSpecialFeatures(OS_VI_GAMMA_OFF | OS_VI_GAMMA_DITHER_OFF | OS_VI_DIVOT_ON | OS_VI_DITHER_FILTER_ON);
	osCreateThread(&mainThread, NU_MAIN_THREAD_ID, &mainproc, (void*)NULL, (nuMainStack + NU_MAIN_STACK_SIZE / sizeof(u64)), NU_MAIN_THREAD_PRI);
	osStartThread(&mainThread);
	osSetThreadPri(&idleThread, 0);

	while (1) {
		if (nuIdleFunc != NULL) {
			(*nuIdleFunc)();
		}
	}
}