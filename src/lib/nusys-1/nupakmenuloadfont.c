#include "common.h"
/*======================================================================*/
/*		NuSystem						*/
/*		nupakfontload.c						*/
/*									*/
/*		Copyright (C) 1998, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/    
/* 		98/11/11	Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/

#include <nusys.h>
#include "nupakmenu.h"
#include "pakmenu.h"

#ifdef	LANG_JAPANESE
#include "fontbmp.h"
#endif

typedef	struct{
    u8	code;
    u8	number;
} FTBL;

FTBL	canvas_ftbl[] = {
    { FONT_CODE_A-FONT_CODE_TOP, 26 },	/* A-Z			*/
    { FONT_CODE_0-FONT_CODE_TOP, 10 },	/* 0-9			*/
    { FONT_CODE_i-FONT_CODE_TOP, 14 },	/* !# '*+,-./:=?@// 	*/
    
#ifdef	LANG_JAPANESE
    { FONT_CODE_o-FONT_CODE_TOP, 58 },	/* KANA			*/
#endif	/* LANG_JAPANESE */
};

#ifdef LANG_JAPANESE
s8 font_kana_dot[] = {	/* font pattern of DAKUTEN,HANDAKUTEN	*/
/* 0123456789012345
   -------------XO-
   -----------XOXXO
   -----------XXOXX
   ------------XX-- */
  0x02,  0x09,  0x04,  0x00,
 ~0x04, ~0x16, ~0x1b, ~0x0c,

/* 0123456789012345
   ------------XOOO
   ------------XOXO
   ------------XOOO
   ------------XXXX */
  0x07,  0x05,  0x07,  0x00,
 ~0x08, ~0x0a, ~0x08, ~0x0f,
};
#endif /* LANG_JAPANESE */

extern u16	*__font_buf;

/*----------------------------------------------------------------------*/
/*	nuPakMenuLoadFont - Loads and deploys fonts			*/
/*	Deploys the character font loaded in RAM to the buffer.	*/
/*	Same as nosFontLoad.				*/
/*									*/
/*	IN:	font_buf		Buffer where the character font is deployed.	*/
/*				The following buffer sizes are required.	*/
/*				English	2112byte(NU_PAKMENU_FONTSIZE_E)	*/
/*				Japanese	4768byte(NU_PAKMENU_FONTSIZE_J)	*/
/*	RTN:	nothing							*/
/*----------------------------------------------------------------------*/
INCLUDE_ASM(const s32, "lib/nusys-1/nupakmenuloadfont", nuPakMenuLoadFont);

INCLUDE_ASM(const s32, "lib/nusys-1/nupakmenuloadfont", canvas_u8_to_str);

INCLUDE_ASM(const s32, "lib/nusys-1/nupakmenuloadfont", canvas_putc);

INCLUDE_ASM(const s32, "lib/nusys-1/nupakmenuloadfont", canvas_puts);
