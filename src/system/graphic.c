#include "common.h"

#include "system/graphic.h"

#include "system/sprite.h"
#include "system/worldGraphics.h"

#include "mainproc.h"

// forward declarations
void func_80026F30(HMBitmap* sprite, u16* palette);
Gfx *func_80028A64(Gfx*, Camera*, WorldMatrices*);
volatile u8 doViewportGfxTask(void);      
void setInitialWorldRotationAngles(f32, f32, f32);            

Gfx* clearFramebuffer(Gfx* dl);                  
Gfx* initRcp(Gfx*);                               
volatile u8 startGfxTask(void);

void setCameraLookAt(Camera*, f32, f32, f32, f32, f32, f32, f32, f32, f32); 
void setCameraOrthographicValues(Camera*, f32, f32, f32, f32, f32, f32); 
void setCameraPerspectiveValues(Camera*, f32, f32, f32, f32);    

// bss
LookAt gSPLookAtBufferA;
LookAt gSPLookAtBufferB;

// non-contigous bss
Camera gCamera;
Gfx initGfxList[2][0x20];
Gfx D_801836A0[2][0x500];
Gfx D_80205000[2][0x20];
extern Gfx viewportDL[3];
                        
// data, possibly external
extern Gfx setup_rdpstate[];
extern Gfx setup_rspstate[];
extern Gfx rdpstateinit_dl[];

// data
extern NUUcode nugfx_ucode[];
extern u16*	FrameBuf[3];

// rodata
static const char gfxExceptionStr1[] = "EX";
static const char gfxExceptionStr2[] = "s:/system/graphic.c";

// shared globals
// also used by mapContext.c, map.c, and worldGraphics.c
extern Vec3f previousWorldRotationAngles;
extern Vec3f currentWorldRotationAngles;
extern f32 D_80170450;
extern f32 D_80170454;


//INCLUDE_RODATA(const s32, "system/graphic", D_8011EC40);

//INCLUDE_ASM(const s32, "system/graphic", graphicsInit);

void graphicsInit(void) {

    u8 i;

    nuGfxInit();
 
    gCamera.perspectiveMode = FALSE;

    setCameraOrthographicValues(&gCamera, -160.0f, 160.0f, -120.0f, 120.0f, 0.0f, 900.0f);
    setCameraPerspectiveValues(&gCamera, 33.0f, 1.3333334f, 10.0f, 1000.0f);
    setCameraLookAt(&gCamera, 0.0f, 0.0f, 400.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 0.0f);

    for (i = 0; i < 2; i++) {
        worldMatrices[i].translation.x = 0.0f;
        worldMatrices[i].translation.y = 0.0f;
        worldMatrices[i].translation.z = 0.0f;
        worldMatrices[i].scaling.x = 1.0f;
        worldMatrices[i].scaling.y = 1.0f;
        worldMatrices[i].scaling.z = 1.0f;
        worldMatrices[i].rotation.x = 0.0f;
        worldMatrices[i].rotation.y = 0.0f;
        worldMatrices[i].rotation.z = 0.0f;
    }

    setInitialWorldRotationAngles(45.0f, 315.0f, 0.0f);  
    
}

//INCLUDE_ASM(const s32, "system/graphic", drawFrame);

void drawFrame(void) {

    gfxTaskNo = 0;

    startGfxTask();
    // set world and view matrices
    func_80026CEC();
    doViewportGfxTask();

    gDisplayContextIndex ^= 1;

}

//INCLUDE_ASM(const s32, "system/graphic", startGfxTask);

volatile u8 startGfxTask(void) {

    Gfx *dl;
 
    dl = initRcp(initGfxList[gDisplayContextIndex]);
    dl = clearFramebuffer(dl);

    gDPFullSync(dl++);
    gSPEndDisplayList(dl++);

    if (dl - initGfxList[gDisplayContextIndex] > GFX_GLIST_LEN) {
        // FIXME: get string literals working
        __assert(&gfxExceptionStr1, &gfxExceptionStr2, 288);
    }
    
    nuGfxTaskStart(initGfxList[gDisplayContextIndex], (s32)(dl - initGfxList[gDisplayContextIndex]) * sizeof(Gfx), NU_GFX_UCODE_F3DEX, NU_SC_NOSWAPBUFFER);
    
    gfxTaskNo += 1;
    
    return gfxTaskNo;
}

//INCLUDE_ASM(const s32, "system/graphic", doViewportGfxTask);

volatile u8 doViewportGfxTask(void) {

    Gfx *dl = D_80205000[gDisplayContextIndex];
    
    gSPDisplayList(dl++, OS_K0_TO_PHYSICAL(&viewportDL));
    gDPFullSync(dl++);
    gSPEndDisplayList(dl++);

    if (dl - D_80205000[gDisplayContextIndex] >= 32) {
        // FIXME: get string literals working
        __assert(&gfxExceptionStr1, &gfxExceptionStr2, 319);
    }

    nuGfxTaskStart(D_80205000[gDisplayContextIndex], (s32)(dl - D_80205000[gDisplayContextIndex]) * sizeof(Gfx), NU_GFX_UCODE_F3DEX, NU_SC_SWAPBUFFER);
    
    gfxTaskNo += 1;

    return gfxTaskNo;
    
}

//INCLUDE_ASM(const s32, "system/graphic", func_80026CEC);

volatile u8 func_80026CEC(s32 arg0, s32 arg1) {

    Gfx *dl = D_801836A0[gDisplayContextIndex];
    
    gSPDisplayList(dl++, OS_K0_TO_PHYSICAL(&viewportDL));
    
    dl = func_80028A64(dl++, &gCamera, &worldMatrices[gDisplayContextIndex]);
    dl = func_800293C0(dl++, &worldMatrices[gDisplayContextIndex]);
    
    gDPFullSync(dl++);
    gSPEndDisplayList(dl++);
    
    if (dl - D_801836A0[gDisplayContextIndex] >= 0x500) {
        // FIXME: get string literals working
        __assert(&gfxExceptionStr1, &gfxExceptionStr2, 0x166);
    }

    nuGfxTaskStart(D_801836A0[gDisplayContextIndex], (s32)(dl - D_801836A0[gDisplayContextIndex]) * sizeof(Gfx), NU_GFX_UCODE_F3DEX, NU_SC_NOSWAPBUFFER);
    
    gfxTaskNo += 1;
    
    return gfxTaskNo;

}

//INCLUDE_ASM(const s32, "system/graphic", setBitmapFormat);

// 16-bit endian swap
// FIXME: something wrong, but matches
static inline u16 swap(u16 halfword) {

    // FIXME: shouldn't be necessary/probably a union
    u32 padding[9];
    
    u16 swapped;
    
    u32 upper;
    u32 lower;
    
    lower = halfword;
    
    upper = (halfword & 0xFF) << 8;
    
    lower = halfword >> 8;
    
    swapped = lower | upper;
    
    return swapped;
    
}

// breaks match for some reason
/*
static inline u16 getWidth(u16* timg) {
    
    return swap(timg[2]);

}

static inline u16 getHeight(u16* timg) {

    return swap(timg[3]);
    
}
*/

void setBitmapFormat(HMBitmap *sprite, Texture *timg, u16 *palette) {
    
    func_80026F30(sprite, palette);
    
    // skip header and size bytes
    sprite->timg = &timg->texture;
    
    // bytes 4-8 = width and height (16 bit) (byte swapped)
    sprite->width = swap(timg->width);  
    sprite->height = swap(timg->height);
    
    // get pixel size from bit 5 in header (bit one when swapped)
    switch ((timg->flags >> 4) & 0xF) {
        case 0:
          sprite->fmt = G_IM_FMT_CI;
          sprite->pixelSize = G_IM_SIZ_8b;
          break;
        
        case 1:
          sprite->fmt = G_IM_FMT_CI;
          sprite->pixelSize = G_IM_SIZ_4b; 
          break;
    }

}

// alternate with timg as array
/*
void setBitmapFormat(Bitmap *sprite, u16 *timg, u16 *palette) {
    
    u32 padding[10];

    func_80026F30(sprite, palette);
    
    // skip header and size bytes
    sprite->timg = timg + 4;
    
    // bytes 4-8 = width and height (16 bit) (byte swapped)
    sprite->width = getWidth(timg);  
    sprite->height = getHeight(timg);
    
    // get pixel size from bit 5 in header (bit one when swapped)
    switch (*(timg + 1) >> 4 & 0xF) {
        case 0:
          sprite->fmt = G_IM_FMT_CI;
          sprite->pixelSize = G_IM_SIZ_8b;
          break;
        
        case 1:
          sprite->fmt = G_IM_FMT_CI;
          sprite->pixelSize = G_IM_SIZ_4b; 
          break;
    }
}

*/

//INCLUDE_ASM(const s32, "system/graphic", func_80026F30);

void func_80026F30(HMBitmap* sprite, u16* palette) {

    // FIXME: shouldn't be necessary
    u32 padding[5];
    
    // skip header
    sprite->pal = palette + 2;

    // get pixel size from bit 5 in header (bit one when swapped)
    switch ((*(palette + 1) >> 4) & 0xF) {                           
        case 0:
            sprite->fmt = G_IM_FMT_CI;
            sprite->pixelSize = G_IM_SIZ_8b;
            return;
        case 1:
            sprite->fmt = G_IM_FMT_CI;
            sprite->pixelSize = G_IM_SIZ_4b;
            return;
        }
}

//INCLUDE_ASM(const s32, "system/graphic", func_80026F88);

Gfx* func_80026F88(Gfx* dl, HMBitmap* sprite, u32 offset, u16 height) {
    
    switch (sprite->pixelSize) {
        case G_IM_SIZ_4b:
            gDPLoadTextureTile_4b(dl++, sprite->timg + offset, sprite->fmt, sprite->width, sprite->height, 0, 0, sprite->width - 1, height - 1, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);        
            gDPLoadTLUT_pal16(dl++, 0, sprite->pal);
            break; 
        case G_IM_SIZ_8b:
            gDPLoadTextureTile(dl++, sprite->timg + offset, sprite->fmt, G_IM_SIZ_8b, sprite->width, sprite->height, 0, 0, sprite->width - 1, height - 1, sprite->pal, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
            gDPLoadTLUT_pal256(dl++, sprite->pal);
            break;
        case G_IM_SIZ_16b:
            gDPLoadTextureBlock(dl++, sprite->timg + offset, sprite->fmt, G_IM_SIZ_16b, sprite->width, height, sprite->pal, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
            break;
        case G_IM_SIZ_32b:
            gDPLoadTextureBlock(dl++, sprite->timg + offset, sprite->fmt, G_IM_SIZ_32b, sprite->width, height, sprite->pal, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
            break;
        default:
            break;
    }

    return dl++;
    
}

//INCLUDE_ASM(const s32, "system/graphic", func_800276AC);

// sprite vertices/texture coordinates
void func_800276AC(Vtx vtxs[], u16 width, u16 height, u16 textureSize, u16 arg4, u16 arg5, u16 arg6, u16 arg7, s16 arg8, u16 arg9, u8 r, u8 g, u8 b, u8 a) {
    
    u8 coordinate1;
    u8 coordinate2;
    u8 coordinate3;
    
    u16 temp;
    u16 temp2;
    u16 temp3;
    u16 temp4;

    switch ((arg9 >> 7) & 3) {
        case 2:
            coordinate1 = 0;
            coordinate2 = 1;
            coordinate3 = 2;
            break;

        case 3:
            coordinate1 = 0;
            coordinate2 = 2;
            coordinate3 = 1;
            break;

        case 0:
        case 1:
        default:
            break;
    }

    temp = height >> 1;
    temp3 = width >> 1;
    
    if (arg5) {

        vtxs[0].v.tc[0] = width << 6;
        vtxs[3].v.tc[0] = width << 6;
        
        vtxs[1].v.tc[0] = 0;
        vtxs[2].v.tc[0] = 0;
        
        vtxs[0].v.ob[coordinate1] = -temp3 - arg7;
        vtxs[3].v.ob[coordinate1] = -temp3 - arg7;
        
        vtxs[2].v.ob[coordinate1] = vtxs[1].v.ob[coordinate1] = vtxs[0].v.ob[coordinate1] + width;
        
    } else {
        
        vtxs[1].v.tc[0] = width << 6;
        vtxs[2].v.tc[0] = width << 6;
        
        vtxs[0].v.tc[0] = 0;
        vtxs[3].v.tc[0] = 0;
        
        vtxs[0].v.ob[coordinate1] = arg7 - temp3;
        vtxs[3].v.ob[coordinate1] = arg7 - temp3;
        
        vtxs[2].v.ob[coordinate1] = vtxs[1].v.ob[coordinate1] = vtxs[0].v.ob[coordinate1] + width;  
        
    }
    
    vtxs[2].v.tc[1] = textureSize << 6;
    vtxs[3].v.tc[1] = textureSize << 6;
    
    vtxs[0].v.tc[1] = 0;
    vtxs[1].v.tc[1] = 0;
    
    switch ((arg9 >> 5) & 3) {
        case 2:
            vtxs[0].v.ob[coordinate2] = (temp - arg4) - arg8;
            vtxs[1].v.ob[coordinate2] = (temp - arg4) - arg8;
            vtxs[3].v.ob[coordinate2] = vtxs[2].v.ob[coordinate2] = temp2 = vtxs[0].v.ob[coordinate2] - textureSize;
            break;

        case 1:
            vtxs[0].v.ob[coordinate2] = arg4 - arg8;
            vtxs[1].v.ob[coordinate2] = arg4 - arg8;
            vtxs[3].v.ob[coordinate2] = vtxs[2].v.ob[coordinate2] = temp4 = vtxs[0].v.ob[coordinate2] - textureSize;
            break;

        case 3:
            vtxs[0].v.ob[coordinate2] = (height - arg4) - arg8;
            vtxs[1].v.ob[coordinate2] = (height - arg4) - arg8;
            vtxs[3].v.ob[coordinate2] = vtxs[2].v.ob[coordinate2] = temp2 = vtxs[0].v.ob[coordinate2] - textureSize;
            break;

        default:
            break;

    }

    vtxs[0].v.ob[coordinate3] = 0;
    vtxs[1].v.ob[coordinate3] = 0;
    vtxs[2].v.ob[coordinate3] = 0;
    vtxs[3].v.ob[coordinate3] = 0;

    vtxs[0].v.cn[0] = r;
    vtxs[0].v.cn[1] = g;
    vtxs[0].v.cn[2] = b;
    vtxs[0].v.cn[3] = a;
    
    vtxs[1].v.cn[0] = r;
    vtxs[1].v.cn[1] = g;
    vtxs[1].v.cn[2] = b;
    vtxs[1].v.cn[3] = a;

    vtxs[2].v.cn[0] = r;
    vtxs[2].v.cn[1] = g;
    vtxs[2].v.cn[2] = b;
    vtxs[2].v.cn[3] = a;
    
    vtxs[3].v.cn[0] = r;
    vtxs[3].v.cn[1] = g;
    vtxs[3].v.cn[2] = b;
    vtxs[3].v.cn[3] = a;
    
}

//INCLUDE_ASM(const s32, "system/graphic", sinfRadians);

f32 sinfRadians(f32 angle) {
    return sinf(angle * DEGREES_TO_RADIANS_CONSTANT);
}

//INCLUDE_ASM(const s32, "system/graphic", cosfRadians);

f32 cosfRadians(f32 angle) {
    return cosf(angle * DEGREES_TO_RADIANS_CONSTANT);
}

//INCLUDE_ASM(const s32, "system/graphic", func_80027950);

// 3D rotation
// sprites
void func_80027950(Vec3f arg0, Vec3f* arg1, Vec3f arg2) {
    
    f32 sinX;
    f32 cosX;
    f32 sinY;
    f32 cosY;
    f32 sinZ;
    f32 cosZ;

    f32 tempX;
    f32 tempY;
    f32 tempZ;
    
    f32 tempRadiansX = arg2.x * DEGREES_TO_RADIANS_CONSTANT;
    f32 tempRadiansY = arg2.y * DEGREES_TO_RADIANS_CONSTANT;
    f32 tempRadiansZ = arg2.z * DEGREES_TO_RADIANS_CONSTANT;
    
    sinX = sinf(tempRadiansX);
    cosX = cosf(tempRadiansX);
    sinY = sinf(tempRadiansY);
    cosY = cosf(tempRadiansY);
    sinZ = sinf(tempRadiansZ);
    cosZ = cosf(tempRadiansZ);
    
    arg1->x = arg0.x;
    arg1->y = arg0.y;
    arg1->z = arg0.z;
    
    tempX = arg1->x;
    tempY = arg1->y;
    tempZ = arg1->z;
    
    if (tempRadiansZ != 0.0f) {
        
        arg1->x = (-tempY * sinZ) + (tempX * cosZ);
        arg1->y = (tempY * sinZ) + (tempX * sinZ);
        arg1->z = tempZ;

    }
    
    tempX = arg1->x;
    tempY = arg1->y;
    tempZ = arg1->z;
    
    if (tempRadiansY != 0.0f) {
        
        arg1->x = (tempZ * sinY) + (tempX * cosY);
        arg1->y = tempY;
        arg1->z = (tempZ * cosY) - (tempX * sinY);
   
    }
    
    tempX = arg1->x;
    tempY = arg1->y;
    tempZ = arg1->z;
    
    if (tempRadiansX != 0.0f) {
    
        arg1->x = tempX;
        arg1->y = (-tempZ * sinX) + (tempY * cosX);
        arg1->z = (tempZ * cosX) + (tempY * sinX);
        
    }
    
}

//INCLUDE_ASM(const s32, "system/graphic", func_80027B74);

// unused or inline
void func_80027B74(Vec3f arg0, Vec3f* arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6) {
    
    f32 temp;
    
    f32 tempX;
    f32 tempY;
    f32 tempZ;
    
    tempX = (arg0.z * arg3) + (arg0.x * arg6);
    
    temp = (arg0.z * arg6) - (arg0.x * arg3);
    
    tempY = (-temp * arg2) + (arg0.y * arg5);
    tempZ = (temp * arg5) + (arg0.y * arg2);
    
    arg1->x = tempX;
    arg1->y = tempY;
    arg1->z = tempZ;
    
}

//INCLUDE_ASM(const s32, "system/graphic", func_80027BFC);

// only used by map
Coordinates* func_80027BFC(Coordinates* arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9) {
    
    Coordinates vec;
    f32 temp_f12;
    f32 temp_f14;
    f32 temp_f20;
    f32 temp_f20_2;
    f32 temp_f22;
    f32 temp_f22_2;
    f32 temp_f24;
    f32 temp_f24_2;
    f32 var_f0;
    
    temp_f14 = (arg2 * (arg6 - arg9)) + (arg5 * (arg9 - arg3));
    temp_f24 = temp_f14 + (arg8 * (arg3 - arg6));
    temp_f22_2 = temp_f24;
    
    temp_f22 = ((arg3 * (arg4 - arg7)) + (arg6 * (arg7 - arg1))) + (arg9 * (arg1 - arg4));
    temp_f20 = ((arg1 * (arg5 - arg8)) + (arg4 * (arg8 - arg2))) + (arg7 * (arg2 - arg5));
    temp_f12 = ((temp_f24 * temp_f24) + (temp_f22 * temp_f22)) + (temp_f20 * temp_f20);
    
    var_f0 = sqrtf(temp_f12);
    
    if (var_f0 == 0) {
        
        vec.x = 0.0f;
        vec.y = 0.0f;
        vec.z = 0.0f;
        vec.w = 0.0f;
        
    } else {
        
        temp_f22_2 /= var_f0;
        temp_f22 /= var_f0;
        temp_f20 /= var_f0;
        
        vec.x = temp_f22_2;
        vec.y = temp_f22;
        vec.z = temp_f20;
        vec.w = -(((temp_f22_2 * arg1) + (temp_f22 * arg2)) + (temp_f20 * arg3));

    }
    
    *arg0 = vec;
    return arg0;

}

// need different return type to match func_80027E10
static inline Coordinates func_80027BFC_static_inline(Coordinates* arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9) {

    Coordinates vec;

    f32 temp_f12;
    f32 temp_f14;
    f32 temp_f20;
    f32 temp_f20_2;
    f32 temp_f22;
    f32 temp_f22_2;
    f32 temp_f24;
    f32 temp_f24_2;
    f32 var_f0;

    temp_f14 = (arg2 * (arg6 - arg9)) + (arg5 * (arg9 - arg3));
    temp_f24 = temp_f14 + (arg8 * (arg3 - arg6));
    temp_f22_2 = temp_f24;

    temp_f22 = ((arg3 * (arg4 - arg7)) + (arg6 * (arg7 - arg1))) + (arg9 * (arg1 - arg4));
    temp_f20 = ((arg1 * (arg5 - arg8)) + (arg4 * (arg8 - arg2))) + (arg7 * (arg2 - arg5));
    temp_f12 = ((temp_f24 * temp_f24) + (temp_f22 * temp_f22)) + (temp_f20 * temp_f20);

    var_f0 = sqrtf(temp_f12);

    if (var_f0 == 0) {

        vec.x = 0.0f;
        vec.y = 0.0f;
        vec.z = 0.0f;
        vec.w = 0.0f;

    } else {

        temp_f22_2 /= var_f0;
        temp_f22 /= var_f0;
        temp_f20 /= var_f0;

        vec.x = temp_f22_2;
        vec.y = temp_f22;
        vec.z = temp_f20;
        vec.w = -(((temp_f22_2 * arg1) + (temp_f22 * arg2)) + (temp_f20 * arg3));

    }

    return vec;
    
}

//INCLUDE_ASM(const s32, "system/graphic", func_80027DC0);

// only used by map
f32 func_80027DC0(f32 arg0, f32 arg1, Coordinates vec) {

    f32 result;
    
    result = 0.0f;
    
    if (vec.y != 0.0f) {
        result = ( (-(vec.x*arg0) - (vec.z * arg1)) - vec.w) / vec.y;
    }
    

    return result;
}

//INCLUDE_ASM(const s32, "system/graphic", func_80027E10);

// only used by map
u8 func_80027E10(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, f32 argA, f32 argB) {

    Coordinates coordinates;
    
    u8 count = 0;
    
    coordinates = func_80027BFC_static_inline(&coordinates, arg3, arg4, arg5, arg6, arg7, arg8, arg0, arg1, arg2);
    
    if (coordinates.y >= 0.0f) {
        count++;
    }
    
    coordinates = func_80027BFC_static_inline(&coordinates, arg6, arg7, arg8, arg9, argA, argB, arg0, arg1, arg2);

    if (coordinates.y >= 0.0f) {
        count++;
    }
    
    coordinates = func_80027BFC_static_inline(&coordinates, arg9, argA, argB, arg3, arg4, arg5, arg0, arg1, arg2);
    
    if (coordinates.y >= 0.0f) {
        count++;
    }
    
    count /= 3;
    
    return count;
    
}

//INCLUDE_ASM(const s32, "system/graphic", func_800284E8);

// dot product?
f32 func_800284E8(f32 arg0, f32 arg1, f32 arg2, Coordinates coordinates) {
    
    f32 x = coordinates.x * arg0;
    f32 y = coordinates.y * arg1;
    f32 z = coordinates.z * arg2;
    f32 w = coordinates.w * 1;
  
    return x + y + z + w;
    
}

// alternate
/*
f32 func_800284E8(f32 arg0, f32 arg1, f32 arg2, Coordinates coordinates) {
    f32 temp = coordinates.x * arg0;
  
    return (temp) + (coordinates.y * arg1) + (coordinates.z * arg2) + coordinates.w;    
}
*/

//INCLUDE_RODATA(const s32, "system/graphic", directionsToYValues);

static const f32 directionsToYValues[8] = { 0.0f, 315.0f, 270.0f, 225.0f, 180.0f, 135.0f, 90.0f, 45.0f };

//INCLUDE_ASM(const s32, "system/graphic", func_80028520);

Vec3f* func_80028520(Vec3f *arg0, f32 arg1, u8 arg2, f32 arg3) {

    f32 buffer[8];

    f32 sinX;
    f32 cosX;
    f32 sinY;
    f32 cosY;
    f32 sinZ;
    f32 cosZ;

    f32 temp1;
    f32 temp2;
    f32 temp3;
    
    Vec3f vec1;
    Vec3f vec2;
    Vec3f vec3;
    Vec3f vec4;
    Vec3f vec5;
    
    f32 x;
    f32 y;
    f32 z;

    memcpy(buffer, directionsToYValues, 32);
    
    if (arg2 != 0xFF) {

        vec1.x = 0.0f;
        vec1.y = arg3;
        vec1.z = arg1;

        vec3.x = 0.0f;
        vec3.y = buffer[arg2];
        vec3.z = 0.0f;

        vec4 = vec1;
        vec5 = vec3;

        x = vec5.x * DEGREES_TO_RADIANS_CONSTANT;
        y = vec5.y * DEGREES_TO_RADIANS_CONSTANT;
        z = vec5.z * DEGREES_TO_RADIANS_CONSTANT;

        sinX = sinf(x);
        cosX = cosf(x);

        sinY = sinf(y);
        cosY = cosf(y);

        sinZ = sinf(z);
        cosZ = cosf(z);

        vec2.x = vec4.x;
        vec2.y = vec4.y;        
        vec2.z = vec4.z;
        
        temp1 = vec4.x;
        temp2 = vec4.y;
        temp3 = vec4.z;

        if (z != 0.0f) {
            vec2.x = (-temp2 * sinZ) + (temp1 * cosZ);
            vec2.y = (temp2 * sinZ) + (temp1 * sinZ);
            vec2.z = temp3;
        }

        temp1 = vec2.x;
        temp2 = vec2.y;
        temp3 = vec2.z;
        
        if (y != 0.0f) {
            vec2.x = (temp3 * sinY) + (temp1 * cosY);
            vec2.y = temp2;
            vec2.z = (temp3 * cosY) - (temp1 * sinY);
        }

        temp1 = vec2.x;
        temp2 = vec2.y;
        temp3 = vec2.z;
        
        if (x != 0.0f) {
            vec2.x = temp1;
            vec2.y = (-temp3 * sinX) + (temp2 * cosX);
            vec2.z = (temp3 * cosX) + (temp2 * sinX);
        }
        
    } else {
        vec2.x = 0.0f;
        vec2.y = arg3;
        vec2.z = 0.0f;
    }

    *arg0 = vec2;
    
    return arg0;
    
}

//INCLUDE_ASM(const s32, "system/graphic", getSpriteYValueFromDirection);

// get y value for sprite based on direction
f32 getSpriteYValueFromDirection(u8 direction) {

    f32 buffer[8];

    memcpy(buffer, directionsToYValues, 32);
    
    return buffer[direction];

}
 
//INCLUDE_ASM(const s32, "system/graphic", func_80028888);

// get ptr to ci texture from index
u8* func_80028888(u16 spriteIndex, u32* textureIndex) {
    return (u8*)textureIndex + textureIndex[spriteIndex];
}

// alternate
/*
void *func_80028888(u16 arg0, u32 *arg1) {
  void *res = arg1;
  return res + *(arg1 + arg0);
}
*/

//INCLUDE_ASM(const s32, "system/graphic", func_800288A0);

// get ptr to palette
// FIXME: should return u16*?
u8 *func_800288A0(u16 index, u32 *paletteIndex) {
  return (u8*)paletteIndex + paletteIndex[index];
}

//INCLUDE_ASM(const s32, "system/graphic", func_800288B8);

// returns palette pointer from sprite-to-palette mapping table
u8* func_800288B8(u16 spriteIndex, u32* paletteIndex, u8* spriteToPaletteIndex) {
    
    u8* arr = spriteToPaletteIndex + 4;
    u16 i = arr[spriteIndex];
    
    return (u8*)paletteIndex + paletteIndex[i]; 

}

// alternate
/*
u8* func_800288B8(u16 arg0, u32 *arg1, u8 *arg2) {
    return (u8*)arg1 + *(arg1 + *(arg0 + arg2 + 4));
}
*/

//INCLUDE_ASM(const s32, "system/graphic", initRcp);

Gfx* initRcp(Gfx* dl) {
    
    gSPSegment(dl++, 0, 0x0);
    gSPDisplayList(dl++, OS_K0_TO_PHYSICAL(setup_rspstate));
    gSPDisplayList(dl++, OS_K0_TO_PHYSICAL(setup_rdpstate));

    return dl++;

}

//INCLUDE_ASM(const s32, "system/graphic", clearFramebuffer);

Gfx* clearFramebuffer(Gfx* dl) {

    gDPSetDepthImage(dl++, OS_K0_TO_PHYSICAL(nuGfxZBuffer));
    gDPPipeSync(dl++);
    gDPSetCycleType(dl++, G_CYC_FILL);
    gDPSetColorImage(dl++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, OS_K0_TO_PHYSICAL(nuGfxZBuffer));
    gDPSetFillColor(dl++,(GPACK_ZDZ(G_MAXFBZ,0) << 16 | GPACK_ZDZ(G_MAXFBZ,0)));
    gDPFillRectangle(dl++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
    gDPPipeSync(dl++);
    
    gDPSetColorImage(dl++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, osVirtualToPhysical(nuGfxCfb_ptr));   
    gDPSetFillColor(dl++, (GPACK_RGBA5551(0, 0, 0, 1) << 16 | GPACK_RGBA5551(0, 0, 0, 1)));
    gDPFillRectangle(dl++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
    gDPPipeSync(dl++);
    
    return dl++;

}

//INCLUDE_ASM(const s32, "system/graphic", func_80028A64);

Gfx* func_80028A64(Gfx* dl, Camera* camera, WorldMatrices* matrices) {

    u16 perspNorm;

    switch (camera->perspectiveMode) {
        case 0:
            guOrtho(&matrices->projection, camera->l, camera->r, camera->t, camera->b, camera->n, camera->f, 0.9999f);
            break;
        case 1:
            guPerspective(&matrices->projection, &perspNorm, camera->fov, camera->aspect, camera->near, camera->far, 1.0f);
            gSPPerspNormalize(dl++, perspNorm);
            break;
        default:
            break;
    }

    guLookAt(&matrices->viewing, camera->eye.x, camera->eye.y, camera->eye.z, camera->at.x, camera->at.y, camera->at.z, camera->up.x, camera->up.y, camera->up.z);
    gSPLookAt(dl++, &gSPLookAtBufferA);

    gSPMatrix(dl++, &matrices->projection, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(dl++, &matrices->viewing, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);

    return dl++;

}

//INCLUDE_ASM(const s32, "system/graphic", func_80028C00);

// unused
Gfx* func_80028C00(Gfx* dl, Camera* camera) {
 
    u16 perspNorm;
    
    switch (camera->perspectiveMode) {
        case 0:
            guOrtho(&camera->projection, camera->l, camera->r, camera->t, camera->b, camera->n, camera->f, 1.0f);
            break;
        case 1:
            guPerspective(&camera->projection, &perspNorm, camera->fov, camera->aspect, camera->near, camera->far, 1.0f);
            gSPPerspNormalize(dl++, perspNorm);
            break;
        default:
            break;
    }

    guLookAt(&camera->viewing, camera->eye.x, camera->eye.y, camera->eye.z, camera->at.x, camera->at.y, camera->at.z, camera->up.x, camera->up.y, camera->up.z);
    gSPLookAt(dl++, &gSPLookAtBufferB);

    gSPMatrix(dl++, &camera->projection, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(dl++, &camera->viewing, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);

    return dl++;

}

//INCLUDE_ASM(const s32, "system/graphic", setCameraOrthographicValues);

void setCameraOrthographicValues(Camera* camera, f32 l, f32 r, f32 t, f32 b, f32 n, f32 f) {
    
    camera->l = l;
    camera->r = r;
    camera->t = t;
    camera->b = b;
    camera->n = n;
    camera->f = f;

}

//INCLUDE_ASM(const s32, "system/graphic", setCameraPerspectiveValues);

void setCameraPerspectiveValues(Camera* camera, f32 fov, f32 aspect, f32 near, f32 far) {

    camera->fov = fov;
    camera->aspect = aspect;
    camera->near = near;
    camera->far = far;

}

//INCLUDE_ASM(const s32, "system/graphic", setCameraLookAt);

void setCameraLookAt(Camera* camera, f32 xEye, f32 yEye, f32 zEye, f32 atX, f32 atY, f32 atZ, f32 upX, f32 upY, f32 upZ) {

    camera->eye.x = xEye;
    camera->eye.y = yEye;
    camera->eye.z = zEye;

    camera->at.x = atX;
    camera->at.y = atY;
    camera->at.z = atZ;
    
    camera->up.x = upX;
    camera->up.y = upY;
    camera->up.z = upZ;

}

//INCLUDE_ASM(const s32, "system/graphic", func_80028E14);

// unused
inline Gfx *func_80028E14(Gfx* dl, u8 a, u8 r, u8 g, u8 b) {

    gSPLightColor(dl++, LIGHT_2, (r << 0x18) + (g << 0x10) + (b << 8));

    return dl++;

}

//INCLUDE_ASM(const s32, "system/graphic", func_80028E60);

// unused
inline Gfx *func_80028E60(Gfx* dl, u8 a, u8 r, u8 g, u8 b) {

    gSPLightColor(dl++, LIGHT_1, (r << 0x18) + (g << 0x10) + (b << 8));

    return dl++;

}

//INCLUDE_ASM(const s32, "system/graphic", func_80028EA8);

// unused
void func_80028EA8(UnknownGraphicsStruct* arg0, u8 arg1, u8 arg2, u8 arg3) {
    arg0->unk_10 = arg1;
    arg0->unk_11 = arg2;
    arg0->unk_12 = arg3;
}

//INCLUDE_ASM(const s32, "system/graphic", setInitialWorldRotationAngles);

void setInitialWorldRotationAngles(f32 x, f32 y, f32 z) {
    
    currentWorldRotationAngles.x = x;
    currentWorldRotationAngles.y = y;

    previousWorldRotationAngles.x = x;
    previousWorldRotationAngles.y = y;

    currentWorldRotationAngles.z = z;
    previousWorldRotationAngles.z = z;

}

//INCLUDE_ASM(const s32, "system/graphic", func_80028EF8);

// mapContext.c --> increments/decrements 6 by 1
void func_80028EF8(f32 x, f32 y, f32 z) {
    currentWorldRotationAngles.x += x;
    currentWorldRotationAngles.y += y;
    currentWorldRotationAngles.z += z;
}

//INCLUDE_ASM(const s32, "system/graphic", nuGfxInit);

void nuGfxInit(void) {

    Gfx	gfxList[0x100];
    Gfx* gfxList_ptr;
    
    nuGfxThreadStart();
    nuGfxSetCfb(FrameBuf, 3);

    nuGfxSetZBuffer((u16*) NU_GFX_ZBUFFER_ADDR);
    
    nuGfxSwapCfbFuncSet(nuGfxSwapCfb);
    
    // macro isn't working yet
    // nuGfxSetUcode(nugfx_ucode);
    nuGfxUcode = &nugfx_ucode;

    nuGfxTaskMgrInit();
    
    gfxList_ptr = gfxList;
    
    gSPDisplayList(gfxList_ptr++, OS_K0_TO_PHYSICAL(rdpstateinit_dl));
    gDPFullSync(gfxList_ptr++);
    gSPEndDisplayList(gfxList_ptr++);
    
    nuGfxTaskStart(gfxList, (s32)(gfxList_ptr - gfxList) * sizeof(Gfx), NU_GFX_UCODE_F3DEX, NU_SC_NOSWAPBUFFER);

    nuGfxTaskAllEndWait();
    
}
