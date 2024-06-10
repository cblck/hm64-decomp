#ifndef _SPRITE_INDICES_H_
#define _SPRITE_INDICES_H_

/* characters */
#define PLAYER 0

/* title screen */
#define HARVEST_MOON_64_LOGO 0x42
#define MARVCOME_VICTOR_COPYRIGHT_INFO 0x43
#define DOG_TITLE_SPRITE 0x44
#define PUSH_THE_START_SIGN_2 0x45
#define PUSH_THE_START_SIGN_1 0x46
#define HOW_TO_PLAY_SIGN 0x47
#define DIRT_ROAD 0x48
#define SIGN_POST_1 0x49
#define SIGN_POST_2 0x4A
#define SIGN_POST_3 0x4B
#define FRONT_GRASS_2_1 0x4C
#define FAR_GRASS_3_1 0x4D
#define FAR_GRASS_2_1 0x4E
#define FRONT_GRASS_2_2 0x4F
#define BACK_GRASS_3_2 0x50
#define BACK_GRASS_2 0x51
#define FAR_GRASS_1_1 0x52
#define FAR_GRASS_1_2 0x53
#define CLOUD_1_1 0x54
#define CLOUD_1_2 0x55
#define CLOUD_2_1 0x56
#define CLOUD_3_1 0x57
#define LICENSED_BY_NINTENDO_1 0x58
#define LICENSED_BY_NINTENDO_2 0x59
#define CLOUD_3_2 0x5A
#define CLOUD_2_2 0x5B

/* dialogue */
#define DIALOGUE_ICONS_1 0x79
#define DIALOGUE_ICONS_2 0x7A
#define DIALOGUE_ICONS_3 0x7B

/* virtual addresses for dma */
#define SHADOW_TEXTURE_VADDR_START 0x80240300
#define SHADOW_TEXTURE_VADDR_END 0x80240800
#define SHADOW_INDEX_VADDR_START 0x80240900
#define SHADOW_INDEX_VADDR_END 0x80240A00

#define DIALOGUE_ICONS_TEXTURES_VADDR_START 0x8023B400
#define DIALOGUE_ICONS_TEXTURES_VADDR_END 0x8023CC00
#define DIALOGUE_ICONS_PALETTE_VADDR_START 0x8023CE00
#define DIALOGUE_ICONS_PALETTE_VADDR_END 0x8023D200

#define TITLE_SCREEN_TEXTURES_VADDR_START 0x80255000
#define TITLE_SCREEN_TEXTURES_VADDR_END 0x8026E100
#define TITLE_SCREEN_PALETTE_VADDR_START 0x8026E500
#define TITLE_SCREEN_PALETTE_VADDR_END 0x8026E900

#endif