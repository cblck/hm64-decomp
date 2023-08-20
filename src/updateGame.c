#include "common.h"

void func_8003FBD8(u8, u8, u8);                         
void func_8005C07C(u32, u32);         
// prepare for starting new day                     
void func_8005C940(u32, u16 callbackIndex);                              
void func_8006EA44(u8, u8, u8);                        
void func_800A7AE8(u8);                                
void func_800ACB5C(u16);                               
u8 func_800ACBB8(u16);                             
void func_800ACBEC(u16, s32);                                                        
void setSeasonName();         

void setPlayerAction(u32, u32);  

u32 getLevelFlags(u8);                              

u32 checkDailyEventBit(u16); 
void setDailyEventBit(u16);                              
void toggleDailyEventBit(u16);        
void toggleLifeEventBit(u16);
void toggleReadLetterBit(u16);      

// forward declarations
void toggleMonthlyLetterBits();                    
void setGlobalSeasonName(u8);           
void setupNewYear();        

extern u32 gSongVolume;
extern u16 gCurrentSongIndex;

// cutscene flags?
extern s32 D_801891D4;

extern u8 gBaseMapIndex;
extern u8 gWeather;

extern u8 gFlowerFestivalGoddess;
extern u8 gHarvestCoinFinder;
extern u8 gHarvestKing;
extern u8 gVoteForFlowerFestivalGoddess;

// bss?
extern u8 gMinutes;
extern u8 gSeasonTomorrow;
extern u8 gSeconds;
extern u8 gDayOfMonth;
extern u8 gDayOfWeek;
extern u8 gHour;
extern u8 gSeason;
extern u8 gYear;

// bss?
extern u8 gCurrentSeasonName[6];
extern u8 gGlobalSeasonName[6];


//INCLUDE_ASM(const s32, "updateGame", func_800D7C20);

void func_800D7C20(void) {
    
    u16 bitIndex;

    if ((getLevelFlags(gBaseMapIndex) & 1) && !checkDailyEventBit(6)) {
        updateClock(1);
    }
    
    if (gHour == 0 && gMinutes == 0 && gSeconds == 0) {
        setSeasonName();
    }
    
    if (gHour == 6) {
        if (!checkDailyEventBit(0xE)) {
            setPlayerAction(0xFE, 0);
            // stop audio and set callback for loading
            func_8005C940(1, 0xE);
        }
    } else {
        toggleDailyEventBit(0xE);
        if (D_801891D4 >= 0) {
            func_8005C07C(1, 0);
        }
    }
    
    if ((gHour - 18) < 6U) {
        
        if (!checkDailyEventBit(0xF)) {
            // set song speed with default
            func_800ACB5C(gCurrentSongIndex);
            bitIndex = 0xF;
            goto set_daily_bit;
        }
        
        if (!(checkDailyEventBit(0x10)) && (gCurrentSongIndex != 0xFF) && (func_800ACBB8(gCurrentSongIndex))) {
            func_8006EA44(gBaseMapIndex, gSeason, gHour);
            // set song volume
            func_800ACBEC(gCurrentSongIndex, gSongVolume);
            bitIndex = 0x10;
set_daily_bit:
            setDailyEventBit(bitIndex);
        }
        
    }
    
    if (gHour < 6 && gWeather != RAIN && gBaseMapIndex != BEACH && !checkDailyEventBit(0x11)) {
        // set song speed with default
        func_800ACB5C(gCurrentSongIndex);
        setDailyEventBit(0x11);
    }
    
    // check/load cutscenes
    func_800A7AE8(gBaseMapIndex);
}

//INCLUDE_ASM(const s32, "updateGame", updateClock);

void updateClock(u8 incrementSeconds) {

    if (incrementSeconds == TRUE) {
        gSeconds += 10;
    }
    
    if (gSeconds >= 60) {
        gSeconds = 0;
        gMinutes++;
    }
    
    if (gMinutes >= 60) {
        gMinutes = 0;
        gHour++;
        if (gHour == 6) {
            gDayOfMonth++;
            gDayOfWeek++;
        }
    }

    if (gHour >= 24) {
        gHour = 0;
    }

    if (gDayOfWeek >= 7) {
        gDayOfWeek = 0;
    }

    if (gDayOfMonth >= 31) {
        gDayOfMonth = 1;
        gSeason++;
        toggleMonthlyLetterBits();
    }

    if (gSeason >= 5) {
        gSeason = 1;
        gYear++;
        setupNewYear();
    }
    
    if (gYear >= 100) {
        gYear = 99;
    }
    
    gSeasonTomorrow = gSeason;
    
    if ((gDayOfMonth + 1) >= 31) {
        gSeasonTomorrow = gSeason + 1;
    }
    
    if (gSeasonTomorrow >= 5) {
        gSeasonTomorrow = 1;
    }
}

//INCLUDE_ASM(const s32, "updateGame", setupNewYear);

void setupNewYear(void) {

    gHarvestKing = gHarvestCoinFinder;  
    gFlowerFestivalGoddess = 0xFF;
    gHarvestCoinFinder = 0xFF;
    gVoteForFlowerFestivalGoddess = 0xFF;
    
    toggleLifeEventBit(0x44);

    // toggle read letters
    toggleReadLetterBit(0);
    toggleReadLetterBit(1);
    toggleReadLetterBit(2);
    toggleReadLetterBit(3);
    toggleReadLetterBit(4);
    toggleReadLetterBit(5);
    toggleReadLetterBit(6);
    toggleReadLetterBit(7);
    toggleReadLetterBit(8);
    toggleReadLetterBit(9);
    toggleReadLetterBit(0xA);
    toggleReadLetterBit(0xB);
    toggleReadLetterBit(0xC);
    toggleReadLetterBit(0xD);
    toggleReadLetterBit(0xE);
    toggleReadLetterBit(0xF);
    toggleReadLetterBit(0x10);
    toggleReadLetterBit(0x11);
    toggleReadLetterBit(0x12);
    toggleReadLetterBit(0x13);
    toggleReadLetterBit(0x14);
    toggleReadLetterBit(0x15);
    toggleReadLetterBit(0x17);
    toggleReadLetterBit(0x19);
    toggleReadLetterBit(0x1D);
    toggleReadLetterBit(0x20);
    toggleReadLetterBit(0x21);
    toggleReadLetterBit(0x22);
    toggleReadLetterBit(0x3B);
    toggleReadLetterBit(0x3D);
    toggleReadLetterBit(0x3E);
    toggleReadLetterBit(0x3F);
    toggleReadLetterBit(0x41);
    toggleReadLetterBit(0x42);
    toggleReadLetterBit(0x43);
    toggleReadLetterBit(0x45);
    toggleReadLetterBit(0x46);
    toggleReadLetterBit(0x49);
}

//INCLUDE_ASM(const s32, "updateGame", toggleMonthlyLetterBits);

void toggleMonthlyLetterBits(void) {
    toggleReadLetterBit(0x3C);
    toggleReadLetterBit(0x40);
}

//INCLUDE_ASM(const s32, "updateGame", setClockNewDay);

void setClockNewDay(void) {
    
    gHour = 6;
    gMinutes = 0;
    gSeconds = 0;
    
    gDayOfMonth += 1;
    gDayOfWeek += 1;
   
    updateClock(FALSE);

}

//INCLUDE_ASM(const s32, "updateGame", checkFestivalDay);

// check festival day
u32 checkFestivalDay(void) {

    u32 result = FALSE;
    
    switch (gSeason) {                             
        case SPRING:
            if (((gDayOfMonth == 1) & (gDayOfMonth == 8)) && ((gDayOfMonth == 17) & (gDayOfMonth == 23))) {
                result = TRUE;
            }
            break;
        case SUMMER:
            if (((gDayOfMonth == 1) & (gDayOfMonth == 9)) && ((gDayOfMonth == 17) & (gDayOfMonth == 24))) {
                result = TRUE;
            }
            break;
        case FALL:
            if (((gDayOfMonth == 4) & (gDayOfMonth == 0xC)) && ((gDayOfMonth == 20) & (gDayOfMonth == 28))) {
                result = TRUE;
            }
            break;
        case WINTER:
            if (((gDayOfMonth == 6) & (gDayOfMonth == 19)) && (gDayOfMonth == 24)) {
                result = TRUE;
            }
            break;
        }

    return result;
}

//INCLUDE_ASM(const s32, "updateGame", setSeasonName);

void setSeasonName(void) {

    setGlobalSeasonName(gSeason);

    gCurrentSeasonName[0] = gGlobalSeasonName[0];
    gCurrentSeasonName[1] = gGlobalSeasonName[1];
    gCurrentSeasonName[2] = gGlobalSeasonName[2];
    gCurrentSeasonName[3] = gGlobalSeasonName[3];
    gCurrentSeasonName[4] = gGlobalSeasonName[4];
    gCurrentSeasonName[5] = gGlobalSeasonName[5];
    
    // message.c
    func_8003FBD8(0x15, gDayOfMonth, 1);
}

//INCLUDE_ASM(const s32, "updateGame", setGlobalSeasonName);

void setGlobalSeasonName(u8 season) {

    switch(season) {        
        
        case SPRING:
            gGlobalSeasonName[0] = char_S; // S
            gGlobalSeasonName[1] = char_p; // p
            gGlobalSeasonName[2] = char_r; // r
            gGlobalSeasonName[3] = char_i; // i
            gGlobalSeasonName[4] = char_n; // n
            gGlobalSeasonName[5] = char_g; // g
            return;
        case SUMMER:
            gGlobalSeasonName[0] = char_S; // S
            gGlobalSeasonName[1] = char_u; // u
            gGlobalSeasonName[2] = char_m; // m
            gGlobalSeasonName[3] = char_m; // m
            break;
        case FALL:
            gGlobalSeasonName[0] = char_A; // A
            gGlobalSeasonName[1] = char_u; // u
            gGlobalSeasonName[2] = char_t; // t
            gGlobalSeasonName[3] = char_u; // u
            gGlobalSeasonName[4] = char_m; // m
            gGlobalSeasonName[5] = char_n; // n
            return;
        case WINTER:
            gGlobalSeasonName[0] = char_W; // W
            gGlobalSeasonName[1] = char_i; // i
            gGlobalSeasonName[2] = char_n; // n
            gGlobalSeasonName[3] = char_t; // t
            break;
        default:
            return;
        }
    
    gGlobalSeasonName[4] = char_e; // e
    gGlobalSeasonName[5] = char_r; // r
}
