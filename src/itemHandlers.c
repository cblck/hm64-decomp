#include "common.h"

#include "animals.h"
#include "npc.h"
#include "system/sprite.h"

// rodata
extern u16 itemFlags[];

extern u8 gBaseMapIndex;
extern u8 gSeason;

extern u8 gWife;

extern u8 npcAffection[];
extern npcInfo npcInfoArray[];

extern Player gPlayer;

extern Chicken gChickens[];

extern u8 gItemBeingHeld;

extern u32 dailyShippingBinValue;
extern u32 gTotalCropsShipped;
extern u32 gTotalEggsShipped;
extern u32 gTotalGoldenMilkShipped;

extern u16 D_801174A0[4][5];
extern u8 D_80117F20[];
extern u8 D_8018907D;
extern u8 D_801890CC;
extern u16 D_80118000[];

extern u8 D_80117180[];
extern u8 D_8011718C[];
extern u16 D_801181C0[];
extern u8 D_80118540[];
extern u8 D_80118620[];
extern u8 D_801C3E18;
extern u8 D_801C3F35;
extern u8 D_801FD624;

extern u32 D_801654F4;
extern u32 D_801806C0;
extern u32 D_80188F60;
extern u32 D_801C3F80;
extern u32 D_801FB5D0;
extern u32 D_801FB6FC;
extern u32 D_80237414;

extern UnknownStruct3 D_80189828;
extern RenderedSprite renderedSprites[];
extern UnknownStruct D_80204DF8[10];

u32 adjustValue(s32 current, s32 amount, s32 max);   
void setDailyEventBit(u16);  
void setDefaultBabyName(u8);

u8 checkWineBarrelInteraction(u8);

void func_8002FA2C(u16);
void func_80034DC8(u16, u8, u16);    
u8 func_8003C1A4(u16);
u32 func_8004D380(u8, u32);    
void func_8005AE8C(u8, u8, u16, u8, u8);
void func_80065F94(f32[], f32, u16);
void func_80073244(u8); 
void func_80085D48(u8, u32);   
void func_8009A398(void);                                  
u8 func_8009A400();
u8 func_8009A810();    
void func_8009A97C(); 
void func_8009A17C();                               
void func_8009A2D0();
void func_8009A53C();  
void func_8009AAC8();     
void func_8009B25C();         
void func_800ACC50(u16);
u8 func_800ACE50(u8);
u8 func_800ACFE8(u8);                 
void func_800D0360();                                  
void func_800D093C();                                  
void func_800D0DD4();                                  
void func_800D1DB0();                                  
void func_800D304C();                                  
void func_800D3694();                                  
void func_800D36BC();                                  
void func_800D36E4();                                  
void func_800D3714();                                                                   
void func_800D3958();                                  
void func_800D3B74();                                  
void func_800D3D90();                                  
void func_800D3FAC();                                  
void func_800D41C8();                                  
void func_800D43E4();                                  
void func_800D45F4();                                  
void func_800D4814();                                  
void func_800D4A34();                                  
void func_800D4C28();                                  
void func_800D4CA8();                                  
void func_800D4CB0();                                  
void func_800D4CB8();                                  
void func_800D4CC0();                                  
void func_800D4CD0();                                  
void func_800D4D1C();                                  
void func_800D4D68();                                  
void handleBlueFeatherUse();                  
void func_800D5130();
void func_800D56E8(u8, u16);   
u8 func_800D67E4(u8);
u8 func_800DA8B8(u8);                             
u8 func_800DA918(u8);                              
u16 func_800DA948(u8);
void func_800DAC70(u8, u8, u8, u8);
f32* func_800DAD74(u8, f32, u8);                           
u8 func_800DAF58(f32, u8);                            


INCLUDE_ASM(const s32, "itemHandlers", func_800CF850);

// jtbl_801222B0
INCLUDE_ASM(const s32, "itemHandlers", func_800CF874);

INCLUDE_ASM(const s32, "itemHandlers", func_800CF8F8);

INCLUDE_ASM(const s32, "itemHandlers", func_800CFB38);

INCLUDE_ASM(const s32, "itemHandlers", func_800CFD78);

INCLUDE_ASM(const s32, "itemHandlers", func_800CFDA0);

INCLUDE_ASM(const s32, "itemHandlers", func_800CFDC4);

INCLUDE_ASM(const s32, "itemHandlers", func_800CFF1C);

INCLUDE_ASM(const s32, "itemHandlers", func_800D0074);

// jtbl_801222C8
//INCLUDE_ASM(const s32, "itemHandlers", handleToolUse);

void handleToolUse(void) {
    
    switch (D_8018907D) {
        case 0:
            return;
        case SICKLE:
            func_800D0360();
            break;
        case HOE:
            func_800D093C();
            break;
        case AX:
            func_800D0DD4();
            break;
        case HAMMER:
            func_800D1DB0();
            break;
        case WATERING_CAN:
            func_800D304C();
            break;
        case MILKER:
            func_800D3694();
            break;
        case BELL:
            func_800D36BC();
            break;
        case BRUSH:
            func_800D36E4();
            break;
        case CLIPPERS:
            func_800D3714();
            break;
        case TURNIP_SEEDS:
            func_800D373C();
            break;
        case POTATO_SEEDS:
            func_800D3958();
            break;
        case CABBAGE_SEEDS:
            func_800D3B74();
            break;
        case TOMATO_SEEDS:
            func_800D3D90();
            break;
        case CORN_SEEDS:
            func_800D3FAC();
            break;
        case EGGPLANT_SEEDS:
            func_800D41C8();
            break;
        case STRAWBERRY_SEEDS:
            func_800D43E4();
            break;
        case MOON_DROP_SEEDS:
            func_800D45F4();
            break;
        case PINK_CAT_MINT_SEEDS:
            func_800D4814();
            break;
        case BLUE_MIST_SEEDS:
            func_800D4A34();
            break;
        case BIRD_FEED:
            func_800D4C28();
            break;
        case BALL:
            func_800D4CA8();
            break;
        case FEEDING_BOTTLE:
            func_800D4CB0();
            break;
        case 23:
            func_800D4CB8();
            break;
        case FISHING_POLE:
            func_800D4CC0();
            break;
        case MIRACLE_POTION:
            func_800D4CD0();
            break;
        case COW_MEDICINE:
            func_800D4D1C();
            break;
        case GRASS_SEEDS:
            func_800D4D68();
            break;
        case BLUE_FEATHER:
            handleBlueFeatherUse();
            break;
        case EMPTY_BOTTLE:
            func_800D5130();
            break;
        default:
            return;
    }
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D0318);

void func_800D0318(void) {

    int temp = D_80189828.unk_E; 

    // very bad programming: really just need to check if D_80189828.unk_E == 2
    if (temp) {
        if (temp != 3) {
            if (temp < 4) {
                if (temp == 2) {
                    handleToolUse();
                }       
            }
        }
    }
}

INCLUDE_ASM(const s32, "itemHandlers", func_800D0360);

INCLUDE_ASM(const s32, "itemHandlers", func_800D093C);

INCLUDE_ASM(const s32, "itemHandlers", func_800D0DD4);

INCLUDE_ASM(const s32, "itemHandlers", func_800D1DB0);

INCLUDE_ASM(const s32, "itemHandlers", func_800D304C);

//INCLUDE_ASM(const s32, "itemHandlers", func_800D3694);

// milker
void func_800D3694(void) {
    func_8009A53C();
    D_80189828.unk_E = 0;
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D36BC);

// cow bell
void func_800D36BC(void) {
    func_8009B25C();
    D_80189828.unk_E = 0;
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D36E4);

// brush
void func_800D36E4(void) {
    func_8009A17C();
    func_8009A2D0();
    D_80189828.unk_E = 0;
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D3714);

// clippers
void func_800D3714(void) {
    func_8009AAC8();
    D_80189828.unk_E = 0;
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D373C);

// turnip seeds
void func_800D373C(void) {

    int temp1;
    int temp3;
    u8 temp2;
    u8 temp5;
    u8 temp4;
    f32 float_array[3];

    temp1 = renderedSprites[0].direction + func_8003C1A4(0);
    temp3 = temp1;
    
    if (temp3 < 0) {
        temp3 = temp1 + 7;
    }
    
    temp5 = temp1 - (temp3 & 0xF8);
    
    if (temp5 < 5) {
        temp2 = D_80117180[D_80189828.unk_2];
    }
    else {
        temp2 = D_8011718C[D_80189828.unk_2];
    }
    
    func_80065F94(&float_array, 0.0, temp2);
 
    if ((func_800DA948(func_800DAF58(0.0f, temp2)) & 0x20) && (float_array[1] != 65535)) {
        if ((gSeason == 1) || (gBaseMapIndex == 0x56)) {
          temp4 = 8;
        } else {
          temp4 = 0xD7;
        }
        
        func_800DAC70(gBaseMapIndex, temp4, ((u8)float_array[0] - D_801FD624), ((u8)float_array[2]- D_801C3F35));
    }
    
    if (!D_80189828.unk_2) {
        func_800ACC50(0x1D);
    }
    
    D_80189828.unk_2 += 1;
    
    if (D_80189828.unk_2 == 9) {
        D_80189828.unk_E = 0;
    }
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D3958);

void func_800D3958(void) {

    int temp1;
    int temp3;
    u8 temp2;
    u8 temp5;
    u8 temp4;
    f32 floatArray[3];

    temp1 = renderedSprites[0].direction + func_8003C1A4(0);
    temp3 = temp1;
    
    if (temp3 < 0) {
        temp3 = temp1 + 7;
    }
    
    temp5 = temp1 - (temp3 & 0xF8);
    
    if (temp5 < 5) {
        temp2 = D_80117180[D_80189828.unk_2];
    }
    else {
        temp2 = D_8011718C[D_80189828.unk_2];
    }
    
    func_80065F94(floatArray, 0.0, temp2);
 
    if ((func_800DA948(func_800DAF58(0.0f, temp2)) & 0x20) && (floatArray[1] != 65535)) {
        if ((gSeason == SPRING) || (gBaseMapIndex == GREENHOUSE)) {
          temp4 = 0x13;
        } else {
          temp4 = 0xD7;
        }
        
        func_800DAC70(gBaseMapIndex, temp4, ((u8)floatArray[0] - D_801FD624), ((u8)floatArray[2]- D_801C3F35));
    }
    
    if (!D_80189828.unk_2) {
        func_800ACC50(0x1D);
    }
    
    D_80189828.unk_2 += 1;
    
    if (D_80189828.unk_2 == 9) {
        D_80189828.unk_E = 0;
    }
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D3B74);

// cabbage seeds
void func_800D3B74(void) {
 
    int temp1;
    int temp3;
    u8 temp2;
    u8 temp5;
    u8 temp4;
    f32 floatArray[3];

    temp1 = renderedSprites[0].direction + func_8003C1A4(0);
    temp3 = temp1;
    
    if (temp3 < 0) {
        temp3 = temp1 + 7;
    }
    
    temp5 = temp1 - (temp3 & 0xF8);
    
    if (temp5 < 5) {
        temp2 = D_80117180[D_80189828.unk_2];
    }
    else {
        temp2 = D_8011718C[D_80189828.unk_2];
    }
    
    func_80065F94(floatArray, 0.0, temp2);
 
    if ((func_800DA948(func_800DAF58(0.0f, temp2)) & 0x20) && (floatArray[1] != 65535)) {
        if ((gSeason == SPRING) || (gBaseMapIndex == GREENHOUSE)) {
          temp4 = 0x33;
        } else {
          temp4 = 0xD7;
        }
        
        func_800DAC70(gBaseMapIndex, temp4, ((u8)floatArray[0] - D_801FD624), ((u8)floatArray[2]- D_801C3F35));
    }
    
    if (!D_80189828.unk_2) {
        func_800ACC50(0x1D);
    }
    
    D_80189828.unk_2 += 1;
    
    if (D_80189828.unk_2 == 9) {
        D_80189828.unk_E = 0;
    }
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D3D90);

// tomato seeds
void func_800D3D90(void) {
    int temp1;
    int temp3;
    u8 temp2;
    u8 temp5;
    u8 temp4;
    f32 floatArray[3];

    temp1 = renderedSprites[0].direction + func_8003C1A4(0);
    temp3 = temp1;
    
    if (temp3 < 0) {
        temp3 = temp1 + 7;
    }
    
    temp5 = temp1 - (temp3 & 0xF8);
    
    if (temp5 < 5) {
        temp2 = D_80117180[D_80189828.unk_2];
    }
    else {
        temp2 = D_8011718C[D_80189828.unk_2];
    }
    
    func_80065F94(floatArray, 0.0, temp2);
 
    if ((func_800DA948(func_800DAF58(0.0f, temp2)) & 0x20) && (floatArray[1] != 65535)) {
        if ((gSeason == SUMMER) || (gBaseMapIndex == GREENHOUSE)) {
          temp4 = 0x53;
        } else {
          temp4 = 0xD7;
        }
        
        func_800DAC70(gBaseMapIndex, temp4, ((u8)floatArray[0] - D_801FD624), ((u8)floatArray[2]- D_801C3F35));
    }
    
    if (!D_80189828.unk_2) {
        func_800ACC50(0x1D);
    }
    
    D_80189828.unk_2 += 1;
    
    if (D_80189828.unk_2 == 9) {
        D_80189828.unk_E = 0;
    }
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D3FAC);

// corn seeds
void func_800D3FAC(void) {

    int temp1;
    int temp3;
    u8 temp2;
    u8 temp5;
    u8 temp4;
    f32 floatArray[3];

    temp1 = renderedSprites[0].direction + func_8003C1A4(0);
    temp3 = temp1;
    
    if (temp3 < 0) {
        temp3 = temp1 + 7;
    }
    
    temp5 = temp1 - (temp3 & 0xF8);
    
    if (temp5 < 5) {
        temp2 = D_80117180[D_80189828.unk_2];
    }
    else {
        temp2 = D_8011718C[D_80189828.unk_2];
    }
    
    func_80065F94(floatArray, 0.0, temp2);
 
    if ((func_800DA948(func_800DAF58(0.0f, temp2)) & 0x20) && (floatArray[1] != 65535)) {
        if ((gSeason == SUMMER) || (gBaseMapIndex == GREENHOUSE)) {
          temp4 = 0x68;
        } else {
          temp4 = 0xD7;
        }
        
        func_800DAC70(gBaseMapIndex, temp4, ((u8)floatArray[0] - D_801FD624), ((u8)floatArray[2]- D_801C3F35));
    }
    
    if (!D_80189828.unk_2) {
        func_800ACC50(0x1D);
    }
    
    D_80189828.unk_2 += 1;
    
    if (D_80189828.unk_2 == 9) {
        D_80189828.unk_E = 0;
    }
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D41C8);

// eggplant seeds
void func_800D41C8(void) {

    int temp1;
    int temp3;
    u8 temp2;
    u8 temp5;
    u8 temp4;
    f32 floatArray[3];

    temp1 = renderedSprites[0].direction + func_8003C1A4(0);
    temp3 = temp1;
    
    if (temp3 < 0) {
        temp3 = temp1 + 7;
    }
    
    temp5 = temp1 - (temp3 & 0xF8);
    
    if (temp5 < 5) {
        temp2 = D_80117180[D_80189828.unk_2];
    }
    else {
        temp2 = D_8011718C[D_80189828.unk_2];
    }
    
    func_80065F94(floatArray, 0.0, temp2);
 
    if ((func_800DA948(func_800DAF58(0.0f, temp2)) & 0x20) && (floatArray[1] != 65535)) {
        if ((gSeason == 3) || (gBaseMapIndex == 0x56)) {
          temp4 = 0x22;
        } else {
          temp4 = 0xD7;
        }
        
        func_800DAC70(gBaseMapIndex, temp4, ((u8)floatArray[0] - D_801FD624), ((u8)floatArray[2]- D_801C3F35));
    }
    
    if (!D_80189828.unk_2) {
        func_800ACC50(0x1D);
    }
    
    D_80189828.unk_2 += 1;
    
    if (D_80189828.unk_2 == 9) {
        D_80189828.unk_E = 0;
    }
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D43E4);

// strawberry seeds
void func_800D43E4(void) {
    
    int temp1;
    int temp3;

    u8 temp2;
    u8 temp5;
    u8 temp4;

    f32 floatArray[3];

    temp1 = renderedSprites[0].direction + func_8003C1A4(0);
    temp3 = temp1;
    
    if (temp3 < 0) {
        temp3 = temp1 + 7;
    }
    
    temp5 = temp1 - (temp3 & 0xF8);
    
    if (temp5 < 5) {
        temp2 = D_80117180[D_80189828.unk_2];
    }
    else {
        temp2 = D_8011718C[D_80189828.unk_2];
    }

    func_80065F94(floatArray, 0.0, temp2);

    if ((func_800DA948(func_800DAF58(0.0f, temp2)) & 0x20) && (floatArray[1] != 65535)) {
        temp4 =  ((-(gBaseMapIndex != 0x56) & ~0x28) | 0x44);
        func_800DAC70(gBaseMapIndex, temp4, ((u8)floatArray[0] - D_801FD624), ((u8)floatArray[2]- D_801C3F35));
    }

    if (!D_80189828.unk_2) {
        func_800ACC50(0x1D);
    }

    D_80189828.unk_2 += 1;
    
    if (D_80189828.unk_2 == 9) {
        D_80189828.unk_E = 0;
    }
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D45F4);

// moon drop seeds
void func_800D45F4(void) {

    int temp1;
    int temp3;

    u8 temp2;
    u8 temp5;
    u8 temp4;

    f32 floatArray[3];

    temp1 = renderedSprites[0].direction + func_8003C1A4(0);
    temp3 = temp1;
    
    if (temp3 < 0) {
        temp3 = temp1 + 7;
    }
    
    temp5 = temp1 - (temp3 & 0xF8);
    
    if (temp5 < 5) {
        temp2 = D_80117180[D_80189828.unk_2];
    }
    else {
        temp2 = D_8011718C[D_80189828.unk_2];
    }
    
    func_80065F94(floatArray, 0.0, temp2);
 
    if ((func_800DA948(func_800DAF58(0.0f, temp2)) & 0x20) && (floatArray[1] != 65535)) {
        
        if ((gSeason - 1 < 2U) || (gBaseMapIndex == 0x56)) {
            temp4 = 0x90; 
        } else {
            temp4 = 0xD7;
        }

        func_800DAC70(gBaseMapIndex, temp4, ((u8)floatArray[0] - D_801FD624), ((u8)floatArray[2]- D_801C3F35));
    }
    
    if (!D_80189828.unk_2) {
        func_800ACC50(0x1D);
    }
    
    D_80189828.unk_2 += 1;
    
    if (D_80189828.unk_2 == 9) {
        D_80189828.unk_E = 0;
    }
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D4814);

void func_800D4814(void) {

    int temp1;
    int temp3;

    u8 temp2;
    u8 temp5;
    u8 temp4;

    f32 floatArray[3];

    temp1 = renderedSprites[0].direction + func_8003C1A4(0);
    temp3 = temp1;
    
    if (temp3 < 0) {
        temp3 = temp1 + 7;
    }
    
    temp5 = temp1 - (temp3 & 0xF8);
    
    if (temp5 < 5) {
        temp2 = D_80117180[D_80189828.unk_2];
    }
    else {
        temp2 = D_8011718C[D_80189828.unk_2];
    }
    
    func_80065F94(floatArray, 0.0, temp2);
 
    if ((func_800DA948(func_800DAF58(0.0f, temp2)) & 0x20) && (floatArray[1] != 65535)) {
        
        if ((gSeason - 1 < 2U) || (gBaseMapIndex == 0x56)) {
            temp4 = 0x9F; 
        } else {
            temp4 = 0xD7;
        }

        func_800DAC70(gBaseMapIndex, temp4, ((u8)floatArray[0] - D_801FD624), ((u8)floatArray[2]- D_801C3F35));
    }
    
    if (!D_80189828.unk_2) {
        func_800ACC50(0x1D);
    }
    
    D_80189828.unk_2 += 1;
    
    if (D_80189828.unk_2 == 9) {
        D_80189828.unk_E = 0;
    }
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D4A34);

void func_800D4A34(void) {

    int temp1;
    int temp3;

    u8 temp2;
    u8 temp5;
    u8 temp4;

    f32 floatArray[3];

    temp1 = renderedSprites[0].direction + func_8003C1A4(0);
    temp3 = temp1;
    
    if (temp3 < 0) {
        temp3 = temp1 + 7;
    }
    
    temp5 = temp1 - (temp3 & 0xF8);
    
    if (temp5 < 5) {
        temp2 = D_80117180[D_80189828.unk_2];
    }
    else {
        temp2 = D_8011718C[D_80189828.unk_2];
    }
    
    func_80065F94(floatArray, 0.0, temp2);
 
    if ((func_800DA948(func_800DAF58(0.0f, temp2)) & 0x20) && (floatArray[1] != 65535)) {
        func_800DAC70(gBaseMapIndex, 0xB2, ((u8)floatArray[0] - D_801FD624), ((u8)floatArray[2]- D_801C3F35));
    }
    
    if (!D_80189828.unk_2) {
        func_800ACC50(0x1D);
    }
    
    D_80189828.unk_2 += 1;
    
    if (D_80189828.unk_2 == 9) {
        D_80189828.unk_E = 0;
    }
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D4C28);

// chicken feed
void func_800D4C28(void) {
    
    u8 chicken;
    
    chicken = func_800ACE50(gBaseMapIndex);
  
    if (chicken != 0xFF) {
        func_80073244(chicken - 0x13);
        gChickens[chicken - 0x13].upkeep |= 0x10;
    }
    
    D_80189828.unk_E = 0;  
}

// ball
void func_800D4CA8(void) {}

// feeding bottle
void func_800D4CB0(void) {}

// unknown tool
void func_800D4CB8(void) {}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D4CC0);

// fishing pole
void func_800D4CC0(void) {
    D_80189828.unk_E = 0;
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D4CD0);

// miracle potion
void func_800D4CD0(void) {

    if (func_8009A810()) {
        setDailyEventBit(0x14);
    } else {
        toggleDailyEventBit(0x14);
    }

    D_80189828.unk_E = 0;
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D4D1C);

// cow medicine
void func_800D4D1C(void) {

    if (func_8009A400()) {
        setDailyEventBit(0x14);
    } else {
        toggleDailyEventBit(0x14);
    }

    D_80189828.unk_E = 0;
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D4D68);

// grass seeds
void func_800D4D68(void) {

    int temp1;
    int temp3;
    u8 temp2;
    u8 temp5;
    u8 temp4;
    f32 floatArray[3];

    temp1 = renderedSprites[0].direction + func_8003C1A4(0);
    temp3 = temp1;
    
    if (temp3 < 0) {
        temp3 = temp1 + 7;
    }
    
    temp5 = temp1 - (temp3 & 0xF8);
    
    if (temp5 < 5) {
        temp2 = D_80117180[D_80189828.unk_2];
    }
    else {
        temp2 = D_8011718C[D_80189828.unk_2];
    }
    
    func_80065F94(floatArray, 0.0, temp2);
 
    if ((func_800DA948(func_800DAF58(0.0f, temp2)) & 0x20) && (floatArray[1] != 65535)) {
        if (gBaseMapIndex == FARM) {
            func_800DAC70(FARM, 0x85, ((u8)floatArray[0] - D_801FD624), ((u8)floatArray[2]- D_801C3F35));    
        }
    }
    
    if (!D_80189828.unk_2) {
        func_800ACC50(0x1D);
    }
    
    D_80189828.unk_2 += 1;
    
    if (D_80189828.unk_2 == 9) {
        D_80189828.unk_E = 0;
    }
}

INCLUDE_RODATA(const s32, "itemHandlers", D_80122340);

INCLUDE_RODATA(const s32, "itemHandlers", D_80122344);

INCLUDE_RODATA(const s32, "itemHandlers", D_80122348);

INCLUDE_RODATA(const s32, "itemHandlers", D_8012234C);

INCLUDE_RODATA(const s32, "itemHandlers", D_80122350);

INCLUDE_RODATA(const s32, "itemHandlers", D_8012235C);

INCLUDE_RODATA(const s32, "itemHandlers", D_80122368);

//INCLUDE_RODATA(const s32, "itemHandlers", D_80122374);

const u8 D_80122374[12] = { 5, 4, 3, 6, 8, 2, 7, 0, 1, 0, 0, 0 };

//INCLUDE_ASM(const s32, "itemHandlers", handleBlueFeatherUse);

// jumtable: 80122380, 0xFD780
void handleBlueFeatherUse(void) {
    
    toggleDailyEventBit(0x14);
    
    if (D_801C3E18 == 0xFF) goto func_end;

    func_80085D48(D_801C3E18, 0);
    
    npcInfoArray[D_801C3E18].movingFlag = npcInfoArray[D_801C3E18].unk_1E;
    
    setSpecialDialogueBit(0x135);
    
    switch (D_801C3E18) {
        case 0:
            if (npcAffection[MARIA] >= 220) {
                gWife = MARIA;   
                setDailyEventBit(0x14);
                setLifeEventBit(ENGAGED);
                setDefaultBabyName(MARIA);
            }
            break;
        case 1:
            if (npcAffection[POPURI] >= 220) {
                gWife = POPURI;
                setDailyEventBit(0x14);
                setLifeEventBit(ENGAGED);
                setDefaultBabyName(POPURI);
            }
            break;
        case 2:
            if (npcAffection[ELLI] >= 220) {
                gWife = ELLI;
                setDailyEventBit(0x14);
                setLifeEventBit(ENGAGED);
                setDefaultBabyName(ELLI);
            }
            break;
        case 3:
            if (npcAffection[ANN] >= 220) {
                gWife = ANN;
                setDailyEventBit(0x14);
                setLifeEventBit(ENGAGED);
                setDefaultBabyName(ANN);
            }
            break;
        case 4:
            if (npcAffection[KAREN] >= 220) {
                gWife = KAREN;
                setDailyEventBit(0x14);
                setLifeEventBit(ENGAGED);
                setDefaultBabyName(KAREN);
            }
            break;
        default:
            break;
        }

    if (checkDailyEventBit(0x14)) {
        // use up blue feather if successful proposal
        gPlayer.unk_1D = 0;
    }
    
func_end:
        D_80189828.unk_E = 0;
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D5130);

// empty bottle
void func_800D5130(void) {

    if (func_800ACFE8(gBaseMapIndex)) {
        gPlayer.bottleContents = 1;
    } else if (checkWineBarrelInteraction(gBaseMapIndex) ) {
        gPlayer.bottleContents = 2;
    } else {
        func_8009A97C();
    }

    D_80189828.unk_E = 0;
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D51B0);

void func_800D51B0(void) {
    u8 i;

    for (i = 0; i < 0xA; i++) {
        D_80204DF8[i].unk_0.x = 0;
        D_80204DF8[i].unk_0.y = 0;
        D_80204DF8[i].unk_0.z = 0;
        D_80204DF8[i].unk_C = 0;
        D_80204DF8[i].unk_10 = 0;
        D_80204DF8[i].unk_14 = 0;
        D_80204DF8[i].unk_18 = 0;
        D_80204DF8[i].unk_1C = 0;
        D_80204DF8[i].unk_20 = 0;
        D_80204DF8[i].unk_24 = 0;
        D_80204DF8[i].unk_26 = 0;
        D_80204DF8[i].unk_28 = 0;
        D_80204DF8[i].unk_2A = 0;
        D_80204DF8[i].flags = 0;
    }
}


//INCLUDE_ASM(const s32, "itemHandlers", func_800D5290);

void func_800D5290(void) {

    u8 i;
    
    for (i = 0; i < 10; i++) {
        if (D_80204DF8[i].flags & 1) {
            func_800D56E8(i, D_80204DF8[i].unk_28);
        }    
    }
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D5308);

// tool acquisition handler
u8 func_800D5308(u8 arg0, u8 arg1, u32 arg2, s32 arg3, s32 arg4) {

    D_80204DF8[arg0].unk_2A = arg1;
    D_80204DF8[arg0].unk_28 = arg2;
    D_80204DF8[arg0].unk_26 = 0;
    D_80204DF8[arg0].flags = arg3 | (arg4 | 1);;
    
    gItemBeingHeld = func_800D67E4(arg2);
    
    return arg0;
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D5390);

u8 func_800D5390(u8 arg0, u8 arg1, u32 arg2, u16 arg3, u8 arg4) {
    
    u8 found = 0;
    int tempBit = 1;
    
    while ((arg0 < 10) && (!found)) {
        if (!(D_80204DF8[arg0].flags & 1)) {
          found = 1;
        }
        else {
          arg0++;
        }
    }
    
    if (found) {
        D_80204DF8[arg0].unk_2A = arg1;
        D_80204DF8[arg0].unk_28 = arg2;
        D_80204DF8[arg0].flags = arg3 | (arg4 | tempBit);
        D_80204DF8[arg0].unk_26 = 0;
        gItemBeingHeld = func_800D67E4(arg2);
    }
      
    else {
        arg0 = 0xff;
    }
    
    return arg0;
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D5488);

u8 func_800D5488(u8 arg0, u8 arg1, u32 arg2, u16 arg3, u8 arg4) {
    
    u8 found = 0;
    
    while ((arg0 < 10) && (!found)) {
        if (!(D_80204DF8[arg0].flags & 1)) {
          found = 1;
        }
        else {
          arg0++;
        }
    }

    if (found) {
        D_80204DF8[arg0].unk_2A = arg1;
        D_80204DF8[arg0].unk_28 = arg2;
        D_80204DF8[arg0].flags = arg3 | (arg4 | 1);
        D_80204DF8[arg0].unk_26 = 0;
    }
    else {
        arg0 = 0xff;
    }
    
    return arg0;
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D5548);

void func_800D5548(u8 arg0) {

    if (D_80204DF8[arg0].flags & 1) {        
        func_8002FA2C(arg0 + 0x27);
        D_80204DF8[arg0].unk_2A = 0;
        D_80204DF8[arg0].flags = 0;
    }
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D55B8);

u16 func_800D55B8(u8 arg0) {
    return D_80204DF8[arg0].unk_28;
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D55E4);

void func_800D55E4(u8 arg0, u8 arg1) {

    if (D_80204DF8[arg0].flags & 1) {
        D_80204DF8[arg0].unk_2A = arg1;
    }
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D5628);

void func_800D5628(u8 arg0, f32 arg1, f32 arg2, f32 arg3) {

    D_80204DF8[arg0].unk_0.x = arg1;
    D_80204DF8[arg0].unk_0.y = arg2;
    D_80204DF8[arg0].unk_0.z = arg3;
    D_80204DF8[arg0].flags |= 2;

}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D5688);

void func_800D5688(u8 arg0, u32 arg1, u32 arg2, u32 arg3) {

    D_80204DF8[arg0].unk_18 = arg1;
    D_80204DF8[arg0].unk_1C = arg2;
    D_80204DF8[arg0].unk_20 = arg3;
    D_80204DF8[arg0].flags |= 8;
    
}

INCLUDE_ASM(const s32, "itemHandlers", func_800D56E8);

u16 func_800D5A6C(u16 index) {
    return itemFlags[index];
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D5A88);

u16 func_800D5A88(u16 arg0) {

  u16 result;
  
  result = D_801174A0[arg0][gSeason];

  if ((u16)(arg0 - 0xBA) < 8) {
    result = 0xF9;

  }
  if ((u16)(arg0 - 0xC2) < 8) {
    result = 0xFA;
  }

  if ((u16)(arg0 - 0xAB) < 8) {
    result = 0xFB;
  }

  return result;

}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D5B00);

u8 func_800D5B00(u16 arg0) {
    return D_80118540[arg0];
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D5B18);

u8 func_800D5B18(u16 arg0) {
    return D_80118620[arg0];
}

INCLUDE_ASM(const s32, "itemHandlers", func_800D5B30);

INCLUDE_ASM(const s32, "itemHandlers", func_800D5CC0);

//INCLUDE_ASM(const s32, "itemHandlers", func_800D67E4);

u8 func_800D67E4(u8 arg0) {
    return D_80117F20[arg0];
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D67FC);

void func_800D67FC(u8 arg0) {
    
    s32 temp;

    // member of dialogue box struct
    u16 var_a2;

    // why is this needed?
    temp = arg0;
    
    if (temp < 0x70) {
        if (temp > 0x5F) {
            // from player struct
            var_a2 = D_801890CC + 0xF5;
            goto func_end;
        } 
    } 

    // array of npc/dialogue-related u16s
    var_a2 = D_80118000[arg0];
    
func_end:
    func_8005AE8C(1, 6, var_a2, 0, 2);
}

// jumptable: 801226D8, 0xFDAD8
//INCLUDE_ASM(const s32, "itemHandlers", func_800D6868);

// shipping bin
void func_800D6868(u8 arg0) {

    u32 result;
    
    dailyShippingBinValue += adjustValue(dailyShippingBinValue, D_801181C0[arg0], 999999);
    
    switch (arg0) {
        case 13:
            D_801654F4 += adjustValue(D_801654F4, 1, 999);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped,1, 99999);
            break;
        case 14:
            D_80237414 += adjustValue(D_80237414, 1, 999);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped,1, 99999);
            break;
        case 18:
            D_801FB6FC += adjustValue(D_801FB6FC, 1, 999);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped,1, 99999);
            break;
        case 15:
            D_801C3F80 += adjustValue(D_801C3F80, 1U, 0x3E7);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped,1, 99999);
            break;
        case 19:
            D_801806C0 += adjustValue(D_801806C0, 1U, 0x3E7);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped,1, 99999);
            break;
        case 16:
            D_80188F60 += adjustValue(D_80188F60, 1U, 0x3E7);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped,1, 99999);
            break;
        case 17:
            D_801FB5D0 += adjustValue(D_801FB5D0, 1U, 0x3E7);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped,1, 99999);
            break;
        case 21:
        case 22:
        case 23:
        case 24:
            gTotalGoldenMilkShipped += adjustValue(gTotalGoldenMilkShipped, 1U, 0x270F);
            break;
        case 20:
            gTotalEggsShipped += adjustValue(gTotalEggsShipped, 1, 0x270F);
            break;
            
    }

    func_8009A398();
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D6B28);

void func_800D6B28(void) {
    func_80034DC8(0, 0, 0x13);
    setDailyEventBit(0x43);
}

#ifdef PERMUTER
// pretty much 99%, but does array shifting on Vec3f before loading floats 
void func_800D6B58(u8 arg0, u8 arg1) {

    u8 temp;
    u8 temp2;
 
    D_80204DF8[arg1].unk_0.x = 0;
    D_80204DF8[arg1].unk_0.y = 0;
    D_80204DF8[arg1].unk_0.z = 0;
    D_80204DF8[arg1].flags |= 8;

    func_8002FE10(arg1 + 0x27, 0.0f, 0.0f, 0.0f, 0.0f);
    func_8002ECD4(arg1 + 0x27, 0xFFFF, 0xFF);
    
    func_8003019C(arg1 + 0x27, 0);
    func_80030054(arg1 + 0x27, 0);
    func_80030240(arg1 + 0x27, 0);
    func_800302E4(arg1 + 0x27, 0);

    if (gBaseMapIndex == 0x52) {
        temp = func_800309B4(0, 0.0f, 32.0f);
        temp2 = temp - 0x1B;
        if (temp2 < 2 || temp == 0x1D) {
            D_80204DF8[arg1].unk_0.x = 288.0f;
            D_80204DF8[arg1].unk_0.y = 80.0f;
            D_80204DF8[arg1].unk_0.z = -404.0f;
            D_80204DF8[arg1].flags |= 2;
            func_8002F2FC(arg1 + 0x27, 0xE9);
        } 
    }
    
    if (gBaseMapIndex == 0x11) {
        temp = func_800309B4(0, 0.0f, 40.0f);
        if (temp == 0x10 || temp == 0x14) {
            D_80204DF8[arg1].unk_0.x = 160.0f;
            D_80204DF8[arg1].unk_0.y = 64.0f;
            D_80204DF8[arg1].unk_0.z = -128.0f;
            D_80204DF8[arg1].flags |= 2;
            func_8002F2FC(arg1 + 0x27, 0xE9);
        }
        if (!(D_8016FE00 & 1)) {
            if ((!checkLifeEventBit(0x5E)) && (!checkDailyEventBit(0x21)) && (D_80204DF8[arg1].unk_28 - 0x25 < 2U) && (gHour - 9) < 8U) {
                setDailyEventBit(0x47);
            }
            if ((!checkLifeEventBit(0x5F)) && (!checkDailyEventBit(0x21)) && (D_80204DF8[arg1].unk_28 ==  0x27) && (gHour - 9) < 8U) {
                setDailyEventBit(0x48);
            }
        }
    }

    if (gBaseMapIndex == 0x23) {
        temp = func_800309B4(0, 0.0f, 32.0f);
        if (temp == 0x10) {
            D_80204DF8[arg1].unk_0.x = -16.0f;
            D_80204DF8[arg1].unk_0.y = 80.0f;
            D_80204DF8[arg1].unk_0.z = -224.0f;
            D_80204DF8[arg1].flags |= 2;
            func_8002F2FC(arg1 + 0x27, 0xE9);
        }
        
        if (!(D_8016FE00 & 1)) {
            if (!checkDailyEventBit(0x46) && itemFlags[D_80204DF8[arg1].unk_28] & 0x8000 && (gHour - 9) < 8U ) {
                setDailyEventBit(0x45);    
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "itemHandlers", func_800D6B58);
#endif

// jtbl_80122708
INCLUDE_ASM(const s32, "itemHandlers", func_800D7010);
