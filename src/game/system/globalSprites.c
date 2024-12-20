#include "common.h"

#include "system/sprite.h"
#include "system/mathUtils.h"

// forward declarations
bool func_8002B8E0(u16, u8, void*);
void func_8002CC84(SpriteAnimation*, u32*);             
u8* func_8002CD34(u16 arg0, void* arg1);
u16* func_8002CD4C(u16, u16*);   

// bss
extern HMSprite globalSprites[MAX_ACTIVE_SPRITES];

//INCLUDE_ASM(const s32, "game/system/globalSprites", initializeGlobalSprites);

void initializeGlobalSprites(void) {

    u16 i;

    for (i = 0; i < MAX_SPRITES; i++) {
        
        globalSprites[i].flags2 = 0;
        globalSprites[i].flags1 = 0;
        
        globalSprites[i].paletteIndex = 0;
        globalSprites[i].unk_90 = 0;
        globalSprites[i].unk_92 = 0;
        globalSprites[i].unk_91 = 0;
        
        globalSprites[i].shrink.x = 0;
        globalSprites[i].shrink.y = 0;
        globalSprites[i].shrink.z = 0;
        
        globalSprites[i].scale.x = 1.0f;
        globalSprites[i].scale.y = 1.0f;
        globalSprites[i].scale.z = 1.0f;

        globalSprites[i].angles.x = 0;
        globalSprites[i].angles.y = 0;
        globalSprites[i].angles.z = 0;
        
        globalSprites[i].rgbaCurrent.r = 0;
        globalSprites[i].rgbaCurrent.g = 0;
        globalSprites[i].rgbaCurrent.b = 0;
        globalSprites[i].rgbaCurrent.a = 0;
        
        globalSprites[i].rgbaDefault.r = 0;
        globalSprites[i].rgbaDefault.g = 0;
        globalSprites[i].rgbaDefault.b = 0;
        globalSprites[i].rgbaDefault.a = 0;

    }
}

//INCLUDE_ASM(const s32, "game/system/globalSprites", dmaSprite);

bool dmaSprite(u16 index, u32 romTextureStart, u32 romTextureEnd, u32 romAssetIndexStart, u32 romAssetIndexEnd, 
    u32 romSpritesheetIndexStart, u32 romSpritesheetIndexEnd, u8* texture1Vaddr, u8* texture2Vaddr, u16* paletteVaddr, u16* animationVaddr, 
    u8* spriteToPaletteVaddr, u32* spritesheetIndexVaddr, u8 assetType, u8 flag) {

    u32 assetIndex[8];
    bool result = FALSE;
    
    u32 offset1;
    u32 offset2;
    u32 offset3;
    u32 offset4;
    u32 offset5;

    if (index < MAX_ACTIVE_SPRITES) {
        
        if (!(globalSprites[index].flags2 & ACTIVE)) { 
            
            nuPiReadRom(romAssetIndexStart, assetIndex, romAssetIndexEnd - romAssetIndexStart);

            // spritesheet
            offset1 = assetIndex[0];
            // palette
            offset2 = assetIndex[1];
            // animation
            offset3 = assetIndex[2];
            // sprite to palette
            offset4 = assetIndex[3];
            // end
            offset5 = assetIndex[4];
        
            // has separate spritesheet index
            if (assetType & 1) {

                nuPiReadRom(romTextureStart + offset2, paletteVaddr, offset3 - offset2);
                nuPiReadRom(romTextureStart + offset3, animationVaddr, offset4 - offset3);
                nuPiReadRom(romTextureStart + offset4, spriteToPaletteVaddr, offset5 - offset4);
                nuPiReadRom(romSpritesheetIndexStart, spritesheetIndexVaddr, romSpritesheetIndexEnd - romSpritesheetIndexStart);

                func_8002B50C(index, animationVaddr, spritesheetIndexVaddr, paletteVaddr, spriteToPaletteVaddr, romTextureStart, texture1Vaddr, texture2Vaddr);

            // spritesheet index in spritesheet
            } else {

                nuPiReadRom(romTextureStart + offset1, texture1Vaddr, offset2 - offset1);
                nuPiReadRom(romTextureStart + offset2, paletteVaddr, offset3 - offset2);
                nuPiReadRom(romTextureStart + offset3, animationVaddr, offset4 - offset3);
                nuPiReadRom(romTextureStart + offset4, spriteToPaletteVaddr, offset5 - offset4);
                
                func_8002B36C(index, animationVaddr, texture1Vaddr, paletteVaddr, spriteToPaletteVaddr);

            }

            if (!flag) {
                globalSprites[index].flags2 |= 0x200;
            }

            globalSprites[index].flags2 |= 0x40;

            result = TRUE;

        }
    
    }

    return result;

}

//INCLUDE_ASM(const s32, "game/system/globalSprites", func_8002B36C);

bool func_8002B36C(u16 index, u32* unknownAssetIndexPtr, u32* spritesheetIndexPtr, u32* paletteIndexPtr, u8* spriteToPaletteMappingPtr) {

    bool result = FALSE;

    if (index < MAX_ACTIVE_SPRITES) {

        if (!(globalSprites[index].flags2 & ACTIVE)) {

            globalSprites[index].unknownAssetIndexPtr = unknownAssetIndexPtr;
            globalSprites[index].spritesheetIndexPtr = spritesheetIndexPtr;
            globalSprites[index].paletteIndexPtr = paletteIndexPtr;
            globalSprites[index].spriteToPaletteMappingPtr = spriteToPaletteMappingPtr;
            globalSprites[index].texturePtr = (void*)0;
            globalSprites[index].texture2Ptr = (void*)0;
            globalSprites[index].romTexturePtr = (void*)0;

            globalSprites[index].flags2 = 1;
            globalSprites[index].flags1 = 0;

            globalSprites[index].paletteIndex = 0;
            globalSprites[index].unk_92 = 0;

            setSpriteShrinkFactor(index, 0.0f, 0.0f, 0.0f);
            setSpriteScale(index, 1.0f, 1.0f, 1.0f);
            func_8002BE14(index, 0.0f, 0.0f, 0.0f);
            setSpriteDefaultRGBA(index, 0xFF, 0xFF, 0xFF, 0xFF);
            func_8002C680(index, 2, 2);
            func_8002C6F8(index, 2);
            func_8002C7EC(index, 3);
            
            result = TRUE;
            
        }
    }
 
    return result;

}

//INCLUDE_ASM(const s32, "game/system/globalSprites", func_8002B50C);

bool func_8002B50C(u16 spriteIndex, u32* unknownAssetIndexPtr, u32* spritesheetIndexPtr, u32* paletteIndexPtr, u8* spriteToPaletteMappingPtr, u32 romTexturePtr, u8* texturePtr, u8* texture2Ptr) {
    
    bool result = FALSE;

    if (spriteIndex < MAX_ACTIVE_SPRITES) {

        if (!(globalSprites[spriteIndex].flags2 & ACTIVE)) {

            // animation index
            globalSprites[spriteIndex].unknownAssetIndexPtr = unknownAssetIndexPtr;
            globalSprites[spriteIndex].spritesheetIndexPtr = spritesheetIndexPtr;
            globalSprites[spriteIndex].paletteIndexPtr = paletteIndexPtr;
            globalSprites[spriteIndex].spriteToPaletteMappingPtr = spriteToPaletteMappingPtr;
            globalSprites[spriteIndex].texturePtr = texturePtr;
            globalSprites[spriteIndex].texture2Ptr = texture2Ptr;
            globalSprites[spriteIndex].romTexturePtr = romTexturePtr;

            globalSprites[spriteIndex].flags2 = 5;
            globalSprites[spriteIndex].flags1 = 0;

            globalSprites[spriteIndex].paletteIndex = 0;
            globalSprites[spriteIndex].unk_92 = 0;

            setSpriteShrinkFactor(spriteIndex, 0, 0, 0);
            setSpriteScale(spriteIndex, 1.0f, 1.0f, 1.0f);
            func_8002BE14(spriteIndex, 0, 0, 0);
            setSpriteDefaultRGBA(spriteIndex, 0xFF, 0xFF, 0xFF, 0xFF);
            func_8002C680(spriteIndex, 2, 2);
            func_8002C6F8(spriteIndex, 2);
            func_8002C7EC(spriteIndex, 3);
            
            result = TRUE;
            
        }
    }
 
    return result;
    
}

//INCLUDE_ASM(const s32, "game/system/globalSprites", deactivateSprite);

bool deactivateSprite(u16 spriteIndex) {

    bool result = FALSE;

    if (spriteIndex < MAX_ACTIVE_SPRITES) {
        if (globalSprites[spriteIndex].flags2 & ACTIVE) {
            globalSprites[spriteIndex].flags2 &= ~(ACTIVE | 2);
            result = TRUE;
        }        
    }

    return result; 
}

//INCLUDE_ASM(const s32, "game/system/globalSprites", deactivateGlobalSprites);

void deactivateGlobalSprites(void) {

    u16 i;

    for (i = 0; i < MAX_ACTIVE_SPRITES; i++) {

            if ((globalSprites[i].flags2 & ACTIVE) && (globalSprites[i].flags2 & 2)) {

                // FIXME: probably inline
                if (i < MAX_ACTIVE_SPRITES) {
                    
                    globalSprites[i].flags2 &= ~(ACTIVE | 2);

                }

            }

            globalSprites[i].flags2 = 0;
        
    }
}

//INCLUDE_ASM(const s32, "game/system/globalSprites", func_8002B7A0);

// possibly inline, otherwise unused
bool func_8002B7A0(u16 spriteIndex, u8 arg1) {

    bool result;

    result = func_8002B8E0(spriteIndex, arg1, globalSprites[spriteIndex].unknownAssetIndexPtr);

    globalSprites[spriteIndex].animationCounter1 = 0xFF;
    globalSprites[spriteIndex].animationCounter2 = 0xFF;

    return result;
    
}

//INCLUDE_ASM(const s32, "game/system/globalSprites", func_8002B80C);

bool func_8002B80C(u16 spriteIndex, u16 offset, u8 arg2) {

    bool result = FALSE;
    
    if (spriteIndex < MAX_ACTIVE_SPRITES) {

        if (globalSprites[spriteIndex].flags2 & ACTIVE) {
            
            if (globalSprites[spriteIndex].unknownAssetIndexPtr[offset] != globalSprites[spriteIndex].unknownAssetIndexPtr[offset+1]) {

                // func_8002CD34(offset, globalSprites[spriteIndex].unknownAssetIndexPtr) = get address + offset for unknown asset ptr based on sprite index
                result = func_8002B8E0(spriteIndex, arg2, func_8002CD34(offset, globalSprites[spriteIndex].unknownAssetIndexPtr));
                
                globalSprites[spriteIndex].animationCounter1 = 0xFF;
                globalSprites[spriteIndex].animationCounter2 = 0xFF;
           
            }
        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "game/system/globalSprites", func_8002B8E0);

bool func_8002B8E0(u16 spriteIndex, u8 arg1, void* arg2) {

    bool result = FALSE;
    u16 *temp;
    
    if ((spriteIndex < MAX_ACTIVE_SPRITES) && (globalSprites[spriteIndex].flags2 & ACTIVE) && !(globalSprites[spriteIndex].flags2 & 2)) {

        globalSprites[spriteIndex].unknownAssetPtr = arg2;

        globalSprites[spriteIndex].unk_91 = 0;
        globalSprites[spriteIndex].unk_92 = 0;

        globalSprites[spriteIndex].flags2 &= ~0x40;
        globalSprites[spriteIndex].flags2 |= 2;

        switch (arg1) {
            case 0xFF:
                globalSprites[spriteIndex].unk_90 = 0;
                globalSprites[spriteIndex].flags2 &= ~0x10;
                globalSprites[spriteIndex].flags2 |= 8;
                break;
            case 0xFE:
                globalSprites[spriteIndex].unk_90 = 0;
                globalSprites[spriteIndex].flags2 |= 8 | 0x10;
                break;
            case 0xFD:
                globalSprites[spriteIndex].unk_90 = 0;
                globalSprites[spriteIndex].flags2 |= 8 | 0x800;
                break;
            default:
                globalSprites[spriteIndex].unk_90 = arg1;
                globalSprites[spriteIndex].flags2 &= ~(8 | 0x10);
                break;
        }

        // update animation from byteswapped table
        func_8002CC84(&globalSprites[spriteIndex].animation, globalSprites[spriteIndex].unknownAssetPtr);

        temp = func_8002CD4C(globalSprites[spriteIndex].unk_90, globalSprites[spriteIndex].unknownAssetPtr+8);
        
        globalSprites[spriteIndex].unknownAsset3Ptr = temp;
        globalSprites[spriteIndex].unknownAsset4Ptr = temp + 2;

        result = TRUE;
        
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "game/system/globalSprites", func_8002BAD8);

bool func_8002BAD8(u16 index) {

    bool result = FALSE;

    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].flags2 & ACTIVE) {
            globalSprites[index].flags2 &= ~2;
            result = TRUE;
        }
    }
    
    return result;

}

//INCLUDE_ASM(const s32, "game/system/globalSprites", func_8002BB30);

bool func_8002BB30(u16 index) {

    bool result = FALSE;

    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].flags2 & ACTIVE) {
            globalSprites[index].flags2 &= ~0x20;
            result = TRUE;
        }
    }
    
    return result;

}

//INCLUDE_ASM(const s32, "game/system/globalSprites", func_8002BB88);

bool func_8002BB88(u16 index) {

    bool result = FALSE;

    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].flags2 & ACTIVE) {
            globalSprites[index].flags2 |= 0x20;
            result = TRUE;
        }
    }
    
    return result;

}

//INCLUDE_ASM(const s32, "game/system/globalSprites", func_8002BBE0);

bool func_8002BBE0(u16 index, u8 arg1, u8 arg2) {

    bool result = FALSE;
    
    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].flags2 & ACTIVE) {

            if (arg1) {
                globalSprites[index].flags1 |= ACTIVE;
            } else {
                globalSprites[index].flags1 &= ~ACTIVE;
            }

            if (arg2) {
                globalSprites[index].flags1 |= 2;
            } else {
                globalSprites[index].flags1 &= ~2;
            }
            
            result = TRUE;

        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "game/system/globalSprites", func_8002BCC8);

bool func_8002BCC8(u16 index) {

    bool result = FALSE;

    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].flags2 & ACTIVE) {
            result = globalSprites[index].flags2 & 0x40;
        }
    }
    
    return result;

}

//INCLUDE_ASM(const s32, "game/system/globalSprites", setSpriteShrinkFactor);

bool setSpriteShrinkFactor(u16 index, f32 x, f32 y, f32 z) {

    bool result = FALSE;

    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].flags2 & ACTIVE) {

            globalSprites[index].shrink.x = x;
            globalSprites[index].shrink.y = y;
            globalSprites[index].shrink.z = z;

            result = TRUE;

        }
    }

    return result;

}

//INCLUDE_ASM(const s32, "game/system/globalSprites", setSpriteScale);

bool setSpriteScale(u16 index, f32 x, f32 y, f32 z) {

    bool result = FALSE;

    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].flags2 & ACTIVE) {

            globalSprites[index].scale.x = x;
            globalSprites[index].scale.y = y;
            globalSprites[index].scale.z = z;
            
            result = TRUE;

        }
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "game/system/globalSprites", func_8002BE14);

bool func_8002BE14(u16 index, f32 x, f32 y, f32 z) {

    bool result = FALSE;

    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].flags2 & ACTIVE) {

            globalSprites[index].angles.x = x;
            globalSprites[index].angles.y = y;
            globalSprites[index].angles.z = z;

            result = TRUE;

        }
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "game/system/globalSprites", adjustSpriteShrinkFactor);

bool adjustSpriteShrinkFactor(u16 index, f32 x, f32 y, f32 z) {

    bool result = FALSE;
    
    if (index < MAX_ACTIVE_SPRITES) {
        
        if (globalSprites[index].flags2 & ACTIVE) {

            globalSprites[index].shrink.x += x;
            globalSprites[index].shrink.y += y;
            globalSprites[index].shrink.z += z;
                
            result = TRUE;
            
        }   
    }
    
    return result;

}

//INCLUDE_ASM(const s32, "game/system/globalSprites", adjustSpriteScale);

// unused
bool adjustSpriteScale(u16 index, f32 x, f32 y, f32 z) {

    bool result = FALSE;
    
    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].flags2 & ACTIVE) {

            globalSprites[index].scale.x += x;
            globalSprites[index].scale.y += y;
            globalSprites[index].scale.z += z;
            
            result = TRUE;

        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "game/system/globalSprites", func_8002C000);

bool func_8002C000(u16 index, f32 x, f32 y, f32 z) {

    bool result = FALSE;
    
    if (index < MAX_ACTIVE_SPRITES) {
        
        if (globalSprites[index].flags2 & ACTIVE) {

            globalSprites[index].angles.x += x;
            globalSprites[index].angles.y += y;
            globalSprites[index].angles.z += z;
                
            result = TRUE;
            
        }   
    }
    
    return result;

}

//INCLUDE_ASM(const s32, "game/system/globalSprites", func_8002C0B4);

bool func_8002C0B4(u16 index, s8 r, s8 g, s8 b, s8 a) {

    bool result = FALSE;
    
    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].flags2 & ACTIVE) {

            globalSprites[index].rgbaCurrent.r += r;
            globalSprites[index].rgbaCurrent.g += g;
            globalSprites[index].rgbaCurrent.b += b;
            globalSprites[index].rgbaCurrent.a += a;
            
            result = TRUE;
            
        }   
    }
    
    return result;

}

//INCLUDE_ASM(const s32, "game/system/globalSprites", updateSpriteRGBA);

// update sprite rgba
// fade out on screen transitions
bool updateSpriteRGBA(u16 index, u8 r, u8 g, u8 b, u8 a, s16 rate) {

    bool result;
    
    s16 absValueRate = getAbsoluteValue(rate);
    f32 tempFloat;
    
    result = 0;
    
    if (index < MAX_ACTIVE_SPRITES) {
        
        if (globalSprites[index].flags2 & ACTIVE) {
 
            globalSprites[index].rgbaDefault.r = (globalSprites[index].rgba.r * r) / 255.0f;
            globalSprites[index].rgbaDefault.g = (globalSprites[index].rgba.g * g) / 255.0f;
            globalSprites[index].rgbaDefault.b = (globalSprites[index].rgba.b * b) / 255.0f;
            globalSprites[index].rgbaDefault.a = (globalSprites[index].rgba.a * a) / 255.0f;
            
            globalSprites[index].flags2 &= ~0x400;

            if (globalSprites[index].rgbaDefault.r < globalSprites[index].rgbaCurrent.r) {
                tempFloat = globalSprites[index].rgbaCurrent.r - globalSprites[index].rgbaDefault.r;
            } else {
                tempFloat = globalSprites[index].rgbaDefault.r - globalSprites[index].rgbaCurrent.r;
            }

            globalSprites[index].normalized.r = (tempFloat * absValueRate) / globalSprites[index].rgba.r;

            if (globalSprites[index].rgbaDefault.g < globalSprites[index].rgbaCurrent.g) {
                tempFloat = globalSprites[index].rgbaCurrent.g - globalSprites[index].rgbaDefault.g;
            } else {
                tempFloat = globalSprites[index].rgbaDefault.g - globalSprites[index].rgbaCurrent.g;
            }

            globalSprites[index].normalized.g = (tempFloat * absValueRate) / globalSprites[index].rgba.g;

            if (globalSprites[index].rgbaDefault.b < globalSprites[index].rgbaCurrent.b) {
                tempFloat = globalSprites[index].rgbaCurrent.b - globalSprites[index].rgbaDefault.b;
            } else {
                tempFloat = globalSprites[index].rgbaDefault.b - globalSprites[index].rgbaCurrent.b;
            }

            globalSprites[index].normalized.b = (tempFloat * absValueRate) / globalSprites[index].rgba.b;

            if (globalSprites[index].rgbaDefault.a < globalSprites[index].rgbaCurrent.a) {
                tempFloat = globalSprites[index].rgbaCurrent.a - globalSprites[index].rgbaDefault.a;
            } else {
                tempFloat = globalSprites[index].rgbaDefault.a - globalSprites[index].rgbaCurrent.a;
            }

            globalSprites[index].normalized.a = (tempFloat * absValueRate) / globalSprites[index].rgba.a;
            
            result = TRUE;
            
        }
    }
    
    return result;

}

//INCLUDE_ASM(const s32, "game/system/globalSprites", updateSpriteAlpha);

bool updateSpriteAlpha(u16 index, u8 arg1, s16 rate) {

    bool result;

    f32 tempF;
    s16 absValueRate = getAbsoluteValue(rate);

    result = FALSE;

    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].flags2 & ACTIVE) {

            globalSprites[index].rgbaDefault.a = (globalSprites[index].rgba.a * arg1) / 255.0f;

            globalSprites[index].flags2 &= ~0x400;

            if (globalSprites[index].rgbaDefault.a < globalSprites[index].rgbaCurrent.a) {
                tempF = globalSprites[index].rgbaCurrent.a - globalSprites[index].rgbaDefault.a;
            } else {
                tempF = globalSprites[index].rgbaDefault.a - globalSprites[index].rgbaCurrent.a;
            }

            globalSprites[index].normalized.a = (tempF * absValueRate) / globalSprites[index].rgba.a;
            
            result = TRUE;

        }
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "game/system/globalSprites", func_8002C680);

bool func_8002C680(u16 index, u16 arg1, u16 arg2) {

    bool result = FALSE;
    int temp;

    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].flags2 & ACTIVE) { 
            
            globalSprites[index].flags1 &= ~( 8 | 0x10 | 0x20 | 0x40);
            
            globalSprites[index].flags1 |= arg1 * 8;
            
            temp = arg2 << 5;
            globalSprites[index].flags1 |= temp;
            
            result = TRUE;
            
        } 
    }
    
    return result;

}

//INCLUDE_ASM(const s32, "game/system/globalSprites", func_8002C6F8);

bool func_8002C6F8(u16 index, u16 arg1) {
    
    int temp;

    bool result = FALSE;
    
    if (index < MAX_ACTIVE_SPRITES) {
        
        if (globalSprites[index].flags2 & ACTIVE) {
            
            globalSprites[index].flags1 &= ~(0x80 | 0x100);
            temp = arg1 << 7;
            globalSprites[index].flags1 |= temp;
            
            result = TRUE;

        }
    }
    
    return result;

}

//INCLUDE_ASM(const s32, "game/system/globalSprites", func_8002C768);

bool func_8002C768(u16 index, u16 arg1) {

    bool result = FALSE;
    
    if (index < MAX_ACTIVE_SPRITES) {
        
        if (globalSprites[index].flags2 & ACTIVE) {

            if (arg1) {
                globalSprites[index].flags1 |= 0x200;
            } else {
                globalSprites[index].flags1 &= ~0x200;
            }


            result = TRUE;

        }
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "game/system/globalSprites", func_8002C7EC);

bool func_8002C7EC(u16 index, u16 arg1) {
    
    int temp;

    bool result = FALSE;
    
    if (index < MAX_ACTIVE_SPRITES) {
        
        if (globalSprites[index].flags2 & ACTIVE) {
            
            globalSprites[index].flags1 &= ~(0x400 | 0x800 | 0x1000);
            temp = arg1 << 10;
            globalSprites[index].flags1 |= temp;
            
            result = TRUE;

        }
    }
    
    return result;

}

//INCLUDE_ASM(const s32, "game/system/globalSprites", setSpriteDefaultRGBA);

bool setSpriteDefaultRGBA(u16 index, u8 r, u8 g, u8 b, u8 a) {

    bool result = FALSE;

    if (index < MAX_ACTIVE_SPRITES) {
        
        if (globalSprites[index].flags2 & ACTIVE) {
            
            globalSprites[index].rgba.r = r;
            globalSprites[index].rgba.g = g;
            globalSprites[index].rgba.b = b;
            globalSprites[index].rgba.a = a;
            
            result = TRUE;

        }
    }

    return result;

}

//INCLUDE_ASM(const s32, "game/system/globalSprites", func_8002C914);

bool func_8002C914(u16 index, u8 r, u8 g, u8 b, u8 a) {

    bool result = FALSE;
    
    if (index < MAX_ACTIVE_SPRITES) {
        
        if (globalSprites[index].flags2 & ACTIVE) {        

            result = TRUE;
            
            globalSprites[index].rgbaCurrent.r = (globalSprites[index].rgba.r * r) / 255.0f;
            globalSprites[index].rgbaCurrent.g = (globalSprites[index].rgba.g * g) / 255.0f;
            globalSprites[index].rgbaCurrent.b = (globalSprites[index].rgba.b * b) / 255.0f;
            globalSprites[index].rgbaCurrent.a = (globalSprites[index].rgba.a * a) / 255.0f;

            globalSprites[index].rgbaDefault.r = (globalSprites[index].rgba.r * r) / 255.0f;
            globalSprites[index].rgbaDefault.g = (globalSprites[index].rgba.g * g) / 255.0f;
            globalSprites[index].rgbaDefault.b = (globalSprites[index].rgba.b * b) / 255.0f;
            globalSprites[index].rgbaDefault.a = (globalSprites[index].rgba.a * a) / 255.0f;

        }
    }
    
    return result;
}


//INCLUDE_ASM(const s32, "game/system/globalSprites", setSpriteAlpha);

bool setSpriteAlpha(u16 index, u8 a) {
    
    bool result = FALSE;

    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].flags2 & ACTIVE) {

            globalSprites[index].rgba.a = a;
            globalSprites[index].rgbaCurrent.a = a;
            globalSprites[index].rgbaDefault.a = a;
            
            result = TRUE;

        }
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "game/system/globalSprites", func_8002CB24);

bool func_8002CB24(u16 index, u8 flag) {

    bool result = FALSE;

    if (index < MAX_ACTIVE_SPRITES) {
        
        if (globalSprites[index].flags2 & ACTIVE) {
            if (flag == TRUE) {
                globalSprites[index].flags2 |= 0x80;
            } else {
                globalSprites[index].flags2 &= ~0x80;
            }

            result = TRUE;

        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "game/system/globalSprites", func_8002CB88);

bool func_8002CB88(u16 index, u16 paletteIndex) {

    bool result = FALSE;

    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].flags2 & ACTIVE) {

            globalSprites[index].paletteIndex = paletteIndex;
            globalSprites[index].flags2 |= 0x100;

            result = TRUE;

        }
    }

    return result;

}

//INCLUDE_ASM(const s32, "game/system/globalSprites", func_8002CBF8);

bool func_8002CBF8(u16 index) {

    bool result;

    result = FALSE;
    
    if (index < MAX_ACTIVE_SPRITES) {
        if (globalSprites[index].flags2 & ACTIVE) {
            result = (globalSprites[index].flags2 >> 10) & 1;
        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "game/system/globalSprites", func_8002CC44);

bool func_8002CC44(u16 index) {

    bool result = FALSE;

    if (index < MAX_ACTIVE_SPRITES) {
        // flag 0x40
        result = (globalSprites[index].flags2 >> 6) & 1;
    }

    return result;
    
}

// arg0 = ptr to animation index on sprite struct
// arg1 = ptr to unknown asset (byteswapped u16)
// 16 bit swap
#ifdef PERMUTER
void func_8002CC84(SpriteAnimation* arg0, u16* arg1) {

    u16 arr[2];
    
    u16 temp;
    u16 temp2;
    u16 temp3;

    temp = (arg1[2] << 8);
    temp2 = (arg1[2] >> 8);
    
    *arg0 = temp | temp2;

}
#else
INCLUDE_ASM(const s32, "game/system/globalSprites", func_8002CC84);
#endif

#ifdef PERMUTER
inline void func_8002CCA8(SpriteAnimation* arg0, u16* arg1) {

    u32 arr[2];

    u32 temp;
    u32 temp1;
    u32 temp2;
    u32 temp3;

    temp = *arg1;
    temp1 = (*arg1 << 8);
    temp >>= 8;
    temp3 |= temp;
    temp3 |= temp1;
    
    arg0->animation = temp3;

    temp1 = *(arg1+1);

    arg0->unk_2 = temp1 >> 8;
    arg0->unk_3 = temp1;
    
}
#else
INCLUDE_ASM(const s32, "game/system/globalSprites", func_8002CCA8);
#endif

#ifdef PERMUTER
void func_8002CCDC(UnknownAnimation* arg0, u16* arg1) {

    u32 temp;
    u32 temp2;
    u32 temp3;
    UnknownAnimation struc[2];

    temp = arg1[0];
    
    arg0->unk_2 = 0;
    

    temp2 = (temp & 0xFF) << 8;
    temp3 = temp >> 8;
    
    temp = temp2 | temp3;
    
    arg0->unk_0 = temp;

    
    temp = arg1[2];

    temp2 = (temp & 0xFF) << 8;
    temp3 = temp >> 8;
    
    temp = temp2 | temp3;
    
    arg0->unk_4 = temp;
    

    temp = arg1[3];

    temp2 = temp << 8;
    temp3 = temp >> 8;
    
    temp = temp2 | temp3;
    
    arg0->unk_6 = temp;
    
}
#else
INCLUDE_ASM(const s32, "game/system/globalSprites", func_8002CCDC);
#endif

//INCLUDE_ASM(const s32, "game/system/globalSprites", func_8002CD34);

// TODO: see if void* type is necessary
// unknown sprite asset index lookup
u8* func_8002CD34(u16 arg0, void* arg1) {
 
    u32 *arr = (u32*)arg1;
    
    return (u8*)(arg1 + arr[arg0]);
    
}

// alternate
/*
u8* func_8002CD34(u16 arg0, void* arg1) {
    return (u8*)(arg1 + *(u32*)(arg1 + arg0*4));
}
*/

//INCLUDE_ASM(const s32, "game/system/globalSprites", func_8002CD4C);

// FIXME: probably uses inlines; this could be an inline function used by graphic.c too
// iterates through array of byteswapped shorts and swaps and increments ptr
u16* func_8002CD4C(u16 arg0, u16* arg1) {
    
    u16 i;

    u32 temp;
    u32 temp2;
    u32 temp3;
    u8 temp4;
    
    UnknownAnimation unk;
    
    i = 0;
    
    if (arg0 != 0) {
        
        do {
            
            // FIXME: use swap inline
            temp2 = arg1[0] << 8;
            temp3 = arg1[0] >> 8;
            
            temp = temp2 | temp3;
            
            unk.unk_0 = temp;


            // FIXME: use swap inline?
            temp2 = arg1[1];
            temp = temp2;
            temp2 = temp;
            
            temp2 = temp >> 8;
            temp4 = temp >> 8;
            
            unk.unk_2 = temp4;
            unk.unk_3 = temp;
              
            arg1 += 2;
            arg1 += unk.unk_0 * 4;
            
            i++;
            
        } while (i < arg0);
    }
    
    return arg1;

}

//INCLUDE_ASM(const s32, "game/system/globalSprites", func_8002CDB4);

// unused or inline
Gfx* func_8002CDB4(u16 arg0, Gfx* arg1) {
    
    u32 i;
    
    // FIXME: typecasting i doesn't seem right
    for (i = 0; (u16)i < arg0; i++) {
        arg1++;
    }

    return arg1;
    
}

// alternate
/*
u32 func_8002CDB4(u16 arg0, u32 arg1) {
    
    u16 i = 0;

    u32 padding[2];
    
    if (arg0) {
        do {
            arg1 += 8;
            i++;
        } while (i < arg0);
    }
    
    return arg1;
}
*/

// alternate
/*
u32 func_8002CDB4(u16 arg0, u32 arg1) {
    
    u32 result;
    u32 padding[2];
    
    u16 i = 0;
    
    if (arg0) {
        do {
            result += 8;
            i++;
        } while (i < arg0);
    }
    
    return result;
}
*/

// sprites to bitmaps
INCLUDE_ASM(const s32, "game/system/globalSprites", func_8002CDE8);

// loop through all sprites and update (rgba, animation)
// main loop function
INCLUDE_ASM(const s32, "game/system/globalSprites", func_8002D3D4);
