#include "common.h"
//
#include "PR/os_version.h"

#if BUILD_VERSION < VERSION_J
#ident "$Revision: 3.70 $"
#ident "$Revision: 1.5 $"
#ident "$Revision: 1.2 $"
#ident "$Revision: 1.4 $"
#ident "$Revision: 1.3 $"
#endif

#ifndef _FINALROM

#include "dbgproto.h"
#include "PR/os_internal.h"
#include "PRinternal/rmonint.h"
#include "PR/rcp.h"
#include "PR/sptask.h"
#include "PR/rdb.h"

#include "macros.h"

// TODO: this comes from a header
#if BUILD_VERSION >= VERSION_J
#ident "$Revision: 1.4 $"
#endif

//TODO: Fix. int __rmonActive = FALSE;

static vu32 somethingToDo;
static u32 inbuffer[280] ALIGNED(0x10);
static u8 cmdinptr;
static u8 cmdoutptr;
static int state;
static char* inPointer;

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonmain", __rmonSendHeader);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonmain", __rmonSendReply);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonmain", __rmonSendData);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonmain", rmonMain);

INCLUDE_RODATA(const s32, "lib/os/libultra/rmon/rmonmain", __rmonActive);

#endif
