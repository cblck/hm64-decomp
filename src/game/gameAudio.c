#include "common.h"

#include "system/audio.h"

#include "gameAudio.h"

// bss or system/audio.c
extern Sfx gSfx[4];
extern SongInfo gSongs[4];

// rodata
extern WaveTableInfo waveTableAddresses[1];
extern Addresses songRomAddresses[0x40];
extern Addresses sfxRomAddresses[0x80];
extern s32 volumesTable[];
// rodata: 0xF1CFC
// size 0x60 or 0x180? 0x60 set elements, but takes up 0x180 bytes in rodata
extern u8 audioTypeTable[0x60];


//INCLUDE_ASM(const s32, "game/gameAudio", initializeWaveTable);

void initializeWaveTable(u16 waveTableIndex) {
    setSongBank(waveTableAddresses[waveTableIndex].pbk_addr, waveTableAddresses[waveTableIndex].pbk_end, waveTableAddresses[waveTableIndex].wbk_addr);
}

//INCLUDE_ASM(const s32, "game/gameAudio", setCurrentSong);

void setCurrentSong(u16 songIndex) {

    if (songIndex < TOTAL_SONGS) {
        setSong(0, (u8*)songRomAddresses[songIndex].romAddrStart, (u8*)songRomAddresses[songIndex].romAddrEnd);
    }
    
    setSongVolumes(0, 0, 0);

}

//INCLUDE_ASM(const s32, "game/gameAudio", setSongWithDefaultSpeed);

void setSongWithDefaultSpeed(u16 songIndex) {
    if (songIndex < TOTAL_SONGS) {
        setSongSpeed(0, 32);
    }
}

//INCLUDE_ASM(const s32, "game/gameAudio", stopCurrentSong);

void stopCurrentSong(u16 songIndex) {
    if (songIndex  < TOTAL_SONGS) {
        stopSong(0);
    }
}

//INCLUDE_ASM(const s32, "game/gameAudio", checkDefaultSongChannelOpen);

// check if first song is open/not set
bool checkDefaultSongChannelOpen(u16 songIndex) {
    
    u8 result = FALSE;
    
    if (songIndex < TOTAL_SONGS) {
        result = gSongs[0].flags == 0;
    }

    if (songIndex == 0xFF) {
        result = TRUE;
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "game/gameAudio", setSongVolume);

void setSongVolume(u16 songIndex, u32 arg1) {
    if (songIndex < TOTAL_SONGS) {
        setSongVolumes(0, arg1, 32);
    }
}

//INCLUDE_ASM(const s32, "game/gameAudio", func_800ACC1C);

void func_800ACC1C(u16 songIndex) {
    if (songIndex < TOTAL_SONGS) {
        setSongVolumes(0, 0, 32);
    }
}

//INCLUDE_ASM(const s32, "game/gameAudio", setAudio);

// only used for sfx
void setAudio(u16 index) {
    
  if (index < TOTAL_SFX) {
      
    if (audioTypeTable[index] == SFX) {
      
      setSfx(index + 1);
      setSfxVolume(index + 1, volumesTable[index]);

    } else {

        // FIXME: for sfx used as background music?
        setSong(audioTypeTable[index], (u8*)sfxRomAddresses[index].romAddrStart, (u8*)sfxRomAddresses[index].romAddrEnd);
        setSongVolumes(audioTypeTable[index], volumesTable[index], volumesTable[index]);
    
    }
  }
}

//INCLUDE_ASM(const s32, "game/gameAudio", checkAllSfxInactive);

// check if all sfx objects are inactive
u8 checkAllSfxInactive(void) {

    u8 i;
    u8 count;

    count = 0;

    for (i = 0; i < MAX_ACTIVE_SFX; i++) {
        if (!gSfx[i].flags) {
            count++;
        }
    }
    
    return count == 4;

}