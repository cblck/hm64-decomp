#ifndef _GAME_AUDIO_H_
#define _GAME_AUDIO_H_

#include "common.h"
#include "system/audio.h"

#define OPENING 0
#define TITLE_THEME 1
#define SPORTS_FESTIVAL 2
#define GOODBYE 3
#define SPRING_THEME 4
#define SUMMER_THEME 5
#define AUTUMN_THEME 6
#define WINTER_THEME 7
#define NAMING_SCREEN_THEME 8
#define FESTIVAL_THEME 9
#define SPORTS_FESTIVAL_THEME_2 10
#define SPIRIT_FESTIVAL_THEME 11
#define DOG_RACE_THEME 12
#define RACING_FESTIVAL_THEME 13
#define DANCE_THEME 14
#define MOUNTAIN_THEME 15
#define VILLAGE_THEME 16
#define POND_THEME 17
#define CHURCH_THEME 18 
#define TAVERN_THEME 19
#define WEDDING_THEME 20
#define NEW_YEAR_SUNRISE_THEME 21
#define CREDITS_THEME 22
#define BROKEN_MUSIC_BOX_THEME 23
#define MUSIC_BOX_THEME 24
#define FLUTE_PERFORMANCE 25
#define ORGAN_PERFORMANCE 26
#define OCARINA_PERFORMANCE 27
#define BAD_ENDING_THEME_1 28
#define BAD_ENDING_THEME_2 29
#define VOCAL_PERFORMANCE 30
#define ENSEMBLE_PERFORMANCE 31
#define DRUM_PERFORMANCE 32
#define TYPHOON_AMBIENCE_1 33
#define TYPHOON_AMBIENCE_2 34
#define NIGHT_AMBIENCE_SPRING 35
#define NIGHT_AMBIENCE_SUMMER 36
#define NIGHT_AMBIENCE_AUTUMN 37
#define STARRY_NIGHT_FESTIVAL_THEME 38
#define BEACH_AMBIENCE_1 39
#define BEACH_AMBIENCE_2 40

// TODO: finish adding sfx
#define SELECT_1 0
#define CLOSE 1
#define MOVE_CURSOR 2
#define INVALID_BUZZ 3
// shipping bin
#define ITEM_PLUCK 4
// 5 and 6 = gallop
#define SELECT_2 7
// pause screen, dialogue box
#define MENU_OPEN 8
#define WHISTLE 10
#define SICKLE_SFX 11
#define AX_SFX 17
#define SEEDS 29
#define MILKER_SFX 30
#define RAIN_SFX 32
#define TYPHOON_SFX 34
#define TV_OFF 47
#define DOOR_OPEN 50
// 0x3F set by func_80056030
#define ROOSTER 63
#define CHICKEN_CLUCK 64
#define BIRD_CHIRP 66
#define CAT_MEOW 67
#define BIRD_CHIRP_2 70
#define DIALOGUE_BEEP 87
#define RUMBLE 89


void initializeWaveTable(u16 waveTableIndex);
void setCurrentSong(u16 songIndex);
void stopCurrentSong(u16 songIndex);
void setSongWithDefaultSpeed(u16 songIndex);
u8 checkDefaultSongChannelOpen(u16 songIndex);
void setSongVolume(u16 songIndex, u32 arg1);
void func_800ACC1C(u16 songIndex);
void setAudio(u16 index);
u8 checkAllSfxInactive(void);

extern int musPriorityList[];
extern u8 *sfxList[];

#endif