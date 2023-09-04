#include "common.h"

#include "weather.h"

#include "system/map.h"
#include "system/sprite.h"

// likely bss
extern u8 gWeather;

// data
extern u8 D_80118B50[5][4];

// shared: level.c
extern void *D_D92970;
extern void *D_D92D50;
extern void *D_D92D50_2;
extern void *D_D92D70;
extern void *D_D92D70_2;

//INCLUDE_ASM(const s32, "weather", func_800DC360);

void func_800DC360(void) {
    
    u8 i;
    u16 temp;
    
    u16 temp2 = (u8)-((gWeather == RAIN || gWeather == 4) == 0);

    if (gWeather == SNOW) {
        temp2 = 1;
    }
    
    if (temp2 != 0xFF) {
        for (i = 0; i < 10; i++) {
            
            temp = i;

            // sprite funcs
            func_8002B138(temp+0x6b, &D_D92970, &D_D92D50, &D_D92D50_2, &D_D92D70_2, 0, 0, 0x802A5AC0, 0, 0x802A5DC0, 0x802A5EC0, 0x802A5FC0, 0, 0, 1);
            func_8002BD90(temp+0x6b, 1.0f, 1.0f, 1.0f);
            func_8002C7EC(temp+0x6b, 4);
            func_8002C914(temp+0x6b, 0xFF, 0xFF, 0xFF, 0xFF);

            // map
            func_80034D64(0, i+3, temp+0x6B, temp2);
        }
    }
}

#ifdef PERMUTER
void setForecast(void) {

    u8 i;
    u8 j;
    u8 temp;
    
    u8 *ptr1;
    u8 *ptr2;
    
    UnknownStruct struct1;
    UnknownStruct struct2;

    ptr1 = (u8*)D_80123438;
    struct1 = *(UnknownStruct*)ptr1;
    ptr2 = (u8*)D_8012344C;
    struct2 =* (UnknownStruct*)ptr2;
    
    temp = getRandomNumberInRange(1, 100);
    
    i = 0;
    j = 0;
    
    while (i < 5 && j < temp) {
        if (checkLifeEventBit(0x44) || checkLifeEventBit(0x46)) {
            j += ptr2[gNextSeason*5 + i];
        } else {
            j += ptr1[gNextSeason*5 + i-5];
        }
        j+=i;
        i++;
    } 
    
    gForecast = i;
    
}
#else
INCLUDE_ASM(const s32, "weather", setForecast);
#endif

//INCLUDE_ASM(const s32, "weather", setWeatherLighting);

Vec4f setWeatherLighting(u8 weather) {

    Vec4f vec;
    
    vec.r = D_80118B50[weather - 1][0];
    vec.g = D_80118B50[weather - 1][1];
    vec.b = D_80118B50[weather - 1][2];
    vec.a = D_80118B50[weather - 1][3];
        
    return vec;
}

// static const u8 D_80123438[20];
INCLUDE_RODATA(const s32, "weather", D_80123438);

INCLUDE_RODATA(const s32, "weather", D_8012344C);
