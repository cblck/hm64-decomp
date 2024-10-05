#include "common.h"
////////////////////////////////////////////////
#include "PR/os_internal.h"
#include "dbgproto.h"
#include "PR/rcp.h"
#include "PR/sptask.h"
#include "PRinternal/rmonint.h"

#include "macros.h"

// TODO: this comes from a header
#if BUILD_VERSION >= VERSION_J
#ident "$Revision: 1.4 $"
#else
#ident "$Revision: 3.70 $"
#ident "$Revision: 1.5 $"
#ident "$Revision: 1.2 $"
#ident "$Revision: 1.4 $"
#ident "$Revision: 1.3 $"
#endif

#define TMP_BP 0
#define NUM_BREAKPOINTS 16

typedef struct {
	TVushort type;
	TVushort response;
	TVid threadID;
	void* pc;
} TVExceptionReplyMsg;

typedef struct {
	u32* breakAddress;
	u32 oldInstruction;
} BREAKINFO;

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonbrk", __rmonSetBreak);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonbrk", _rmonListBreak);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonbrk", __rmonClearBreak);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonbrk", __rmonGetBranchTarget);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonbrk", __rmonSetSingleStep);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonbrk", __rmonGetExceptionStatus);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonbrk", __rmonHitBreak);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonbrk", __rmonHitSpBreak);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonbrk", __rmonHitCpuFault);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonbrk", rmonFindFaultedThreads);
