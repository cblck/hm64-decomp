#include "common.h"
/*====================================================================
 * envmixer.c
 *
 * Copyright 1993, Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Silicon Graphics,
 * Inc.; the contents of this file may not be disclosed to third
 * parties, copied or duplicated in any form, in whole or in part,
 * without the prior written permission of Silicon Graphics, Inc.
 *
 * RESTRICTED RIGHTS LEGEND:
 * Use, duplication or disclosure by the Government is subject to
 * restrictions as set forth in subdivision (c)(1)(ii) of the Rights
 * in Technical Data and Computer Software clause at DFARS
 * 252.227-7013, and/or in similar or successor clauses in the FAR,
 * DOD or NASA FAR Supplement. Unpublished - rights reserved under the
 * Copyright Laws of the United States.
 *====================================================================*/
#include <libaudio.h>
#include "synthInternals.h"
#include <os.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
// TODO: these come from headers
#ident "$Revision: 1.49 $"
#ident "$Revision: 1.17 $"
#ifdef AUD_PROFILE
extern u32 cnt_index, env_num, env_cnt, env_max, env_min, lastCnt[];
extern u32 rate_num, rate_cnt, rate_max, rate_min;
extern u32 vol_num, vol_cnt, vol_max, vol_min;
#endif

INCLUDE_ASM(const s32, "lib/os/libultra/audio/env", alEnvmixerPull);

INCLUDE_ASM(const s32, "lib/os/libultra/audio/env", alEnvmixerParam);

INCLUDE_ASM(const s32, "lib/os/libultra/audio/env", _pullSubFrame);

INCLUDE_ASM(const s32, "lib/os/libultra/audio/env", _frexpf);

INCLUDE_ASM(const s32, "lib/os/libultra/audio/env", _ldexpf);

INCLUDE_ASM(const s32, "lib/os/libultra/audio/env", _getRate);

INCLUDE_ASM(const s32, "lib/os/libultra/audio/env", _getVol);

INCLUDE_RODATA(const s32, "lib/os/libultra/audio/env", env_data_0000);
