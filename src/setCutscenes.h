#ifndef _SET_CUTSCENES_H_
#define _SET_CUTSCENES_H_

#include "common.h"

#include "game.h"

#define MARRIED_TO_MARIA checkLifeEventBit(MARRIED) && gWife == MARIA
#define MARRIED_TO_POPURI checkLifeEventBit(MARRIED) && gWife == POPURI
#define MARRIED_TO_ELLI checkLifeEventBit(MARRIED) && gWife == ELLI
#define MARRIED_TO_ANN checkLifeEventBit(MARRIED) && gWife == ANN
#define MARRIED_TO_KAREN checkLifeEventBit(MARRIED) && gWife == KAREN
// TODO: clean up usage/make consistent
#define ARE_MARRIED checkLifeEventBit(MARRIED)
#define NOT_MARRIED !checkLifeEventBit(MARRIED)

// TODO: finish adding these
/* cutscene indices */
/* house cutscenes */
// = 0 different farm cutscenes with sub-indices: house construction, kent visit
// #define KENT_VISIT_PLAYER_SICK 0
#define MARIA_VISIT_PLAYER_SICK 3
#define MARIA_BIRTH 4
#define POPURI_BIRTH 0xB
#define ELLI_BIRTH 0x12
#define ANN_BIRTH 0x19
#define KAREN_BIRTH 0x20
#define MARIA_LEAVES_MARRIAGE 0x2E
#define POPURI_LEAVES_MARRIAGE 0x2F
#define ELLI_LEAVES_MARRIAGE 0x30
#define ANN_LEAVES_MARRIAGE 0x31
#define KAREN_LEAVES_MARRIAGE 0x32
// #define SICK_DAY_DEFAULT 0x33
/* farm cutscenes */
#define KENT_FARM_SOWING_FESITVAL 0x64
#define KENT_FARM_THANKSGIVING_FESTIVAL 0x67
#define KENT_AND_STU_FARM_PUPPIES 0x71
#define MAYOR_FARM_SOWING_FESTIVAL_YEAR_1 0x75
#define MAYOR_FARM_SOWING_FESTIVAL 0x76
#define MAYOR_FARM_HORSE_RACE 0x77
#define MAYOR_FARM_FIREFLY_FESTIVAL 0x78
#define MAYOR_FARM_SEA_FESTIVAL 0x79
#define MAYOR_FARM_HARVEST_FESTIVAL 0x7A
#define MAYOR_FARM_EGG_FESTIVAL 0x7B
#define MAYOR_FARM_SPIRIT_FESTIVAL_PLAYER_HARVEST_KING 0x7C 
#define MAYOR_FARM_SPIRIT_FESTIVAL 0x7D
#define DOUG_FARM_HORSE_RACE 0x7E
#define DOUG_FARM_DOG_RACE 0x80
#define DOUG_FARM_FAVOR 0x83
#define BASIL_FARM_FLOWER_FESTIVAL 0x89
#define BASIL_FARM_VEGETABLE_FESTIVAL 0x8A
#define BASIL_INTRO 0x8C
#define BASIL_POWER_NUT_GIFT 0x8D
#define BASIL_RETURN 0x8E
#define BASIL_LEAVING 0x8F
#define MARIA_FARM_GIFT_CHOCOLATE 0x90
#define MARIA_FARM_GIFT_RICE_BALLS 0x91
#define MARIA_FIREWORKS_INVITATION 0x92
#define MARIA_THANKSGIVING_GIFT 0x94
#define POPURI_FARM_GIFT_TEA 0x9A
#define POPURI_FARM_GIFT_COOKIES 0x9B
#define POPURI_FIREWORKS_INVITATION 0x9D
#define POPURI_THANKSGIVING_GIFT 0x9F
#define ELLI_FIREWORKS_INVITATION 0xA7
#define ELLI_THANKSGIVING_GIFT 0xAB
#define ANN_FIREWORKS_INVITATION 0xB4
#define ANN_THANKSGIVING_GIFT 0xB6
#define KAREN_FIREWORKS_INVITATION 0xC0
#define KAREN_THANKSGIVING_GIFT 0xC3
#define CLIFF_INTRO 0xD1
#define CLIFF_LEAVING_DEFAULT 0xD4
#define DOUG_DROPPING_OFF_COWS 0xED
#define MISS_MANA_PICKUP_COWS 0xEE

#define MOUNTAIN_FIREWORKS 0x130
#define ELLI_SPRAINED_ANKLE 0x157
#define MARIA_WEDDING 0x190
#define POPURI_WEDDING 0x191
#define ELLI_WEDDING 0x192
#define ANN_WEDDING 0x193
#define KAREN_WEDDING 0x194
#define ANN_AND_RICK 0x1A6
#define STARRY_NIGHT_FESTIVAL 0x1AC 
#define CHURCH_NEW_YEARS 0x1B3
#define POPURI_SICK 0x1B9
#define THANKSGIVING_MAYOR_HOUSE 0x209
#define MARIA_SPRAINED_ANKLE 0x20D
// 0x20F library cutscenes
#define MARIA_SICK 0x212
#define MARIA_REPAIR_MARRIAGE 0x213
#define SQUARE_FIREWORKS 0x258
#define ANN_SICK_VISIT_CUTSCENE 0x29A
#define ANN_REPAIR_MARRIAGE 0x29B
#define KAREN_YELLOW_HEART_EVENT_1 0x2C1
#define KAREN_YELLOW_HEART_EVENT_4 0x2C4
#define KAREN_SICK 0x2C7
#define BEACH_FIREWORKS 0x2EE
#define KAREN_AND_ELLI 0x2F2
#define KAREN_PINK_HEART_EVENT 0x2F3
#define SHADY_SALESMAN_AND_MARIA_1 0x322
#define SHADY_SALESMAN_AND_MARIA_2 0x323
#define ANN_SPRAINED_ANKLE 0x324
#define SOWING_FESTIVAL_POTION_SHOP_DEALER 0x352
#define SOWING_FESTIVAL_PLAYER 0x353
#define SOWING_FESTIVAL_HARRIS 0x354
#define HORSE_RACE_SQUARE 0x384
#define HORSE_RACE_STARTING_ANNOUNCEMENT 0x385
#define FLOWER_FESTIVAL 0x3B6
#define VEGETABLE_FESTIVAL_VILLAGE 0x3E8
#define VEGETABLE_FESTIVAL_SQUARE 0x3E9
#define FIREFLY_FESTIVAL 0x41A
#define SEA_FESTIVAL 0x44C
#define COW_FESTIVAL 0x47E
#define HARVEST_FESTIVAL 0x4B0
#define EGG_FESTIVAL 0x4E2
#define DOG_RACE_SQUARE 0x514
#define DOG_RACE_AFTER_RACE 0x515
#define SPIRIT_FESTIVAL 0x547
#define NEW_YEAR_FESTIVAL 0x578
#define MAYOR_VILLAGE_TOUR 0x5AD
#define INTRO 0x5B0
#define EVALUATION 0x60E

#define DEMO_CUTSCENE_1 0x5DC
#define DEMO_CUTSCENE_2 0x5DD
// TODO: 0x5DE missing?
#define DEMO_CUTSCENE_3 0x5DF

#define HOW_TO_PLAY_CUTSCENE 0x640

u16 func_8009C054(u16 mapIndex);
extern u16 func_800A7AE8(u8);  
extern u16 func_800A7DFC();        
extern u16 func_800A87C4();   
extern void func_800A8F74();   
extern void loadCutscene(u32);  

// maybe game.c bss
extern u16 gCutsceneIndex;

extern u8 D_80113580[0x18][0x14];

// flags
extern s32 D_801891D4;
// cutscene location
extern u16 D_8018981C;

#endif