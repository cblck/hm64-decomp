#include "common.h"

#include "system/map.h"

#include "system/graphic.h"
#include "system/mathUtils.h"
#include "system/worldGraphics.h"

#include "mainproc.h"

// forward declarations
void func_80035A58(UnknownMapStruct2*);
void func_800366F4(Vec3f*, u16, f32, f32);                  
u16 func_80036880(u32, f32, f32);  
u8* func_800374C0(UnknownMapStruct5*, u8*);
u8* func_80037614(MapVtx* arg0, VtxTexture* arg1);      
u8* func_80037650(u16 arg0, void *arg1);
bool func_80037668(MainMap*, u16, f32, f32, f32);
Gfx* func_80037BC4(Gfx*, MainMap*, u16, u16);
s16 func_80037F08(Gfx*, MainMap*, UnknownMapStruct5*);  
Gfx* func_800383B0(Gfx* arg0, MainMap* arg1, u16 arg2, f32 arg3, f32 arg4, f32 arg5);
void func_8003851C(MainMap*);              
void func_80038BC4(MainMap*);                             
void func_800393E0(MainMap*);                             
void func_80039990(MainMap*);                             
void func_80039E20(u16, Gfx*);                        
void func_80039F58(u16);                               
bool func_8003B1BC(MainMap*);               

// bss
extern MainMap mainMap[1];
 
// inner struct member
extern u16 D_80142868;

extern f32 D_8013D550;
extern f32 D_80170460;
extern u8 D_801FB5CB;
extern u8 D_801FB700;
extern f32 D_801FB5D4;
extern f32 D_802226EC;

extern Vtx D_80223380[2][0x1400];
extern Mtx D_80165500[2][0x140];

// rodata
extern Gfx D_8011ED68[4];
extern Gfx D_8011ED88[5];
extern Gfx D_8011EDC8[3];
extern Gfx D_8011ED80;
extern Gfx D_8011EDA8;
extern Gfx D_8011EDD8;

//INCLUDE_ASM(const s32, "system/map", func_800337D0);

void func_800337D0(void) {
 
    u16 i, j; 
     
    D_801FB700 = 0;
    D_801FB5CB = 0;
    
    for (i = 0; i < 1; i++) {
        
        mainMap[i].mapStruct8.flags = 0;
        mainMap[i].mapStruct8.unk_A = 0;
        mainMap[i].mapStruct8.unk_C = 0;
        mainMap[i].mapStruct8.height = 0;

        mainMap[i].mapStruct7.unk_0.x = 0;
        mainMap[i].mapStruct7.unk_0.y = 0;
        mainMap[i].mapStruct7.unk_0.z = 0;

        mainMap[i].mapStruct7.unk_C.x = 1.0f;
        mainMap[i].mapStruct7.unk_C.y = 1.0f;
        mainMap[i].mapStruct7.unk_C.z = 1.0f;

        mainMap[i].mapStruct7.angles.x = 0;
        mainMap[i].mapStruct7.angles.y = 0;
        mainMap[i].mapStruct7.angles.z = 0;

        mainMap[i].mapStruct7.groundRgba.r = 255.0f;
        mainMap[i].mapStruct7.groundRgba.g = 255.0f;
        mainMap[i].mapStruct7.groundRgba.b = 255.0f;
        mainMap[i].mapStruct7.groundRgba.a = 255.0f;

        mainMap[i].mapStruct2.unk_48 = 0;
        mainMap[i].mapStruct2.unk_49 = 0;
        mainMap[i].mapStruct2.unk_4A = 0;
        mainMap[i].mapStruct2.unk_4B = 0;

        mainMap[i].mapStruct2.unk_3C.x = 0;
        mainMap[i].mapStruct2.unk_3C.y = 0;
        mainMap[i].mapStruct2.unk_3C.z = 0;

        mainMap[i].mapStruct2.unk_0.x = 0;
        mainMap[i].mapStruct2.unk_0.y = 0;
        mainMap[i].mapStruct2.unk_0.z = 0;

        mainMap[i].mapStruct2.unk_C.x = 0;
        mainMap[i].mapStruct2.unk_C.y = 0;
        mainMap[i].mapStruct2.unk_C.z = 0;
        
        mainMap[i].mapStruct2.unk_18.x = 0;
        mainMap[i].mapStruct2.unk_18.y = 0;
        mainMap[i].mapStruct2.unk_18.z = 0;

        mainMap[i].mapStruct2.unk_24.x = 0;
        mainMap[i].mapStruct2.unk_24.y = 0;
        mainMap[i].mapStruct2.unk_24.z = 0;

        mainMap[i].mapStruct2.unk_30.x = 0;
        mainMap[i].mapStruct2.unk_30.y = 0;
        mainMap[i].mapStruct2.unk_30.z = 0;
 
        for (j = 0; j < 0x400; j++) {
            mainMap[i].vtxs[j].currentVtxIndex = 0;
            mainMap[i].vtxs[j].vtxCount = 0;
            mainMap[i].vtxs[j].ob[0] = 0;
            mainMap[i].vtxs[j].ob[1] = 0;
            mainMap[i].vtxs[j].ob[2] = 0;
            mainMap[i].vtxs[j].flags = 0;
        }
    }
} 

INCLUDE_ASM(const s32, "system/map", func_80033A90);

//INCLUDE_ASM(const s32, "system/map", func_80034090);

bool func_80034090(u16 mapIndex) {

    u8 i;
    bool result = 0;

    if (mapIndex == 0 && (mainMap[mapIndex].mapStruct8.flags & 1)) {
        
        D_801FB700 = 0;
        D_801FB5CB = 0;

        for (i = 0; i < 0x10; i++) {
            
            if (mainMap[mapIndex].mapStruct4[i].flags & 1) {
                func_8002B6B8(mainMap[mapIndex].mapStruct4[i].spriteIndex);
            }
            
            mainMap[mapIndex].mapStruct4[i].flags = 0;
        }
        
        for (i = 0; i < 0x10; i++) { 
            
            if (mainMap[mapIndex].mapStruct5[i].flags & 1) {
                func_8002B6B8(mainMap[mapIndex].mapStruct5[i].spriteIndex);
            }

            mainMap[mapIndex].mapStruct5[i].flags = 0;
        }
        
        mainMap[mapIndex].mapStruct8.flags = 0;
        result = 1;
        
    }

    return result;
}

//INCLUDE_ASM(const s32, "system/map", func_8003423C);

bool func_8003423C(u16 mapIndex, f32 arg1, f32 arg2, f32 arg3) {

    bool result = 0;
    
    if (mapIndex == 0 && (mainMap[mapIndex].mapStruct8.flags & 1)) {
        result = 1;
        mainMap[mapIndex].mapStruct7.unk_0.x = arg1;
        mainMap[mapIndex].mapStruct7.unk_0.y = arg2;
        mainMap[mapIndex].mapStruct7.unk_0.z = arg3;
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/map", func_80034298);

bool func_80034298(u16 mapIndex, f32 arg1, f32 arg2, f32 arg3) {

    bool result = 0;
    
    if (mapIndex == 0 && (mainMap[mapIndex].mapStruct8.flags & 1)) {
        result = 1;
        mainMap[mapIndex].mapStruct7.unk_C.x = arg1;
        mainMap[mapIndex].mapStruct7.unk_C.y = arg2;
        mainMap[mapIndex].mapStruct7.unk_C.z = arg3;
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/map", func_800342F4);

bool func_800342F4(u16 mapIndex, f32 arg1, f32 arg2, f32 arg3) {

    bool result = 0;
    
    if (mapIndex == 0 && (mainMap[mapIndex].mapStruct8.flags & 1)) {
        result = 1;
        mainMap[mapIndex].mapStruct7.angles.x = arg1;
        mainMap[mapIndex].mapStruct7.angles.y = arg2;
        mainMap[mapIndex].mapStruct7.angles.z = arg3;
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/map", func_80034350);

bool func_80034350(u16 mapIndex, u8 arg1, u8 arg2, u8 arg3, u8 arg4) {
    
    bool result = 0;
    
    if (mapIndex == 0 && (mainMap[mapIndex].mapStruct8.flags & 1)) {
        
        result = 1; 

        mainMap[mapIndex].mapStruct7.groundRgba.r = arg1;
        mainMap[mapIndex].mapStruct7.groundRgba.g = arg2;
        mainMap[mapIndex].mapStruct7.groundRgba.b = arg3;
        mainMap[mapIndex].mapStruct7.groundRgba.a = arg4;
        
        mainMap[mapIndex].mapStruct7.defaultRgba.r = arg1;
        mainMap[mapIndex].mapStruct7.defaultRgba.g = arg2;
        mainMap[mapIndex].mapStruct7.defaultRgba.b = arg3;
        mainMap[mapIndex].mapStruct7.defaultRgba.a = arg4;
    
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/map", func_800343FC);

bool func_800343FC(u16 mapIndex, u8 arg1, u8 arg2, u8 arg3, u8 arg4, f32 arg5, f32 arg6, f32 arg7, u8 arg8) {

    bool result = 0;

    if (mapIndex == 0 && (mainMap[mapIndex].mapStruct8.flags & 1)) {

        mainMap[mapIndex].mapStruct2.unk_48 = arg1;
        mainMap[mapIndex].mapStruct2.unk_49 = arg2;
        mainMap[mapIndex].mapStruct2.unk_4A = arg3;
        mainMap[mapIndex].mapStruct2.unk_4B = arg4;

        mainMap[mapIndex].mapStruct2.unk_3C.x = -arg5;
        mainMap[mapIndex].mapStruct2.unk_3C.y = -arg6;
        mainMap[mapIndex].mapStruct2.unk_3C.z = -arg7;
        
        if (arg8 == 1) {
            mainMap[mapIndex].mapStruct8.flags |= 4;
        } else {
            mainMap[mapIndex].mapStruct8.flags &= ~4;
        }

        func_80035A58(&mainMap[mapIndex].mapStruct2);
        
        result = 1;
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/map", func_800344E8);

bool func_800344E8(u16 mapIndex, f32 arg1, f32 arg2, f32 arg3) {

    bool result = 0;
    
    if (mapIndex == 0 && (mainMap[mapIndex].mapStruct8.flags & 1)) {
        result = 1;
        mainMap[mapIndex].mapStruct7.unk_0.x += arg1;
        mainMap[mapIndex].mapStruct7.unk_0.y += arg2;
        mainMap[mapIndex].mapStruct7.unk_0.z += arg3;
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/map", func_80034568);

bool func_80034568(u16 mapIndex, f32 arg1, f32 arg2, f32 arg3) {

    bool result = 0;
    
    if (mapIndex == 0 && (mainMap[mapIndex].mapStruct8.flags & 1)) {
        result = 1;
        mainMap[mapIndex].mapStruct7.unk_C.x += arg1;
        mainMap[mapIndex].mapStruct7.unk_C.y += arg2;
        mainMap[mapIndex].mapStruct7.unk_C.z += arg3;
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/map", func_800345E8);

bool func_800345E8(u16 mapIndex, f32 arg1, f32 arg2, f32 arg3) {

    bool result = 0;
    
    if (mapIndex == 0 && (mainMap[mapIndex].mapStruct8.flags & 1)) {
        result = 1;
        mainMap[mapIndex].mapStruct7.angles.x += arg1;
        mainMap[mapIndex].mapStruct7.angles.y += arg2;
        mainMap[mapIndex].mapStruct7.angles.z += arg3;
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/map", func_80034668);

bool func_80034668(u16 mapIndex, s8 arg1, s8 arg2, s8 arg3, s8 arg4) {

    bool result = 0;
    
    if (mapIndex == 0 && (mainMap[mapIndex].mapStruct8.flags & 1)) {
        result = 1;
        mainMap[mapIndex].mapStruct7.groundRgba.r += arg1;
        mainMap[mapIndex].mapStruct7.groundRgba.g += arg2;
        mainMap[mapIndex].mapStruct7.groundRgba.b += arg3;
        mainMap[mapIndex].mapStruct7.groundRgba.a += arg4;
    }
    
    return result;
}

INCLUDE_ASM(const s32, "system/map", func_80034738);

// bool func_80034738(u16 arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4, s16 arg5) {

//     bool result;

//     f32 tempF;
    
//     s16 temp = getAbsoluteValue(arg5);

//     result = 0;

//     if (arg0 == 0 && (mainMap[arg0].mapStruct8.flags & 1)) {

//         mainMap[arg0].mapStruct7.defaultRgba.r = arg1;
//         mainMap[arg0].mapStruct7.defaultRgba.g = arg2;
//         mainMap[arg0].mapStruct7.defaultRgba.b = arg3;
//         mainMap[arg0].mapStruct7.defaultRgba.a = arg4;
        
//         handleRgba(arg0, temp, tempF);

//         result = 1;
//     }
    

//     return result;
// }

//INCLUDE_ASM(const s32, "system/map", func_80034A6C);

bool func_80034A6C(u16 mapIndex, s8 arg1, s8 arg2, s8 arg3, u8 arg4, f32 arg5, f32 arg6, f32 arg7) {

    bool result = 0; 

    if (mapIndex == 0 && (mainMap[mapIndex].mapStruct8.flags & 1)) { 

        mainMap[mapIndex].mapStruct2.unk_48 += arg1;

        mainMap[mapIndex].mapStruct2.unk_0.y = 0;
        mainMap[mapIndex].mapStruct2.unk_C.y = 0;
        mainMap[mapIndex].mapStruct2.unk_18.y = 0;
        mainMap[mapIndex].mapStruct2.unk_24.y = 0;
        
        mainMap[mapIndex].mapStruct2.unk_49 += arg2;
        mainMap[mapIndex].mapStruct2.unk_4A += arg3;
        mainMap[mapIndex].mapStruct2.unk_4B += arg4;

        mainMap[mapIndex].mapStruct2.unk_3C.x += arg5;
        mainMap[mapIndex].mapStruct2.unk_3C.y += arg6;
        mainMap[mapIndex].mapStruct2.unk_3C.z += arg7;

        mainMap[mapIndex].mapStruct2.unk_0.x += arg1;
        mainMap[mapIndex].mapStruct2.unk_0.z += arg2;

        mainMap[mapIndex].mapStruct2.unk_C.x += arg1;
        mainMap[mapIndex].mapStruct2.unk_C.z += arg2;

        mainMap[mapIndex].mapStruct2.unk_18.x += arg1;
        mainMap[mapIndex].mapStruct2.unk_18.z += arg2;

        mainMap[mapIndex].mapStruct2.unk_24.x += arg1;
        mainMap[mapIndex].mapStruct2.unk_24.z += arg2;
        
        result = 1;
    
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/map", func_80034C40);

bool func_80034C40(u16 mapIndex, u8 arg1, u16 arg2, u16 arg3, f32 arg4, f32 arg5, f32 arg6, u8 arg7, u8 arg8, u8 arg9, u8 argA) {

    bool result = 0; 

    if (mapIndex == 0 && (mainMap[mapIndex].mapStruct8.flags & 1)) {
         
        mainMap[mapIndex].mapStruct4[arg1].spriteIndex = arg2; 
        mainMap[mapIndex].mapStruct4[arg1].unk_E = arg3;

        mainMap[mapIndex].mapStruct4[arg1].unk_0.x = arg4;
        mainMap[mapIndex].mapStruct4[arg1].unk_0.y = arg5;
        mainMap[mapIndex].mapStruct4[arg1].unk_0.z = arg6;

        mainMap[mapIndex].mapStruct4[arg1].unk_10 = arg7;
        mainMap[mapIndex].mapStruct4[arg1].unk_11 = arg8;

        mainMap[mapIndex].mapStruct4[arg1].flags |= 1;
        
        if (arg9) {
            mainMap[mapIndex].mapStruct4[arg1].flags |= 4;
        }

        if (argA) {
            mainMap[mapIndex].mapStruct4[arg1].flags |= 8;
        }

        result = 1;
        
        mainMap[mapIndex].mapStruct8.unk_11++;
    }
    
    return result;
}
 
//INCLUDE_ASM(const s32, "system/map", func_80034D64);

bool func_80034D64(u16 mapIndex, u8 arg1, u16 arg2, u16 arg3) {

    bool result = 0;
 
    if (mapIndex == 0 && (mainMap[mapIndex].mapStruct8.flags & 1)) {
        mainMap[mapIndex].mapStruct5[arg1].spriteIndex = arg2;
        mainMap[mapIndex].mapStruct5[arg1].unk_E = arg3;
        mainMap[mapIndex].mapStruct5[arg1].flags = 1;
        result = 1;
    }

    return result; 
        
}

//INCLUDE_ASM(const s32, "system/map", func_80034DC8);

bool func_80034DC8(u16 mapIndex, u8 arg1, u16 arg2) {

    bool result = 0; 

    if (mapIndex == 0 && (mainMap[mapIndex].mapStruct8.flags & 1)) { 
        func_8002BAD8(mainMap[mapIndex].mapStruct4[arg1].spriteIndex);
        mainMap[mapIndex].mapStruct4[arg1].unk_E = arg2;
        mainMap[mapIndex].mapStruct4[arg1].flags &= ~2;
        result = 1;
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/map", func_80034E64);

bool func_80034E64(u16 mapIndex, u8 arg1) {

    bool result = 0; 

    if (mapIndex == 0 && (mainMap[mapIndex].mapStruct8.flags & 1)) { 
        if (mainMap[mapIndex].mapStruct4[arg1].flags & 2) {
            func_8002B6B8(mainMap[mapIndex].mapStruct4[arg1].spriteIndex);
            mainMap[mapIndex].mapStruct4[arg1].flags = 0;
            result = 1;
        }
    }

    return result;
    
}
INCLUDE_ASM(const s32, "system/map", func_80034EF0);
  
//INCLUDE_ASM(const s32, "system/map", func_80035004);

bool func_80035004(u16 mapIndex, u16 arg1, u8 arg2, u8 arg3) {
    
    bool result = 0;
    
    if (mapIndex == 0 && (mainMap[mapIndex].mapStruct8.flags & 1)) {
        // 2D array or nested struct?
        mainMap[mapIndex].mapStruct6.arr2[arg3*0x14+arg2] = arg1;
        result = 1;
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/map", func_80035054);

bool func_80035054(u16 mapIndex, u16 bitmapIndex, u16 arg2, f32 arg3, f32 arg4, f32 arg5) {

    bool result = 0;

    if (mapIndex == 0 && (mainMap[mapIndex].mapStruct8.flags & 1)) {
    
        if (bitmapIndex) {
            
            func_80026E78(&mainMap[mapIndex].mapBitmaps[bitmapIndex], func_80028888(arg2, mainMap[mapIndex].mapStruct6.vaddr1), func_800288B8(arg2, mainMap[mapIndex].mapStruct6.vaddr2, mainMap[mapIndex].mapStruct6.vaddr3));
             
            mainMap[mapIndex].mapBitmaps[bitmapIndex].unk_1C.x = arg3;
            mainMap[mapIndex].mapBitmaps[bitmapIndex].unk_1C.y = arg4;
            mainMap[mapIndex].mapBitmaps[bitmapIndex].unk_1C.z = arg5;
            
            result = 1;
        }    
    }
    
    return result;
}

INCLUDE_ASM(const s32, "system/map", func_80035150);

INCLUDE_ASM(const s32, "system/map", func_8003544C);

//INCLUDE_ASM(const s32, "system/map", func_80035914);

bool func_80035914(u16 mapIndex, f32 arg1, f32 arg2) {

    Vec3f vec;
    
    bool result = 0;

    if (mapIndex == 0 && (mainMap[mapIndex].mapStruct8.flags & 1)) {

        func_800366F4(&vec, 0, arg1, arg2);

        if (vec.y != MAX_UNSIGNED_SHORT) {
            result = func_80036880(0, vec.x, vec.z) != 0;
        }

    }
    
    return result;
}
 
//INCLUDE_ASM(const s32, "system/map", func_800359C8);

Vec3f* func_800359C8(Vec3f* arg0, MainMap* arg1, f32 arg2, f32 arg3) {
    
    Vec3f vec;

    vec.x = (arg2 + arg1->mapStruct8.unk_0) / arg1->mapStruct1.unk_8;
    vec.y = 0;
    vec.z = (arg3 + arg1->mapStruct8.unk_4) / arg1->mapStruct1.unk_9;

    *arg0 = vec;
    
    return arg0;
}

INCLUDE_ASM(const s32, "system/map", func_80035A58);

//INCLUDE_ASM(const s32, "system/map", func_80035DA4);

bool func_80035DA4(MainMap *map, u8 arg1, u8 arg2) {
    
  bool result = 0;

  if (-1.0f <= (((map->mapStruct2.unk_0.z * (map->mapStruct2.unk_C.x - arg1)) + (map->mapStruct2.unk_C.z * (arg1 - map->mapStruct2.unk_0.x))) + (arg2 * D_802226EC))) {
    if (-1.0f <= (((map->mapStruct2.unk_C.z * (map->mapStruct2.unk_18.x - arg1)) + (map->mapStruct2.unk_18.z * (arg1 - map->mapStruct2.unk_C.x))) + (arg2 * D_80170460))) {
      if (-1.0f <= (((map->mapStruct2.unk_18.z * (map->mapStruct2.unk_24.x - arg1)) + (map->mapStruct2.unk_24.z * (arg1 - map->mapStruct2.unk_18.x))) + (arg2 * D_8013D550))) {
        if (-1.0f <= (((map->mapStruct2.unk_24.z * (map->mapStruct2.unk_0.x - arg1)) + (map->mapStruct2.unk_0.z * (arg1 - map->mapStruct2.unk_24.x))) + (arg2 * D_801FB5D4))) {
          result = 1;
        }
      }
    }
  }
    
  return result;
}

INCLUDE_ASM(const s32, "system/map", func_80035EE0);

INCLUDE_ASM(const s32, "system/map", func_800360BC);

// returns index for interactable object/exit from rodata array per level
INCLUDE_ASM(const s32, "system/map", func_80036318);

INCLUDE_ASM(const s32, "system/map", func_80036490);

//INCLUDE_ASM(const s32, "system/map", func_80036610);
 
Vec3f *func_80036610(Vec3f *arg0, u16 mapIndex, f32 arg2, f32 arg3) {

    Vec3f vec;
    Vec3f vec2;

    vec.x = 0.0f;
    vec.z = 0.0f;
    vec.y = MAX_UNSIGNED_SHORT;

    if (mapIndex == 0 && (mainMap[mapIndex].mapStruct8.flags & 1)) {
        
        vec2.y = 0;
        vec2.x = (arg2 + mainMap[mapIndex].mapStruct8.unk_0) / mainMap[mapIndex].mapStruct1.unk_8;
        vec2.z = (arg3 + mainMap[mapIndex].mapStruct8.unk_4) / mainMap[mapIndex].mapStruct1.unk_9;

        vec = vec2; 
    }
 
    *arg0 = vec;
    
    return arg0;
}

INCLUDE_ASM(const s32, "system/map", func_800366F4);

INCLUDE_ASM(const s32, "system/map", func_80036880);

INCLUDE_ASM(const s32, "system/map", func_80036980);

//INCLUDE_ASM(const s32, "system/map", func_80036A84);

bool func_80036A84(u16 mapIndex) {

    bool result = 0;
     
    if (mapIndex == 0 && (mainMap[mapIndex].mapStruct8.flags & 1)) {
        result = (mainMap[mapIndex].mapStruct8.flags >> 3) & 1;
    }

    return result;
}

//INCLUDE_ASM(const s32, "system/map", func_80036AB4);

void func_80036AB4(MainMap* map) {

    u16 i;
    u16 j;
    u16 count;
    u8* ptr;
    u16 temp;
    UnknownMapStruct5 mapStructs[32]; 

    count = 0;
    i = 0;
  
    if (map->mapStruct1.unk_6) {
        
        do {

            ptr = func_80037614(&map->vtxs[i], func_80037650(i, map->unk_0));

            j = 0;
            
            while (1) {

                ptr =  func_800374C0(&mapStructs[j], ptr);
    
                if (mapStructs[j].flags & 0x80) {
                    // probably should be a flag?
                    map->vtxs[i].ob[2] |= 0x80;
                }

                if (mapStructs[j++].flags & 0x10) {
                    break;
                } 

            } 

            map->vtxs[i].currentVtxIndex = count;
            temp = func_80037F08(&map->displayLists[count], map, mapStructs);

            count += temp;
            map->vtxs[i].vtxCount = temp;
            
            i++;
            
        } while (i < map->mapStruct1.unk_6);
    }  
} 

INCLUDE_ASM(const s32, "system/map", func_80036C08);

INCLUDE_ASM(const s32, "system/map", func_80036FA0);

INCLUDE_ASM(const s32, "system/map", func_80037254);

#ifdef PERMUTER
void func_800372F0(UnknownMapStruct1* arg0, Unknown1* arg1) {

    Unknown1 struc;
    
    arg0->unk_8 = arg1->unk_4;
    arg0->unk_9 = arg1->unk_5;
    arg0->unk_A = arg1->unk_6;
    arg0->unk_B = arg1->unk_7;
    
    arg0->unk_4 = arg1->unk_9 << 8 | arg1->unk_8;
    arg0->unk_6 = arg1->unk_B << 8 | arg1->unk_A;
    
    arg0->ptr = arg1+1;

}
#else
INCLUDE_ASM(const s32, "system/map", func_800372F0);
#endif

INCLUDE_ASM(const s32, "system/map", func_80037350);

INCLUDE_ASM(const s32, "system/map", func_80037388);

INCLUDE_ASM(const s32, "system/map", func_80037400);

INCLUDE_ASM(const s32, "system/map", func_800374C0);

//INCLUDE_ASM(const s32, "system/map", func_80037614);

// param 2 = 80255A10 + (n * 0x28)
u8* func_80037614(MapVtx* arg0, VtxTexture* arg1) {

    u8 *ptr;
    
    arg0->ob[0] = arg1->cn[0];
    arg0->ob[1] = arg1->cn[1];
    arg0->ob[2] = arg1->cn[2];
    arg0->flags = arg1->cn[3];

    ptr = arg1+1;
    
    arg0->vtx = ptr;
    
    return (ptr + arg0->flags*3);
    
}

/*
u8* func_80037614(VtxTextureInfo* arg0, VtxTexture* arg1) {

    u8 *ptr;
    
    arg0->vtxTex.cn[0] = arg1->cn[0];
    arg0->vtxTex.cn[1] = arg1->cn[1];
    arg0->vtxTex.cn[2] = arg1->cn[2];
    arg0->vtxTex.cn[3] = arg1->cn[3];

    ptr = arg1+1;
    
    arg0->vtx = ptr;
    
    return (ptr + (arg0->vtxTex.cn[3]*3));
    
}
*/

// alternate
/*
UnknownMapStruct9* func_80037614(VtxTextureInfo* arg0, UnknownMapStruct9* arg1) {

    void *ptr;
    
    arg0->unk_8 = arg1->unk_4;
    arg0->unk_9 = arg1->unk_5;
    arg0->unk_A = arg1->unk_6;
    arg0->unk_B = arg1->unk_7;

    ptr = arg1;
    ptr += 8;
    
    arg0->ptr = ptr;
    
    return (UnknownMapStruct9*)(ptr + (arg0->unk_B*3));
    
}
*/

//INCLUDE_ASM(const s32, "system/map", func_80037650);

u8* func_80037650(u16 arg0, void *arg1) {
    return (u8*)(arg1 + *(u32*)(arg1 + arg0*4));
}

// alternate
/*
s32 func_80037650(u16 arg0, s32 arg1) {
    return arg1 + *(s32*)(arg0*4 + arg1);
}
*/

INCLUDE_ASM(const s32, "system/map", func_80037668);

//INCLUDE_ASM(const s32, "system/map", func_8003797C);

Gfx* func_8003797C(Gfx* dl, MainMap* arg1, u8 arg2) {

    Tile bitmap;

    func_80026E78((Bitmap*)&bitmap, func_80028888(arg2, arg1->unk_10), func_800288A0(arg2, arg1->unk_14));

    gDPLoadTextureTile_4b(dl++, bitmap.timg, bitmap.fmt, bitmap.width, bitmap.height, 0, 0, bitmap.width - 1, bitmap.height - 1, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    gDPLoadTLUT_pal16(dl++, 0, bitmap.pal);

    return dl++;
}

INCLUDE_ASM(const s32, "system/map", func_80037BC4);

//INCLUDE_ASM(const s32, "system/map", func_80037DF0);

inline Gfx* func_80037DF0(Gfx* dl, MainMap* arg1, u16 arg2) {

    u8 i;
    u16 temp;
    
    arg1->mapStruct8.unk_A = 0;
    arg1->mapStruct8.unk_C = arg2;

    *dl = D_8011ED68[0]; 
    dl++;
    *dl = D_8011ED68[1]; 
    dl++;
    *dl = D_8011ED68[2]; 
    dl++;

    for (i = 0; i < 0x51; i++) {
        temp = arg1->unk_arr[i];
        if (temp != 0xFFFF) {
            dl = func_80037BC4(dl, arg1, temp, i);
        }
    }

    *dl = D_8011ED68[3];

    arg1->mapStruct8.height = arg1->mapStruct8.unk_A;

    dl++;
    
    return dl++;
    
}

INCLUDE_ASM(const s32, "system/map", func_80037F08);

INCLUDE_ASM(const s32, "system/map", func_800383B0);

void func_80038514(void) {}

INCLUDE_ASM(const s32, "system/map", func_8003851C);

//INCLUDE_ASM(const s32, "system/map", func_80038630);

// decompress vec3fs
void func_80038630(Decompressed* arg0, Compressed* arg1) {

    u32 padding[8];
    
    arg0->size = arg1->size;
    arg0->x = (s16)(arg1->part1 | (arg1->part2 << 8));
    arg0->y = (s16)(arg1->part3 | (arg1->part4 << 8));
    arg0->z = (s16)(arg1->part5 | (arg1->part6 << 8));
}

//INCLUDE_ASM(const s32, "system/map", func_800386C0);

void func_800386C0(UnknownMapStruct3* arg0, u8* arg1) {
    arg0->unk_2 = arg1[0];
    arg0->unk_3 = arg1[1];
}

#ifdef PERMUTER
u8* func_800386D4(u16 arg0, u8* arg1) {

    u16 i;
    u8 arr[4];

    arg1++;

    for (i = 0; i < arg0; i++) {
        arr[2] = arg1[0];
        arg1 += arg1[1] * 7;
        arr[3] = 2;
   }

    return arg1;

}
#else
INCLUDE_ASM(const s32, "system/map", func_800386D4);
#endif

INCLUDE_ASM(const s32, "system/map", func_80038728);

//INCLUDE_ASM(const s32, "system/map", func_800387E0);

u8* func_800387E0(u16 arg0, void* arg1) {
    return (u8*)(arg1 + *(u32*)(arg1 + arg0*4));
}

INCLUDE_ASM(const s32, "system/map", func_800387F8);

//INCLUDE_ASM(const s32, "system/map", func_80038810);

bool func_80038810(u16 mapIndex) {

    bool result;

    if (mapIndex == 0 && (mainMap[mapIndex].mapStruct8.flags & 1)) {
        if (!(mainMap[mapIndex].mapStruct8.flags & 2)) {
            mainMap[mapIndex].mapStruct8.flags |= 2;
            func_80036FA0(0);
        }
    }

    // never initialized; whoops
    return result;
}

//INCLUDE_ASM(const s32, "system/map", func_8003886C);

bool func_8003886C(u16 mapIndex) {
    
    bool result  = 0;

    if (mapIndex == 0 && (mainMap[mapIndex].mapStruct8.flags & 1)) {
        mainMap[mapIndex].mapStruct8.flags &= ~2;
        result = 1;
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/map", func_800388A4);

// param_2 = mainMap.unk_20 (set from gTileContext in func_80033A90, called by func_8003BC50)
u8 *func_800388A4(u16 arg0, u8 *arg1) {
    
    u32 temp1;
    u32 temp2;

    u16 i;
    
    arg1 += 6;
    
    i = 0;
    
    if (arg0) {

        do {
            
            arg1 += 2;
            
            temp1 = *arg1;
            
            arg1++;
            
            temp2 = *arg1;
            temp2 = temp2 * temp1;

            arg1++;
            
            arg1 += temp2 * 2;
            arg1 += temp2;

            i++;
            
        } while (i < arg0);
    }
    
    return arg1;
}

INCLUDE_ASM(const s32, "system/map", func_80038900);

INCLUDE_ASM(const s32, "system/map", func_80038990);

INCLUDE_ASM(const s32, "system/map", func_80038A2C);

INCLUDE_ASM(const s32, "system/map", func_80038AE0);

//INCLUDE_ASM(const s32, "system/map", func_80038B58);

bool func_80038B58(u16 mapIndex, u16 arg1, u8 arg2, u8 arg3) {
    
    bool result  = 0;

    if (mapIndex == 0 && (mainMap[mapIndex].mapStruct8.flags & 1)) {
        func_800360BC(0, *(&D_80142868 + arg1 * 0x24), arg2 , arg3, 2);
        result = 1;
    }
    
    return result;
}

INCLUDE_ASM(const s32, "system/map", func_80038BC4);

INCLUDE_ASM(const s32, "system/map", func_800393E0);

INCLUDE_ASM(const s32, "system/map", func_80039990);

//INCLUDE_ASM(const s32, "system/map", func_80039E00);

//INCLUDE_ASM(const s32, "system/map", func_80039E20);

void func_80039E20(u16 arg0, Gfx* arg1) {

    u16 temp = func_8002929C(arg1, 0x28);
    
    func_800292EC(temp, mainMap[arg0].mapStruct7.unk_0.x + mainMap[arg0].mapStruct2.unk_3C.x, 
        mainMap[arg0].mapStruct7.unk_0.y + mainMap[arg0].mapStruct2.unk_3C.y, 
        mainMap[arg0].mapStruct7.unk_0.z + mainMap[arg0].mapStruct2.unk_3C.z);

    func_80029330(temp, mainMap[arg0].mapStruct7.unk_C.x, mainMap[arg0].mapStruct7.unk_C.y, mainMap[arg0].mapStruct7.unk_C.z);
    
    func_80029374(temp, mainMap[arg0].mapStruct7.angles.x, mainMap[arg0].mapStruct7.angles.y, mainMap[arg0].mapStruct7.angles.z);
}


INCLUDE_ASM(const s32, "system/map", func_80039F58);

INCLUDE_ASM(const s32, "system/map", func_8003A1BC);

//INCLUDE_ASM(const s32, "system/map", func_8003AC14);

inline void func_8003AC14(Gfx* dl, UnknownMapStruct8* arg1) {
    
    // gsDPSetCombineMode(G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA)
    // gsDPSetRenderMode(G_RM_TEX_EDGE, G_RM_TEX_EDGE2)
    // gsSPTexture(qu016(0.5), qu016(0.5), 0, G_TX_RENDERTILE, G_ON)
    // gsDPSetCombineMode(G_CC_MODULATEIA, G_CC_MODULATEIA)
    
    *dl = D_8011ED88[1];
    dl++;
    *dl = D_8011EDC8[0];
    dl++;
    *dl = D_8011EDC8[1];
    dl++;
    *dl = D_8011ED68[2];
    dl++;

    func_80026F88(dl++, (Bitmap*)arg1, 0, arg1->height);
}
 
// param1: Gfx* 8020DE78
// param2: MainMap*
// param3: mapBitmap*, 8014318C
// param4 = offset into vertex bank
// Vertex bank: D_80165500[]
//      size 0x40
//INCLUDE_ASM(const s32, "system/map", func_8003ACA8);

Gfx* func_8003ACA8(Gfx* arg0, MainMap* arg1, MapBitmap* arg2, u16 arg3) {

    Gfx dl[2];
    
    func_800276AC(&D_80165500[gDisplayContextIndex][arg3], arg2->height, arg2->width, arg2->width, 0, 0,
        0, 0, 0, 0x150, arg1->mapStruct7.groundRgba.r, arg1->mapStruct7.groundRgba.g, arg1->mapStruct7.groundRgba.b,
        arg1->mapStruct7.groundRgba.a);

    gSPVertex(&dl[1], &D_80165500[gDisplayContextIndex][arg3], 4, 0);

    *dl = *(dl+1);
    *arg0 = *dl;
    
    arg0++;
    *arg0 = D_8011EDD8;
    arg0++;
    *arg0 = D_8011EDA8;
    arg0++;
    *arg0 = D_8011ED80;
    arg0++;
    
    return arg0++;
    
}

INCLUDE_ASM(const s32, "system/map", func_8003AF58);

//INCLUDE_ASM(const s32, "system/map", func_8003B100);

void func_8003B100(MainMap* map, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, Gfx* dl) {

    Vec3f vec;
    
    f32 temp_f20;
    f32 temp_f22;
    f32 temp_f24;

    temp_f22 = (arg1 + map->mapStruct8.unk_0 + map->mapStruct2.unk_3C.x) - arg4;
    temp_f24 = arg2 + map->mapStruct2.unk_3C.y;
    temp_f20 = (arg3 + map->mapStruct8.unk_4 + map->mapStruct2.unk_3C.z) - arg5;
    
    func_800292EC(func_8002929C(dl, 8), temp_f22, temp_f24, temp_f20);
}

// main loop function
INCLUDE_ASM(const s32, "system/map", func_8003B1BC);


// static const Gfx D_8011ED68[4] = {

//     gsDPSetCombineMode(G_CC_MODULATEIA, G_CC_MODULATEIA),
//     gsDPSetRenderMode(G_RM_RA_ZB_OPA_SURF, G_RM_RA_ZB_OPA_SURF2),
//     gsDPSetTextureFilter(G_TF_BILERP),
//     gsSPEndDisplayList()

// };

// gsDPSetCombineMode(G_CC_MODULATEIA, G_CC_MODULATEIA)

INCLUDE_RODATA(const s32, "system/map", D_8011ED68);

INCLUDE_RODATA(const s32, "system/map", D_8011ED6C);

// gsDPSetRenderMode(G_RM_RA_ZB_OPA_SURF, G_RM_RA_ZB_OPA_SURF2)

INCLUDE_RODATA(const s32, "system/map", D_8011ED70);

INCLUDE_RODATA(const s32, "system/map", D_8011ED74);

// gsDPSetTextureFilter(G_TF_BILERP)

INCLUDE_RODATA(const s32, "system/map", D_8011ED78);

INCLUDE_RODATA(const s32, "system/map", D_8011ED7C);

// gsSPEndDisplayList()

INCLUDE_RODATA(const s32, "system/map", D_8011ED80);

INCLUDE_RODATA(const s32, "system/map", D_8011ED84);


// static const Gfx D_8011ED88[5] = {

//     gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_ON),
//     gsDPSetCombineMode(G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA),
//     gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF),
//     gsDPSetCombineLERP(PRIMITIVE, 0, SHADE, 0, PRIMITIVE, 0, SHADE, 0, PRIMITIVE, 0, SHADE, 0, PRIMITIVE, 0, SHADE, 0),
//     gsDPPipeSync()

// };

// gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_ON),

INCLUDE_RODATA(const s32, "system/map", D_8011ED88);

INCLUDE_RODATA(const s32, "system/map", D_8011ED8C);

// gsDPSetCombineMode(G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA)

INCLUDE_RODATA(const s32, "system/map", D_8011ED90);

INCLUDE_RODATA(const s32, "system/map", D_8011ED94);

// gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF)

INCLUDE_RODATA(const s32, "system/map", D_8011ED98);

INCLUDE_RODATA(const s32, "system/map", D_8011ED9C);

// gsDPSetCombineMode
// gsDPSetCombineLERP(PRIMITIVE, 0, SHADE, 0, PRIMITIVE, 0, SHADE, 0, PRIMITIVE, 0, SHADE, 0, PRIMITIVE, 0, SHADE, 0)

INCLUDE_RODATA(const s32, "system/map", D_8011EDA0);

INCLUDE_RODATA(const s32, "system/map", D_8011EDA4);

// gsDPPipeSync()

INCLUDE_RODATA(const s32, "system/map", D_8011EDA8);

INCLUDE_RODATA(const s32, "system/map", D_8011EDAC);


// binary data

INCLUDE_RODATA(const s32, "system/map", D_8011EDB0);

INCLUDE_RODATA(const s32, "system/map", D_8011EDB4);


// static const Gfx D_8011EDC8[3] = {

//     gsDPSetRenderMode(G_RM_TEX_EDGE, G_RM_TEX_EDGE2),
//     gsSPTexture(qu016(0.5), qu016(0.5), 0, G_TX_RENDERTILE, G_ON),
//     gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0)

// };

// gsDPSetRenderMode(G_RM_TEX_EDGE, G_RM_TEX_EDGE2)

INCLUDE_RODATA(const s32, "system/map", D_8011EDC8);

INCLUDE_RODATA(const s32, "system/map", D_8011EDCC);

// gsSPTexture(qu016(0.5), qu016(0.5), 0, G_TX_RENDERTILE, G_ON)

INCLUDE_RODATA(const s32, "system/map", D_8011EDD0);

INCLUDE_RODATA(const s32, "system/map", D_8011EDD4);

// gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0)

INCLUDE_RODATA(const s32, "system/map", D_8011EDD8);

INCLUDE_RODATA(const s32, "system/map", D_8011EDDC);
