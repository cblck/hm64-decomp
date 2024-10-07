#include "common.h"
/*======================================================================*/
/*		NuSystem						*/
/*		nupakmenu.c						*/
/*									*/
/*		Copyright (C) 1998, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/
/* 		98/11/11	Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/

#include <nusys.h>
#include "nupakmenu.h"
#include "pakmenu.h"

extern	void	canvas_putc(int, u16, u16);
extern	void	canvas_puts(u8*, int, u16, u16);
extern	u16*	__cursor;
extern	void	canvas_u8_to_str(u16, u8*);

INCLUDE_RODATA(const s32, "lib/nusys-1/nupakmenu", D_80123840);

INCLUDE_RODATA(const s32, "lib/nusys-1/nupakmenu", D_80123848);

INCLUDE_RODATA(const s32, "lib/nusys-1/nupakmenu", D_80123854);

INCLUDE_ASM(const s32, "lib/nusys-1/nupakmenu", __display_term);

INCLUDE_ASM(const s32, "lib/nusys-1/nupakmenu", __display_free_block);

INCLUDE_ASM(const s32, "lib/nusys-1/nupakmenu", nuPakMenu);

INCLUDE_RODATA(const s32, "lib/nusys-1/nupakmenu", D_80123864);

INCLUDE_RODATA(const s32, "lib/nusys-1/nupakmenu", D_8012387C);

INCLUDE_RODATA(const s32, "lib/nusys-1/nupakmenu", D_80123898);

INCLUDE_RODATA(const s32, "lib/nusys-1/nupakmenu", D_801238B0);

INCLUDE_RODATA(const s32, "lib/nusys-1/nupakmenu", D_801238C4);

INCLUDE_RODATA(const s32, "lib/nusys-1/nupakmenu", D_801238DC);

INCLUDE_RODATA(const s32, "lib/nusys-1/nupakmenu", D_801238E8);
