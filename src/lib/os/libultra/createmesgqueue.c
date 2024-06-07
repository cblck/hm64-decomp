#include "common.h"

extern struct __osThreadTail __osThreadTail;

//INCLUDE_ASM(const s32, "lib/os/libultra/createmesgqueue", osCreateMesgQueue);

void osCreateMesgQueue(OSMesgQueue* mq, OSMesg* msg, s32 msgCount) {
	mq->mtqueue = (OSThread*)&__osThreadTail;
	mq->fullqueue = (OSThread*)&__osThreadTail;
	mq->validCount = 0;
	mq->first = 0;
	mq->msgCount = msgCount;
	mq->msg = msg;
}