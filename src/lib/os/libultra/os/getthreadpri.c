#include "common.h"

extern OSThread* __osRunningThread;

//INCLUDE_ASM(const s32, "lib/os/libultra/os/getthreadpri", osGetThreadPri);

OSPri osGetThreadPri(OSThread* thread) {
	if (thread == NULL) {
		thread = __osRunningThread;
	}

	return thread->priority;
}