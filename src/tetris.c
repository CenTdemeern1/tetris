#include <snes.h>
#include <string.h>
#include "snes_helpers.h"
#include "math_tables.h"
#include "msu1.h"

extern char tilfont, palfont;
extern char boardedges_img, boardedges_img_end, boardedges_pal, boardedges_pal_end, minoset1_img, minoset1_img_end, minoset1_pal, minoset1_pal_end, minoset2_img, minoset2_img_end, minoset2_pal, minoset2_pal_end;
extern char board_tilemap, board_tilemap_end;

int main(void)
{
    // Init SPC700
    spcBoot();

    // Init SNES
    consoleInit();

    // Init Sprites gfx and palette with default size of 16x16
    // oamInitGfxSet(&slices_img, (&slices_img_end - &slices_img), &slices_pal, (&slices_pal_end - &slices_pal), 0, 0x0000, OBJ_SIZE16_L64);

    // Initialize text console with our font
    consoleSetTextVramBGAdr(0x6800);
    consoleSetTextVramAdr(0x3000);
    consoleSetTextOffset(0x0100);
    consoleInitText(7, 16 * 2, &tilfont, &palfont);
    

    // Init background
    bgSetGfxPtr(0, 0x2000);
    bgSetMapPtr(0, 0x6800, SC_32x32);

    bgInitTileSet(0, &boardedges_img, &boardedges_pal, 0, (&boardedges_img_end - &boardedges_img), (&boardedges_pal_end - &boardedges_pal), BG_16COLORS, 0x2000);
    bgInitTileSet(0, &minoset1_img, &minoset1_pal, 1, (&minoset1_img_end - &minoset1_img), (&minoset1_pal_end - &minoset1_pal), BG_16COLORS, 0x2200);
    bgInitTileSet(0, &minoset2_img, &minoset2_pal, 2, (&minoset2_img_end - &minoset2_img), (&minoset2_pal_end - &minoset2_pal), BG_16COLORS, 0x2240);

    // Now Put in 16 color mode and disable Bgs except current
    setMode(BG_MODE1, 0);
    bgSetDisable(1);
    bgSetDisable(2);

    u16 background0[TILEMAP_TILE_NUMBER_32x32];
    // memset((void *)background0, 0, sizeof(background0));
    memcpy((void *)background0, &board_tilemap, sizeof(background0));

    // Draw a wonderful text :P
    consoleDrawText(13, 14, "Cooool!");
    consoleDrawText(13, 15, "%x", (int)(char *)background0);

    char ident[7] = "\0\0\0\0\0\0";
    strncpy(ident, MSU1_IDENT, 6);
    bool msu1_found;
    if (strcmp(ident, "S-MSU1") != 0) {
        consoleDrawText(0, 1, "MSU-1 not found");
        msu1_found = false;
    } else {
        MSU1_TRACK = 1; // Track 1
        msu1_found = true;
    }
    free(ident);

    // dmaFillVram((u8*)0x2001, 0x0000, 65536);
    // setPaletteColor(0, RGB5(0x10 >> 3, 0x10 >> 3, 0x12 >> 3));

    // Wait for nothing :P
    setScreenOn();

    if (msu1_found) {
        do {} while (MSU1_STATUS & MSU1_STATUS_AUDIO_BUSY);
        if (MSU1_STATUS & MSU1_STATUS_TRACK_MISSING)
            consoleDrawText(0, 1, "Track missing");
        MSU1_VOLUME = 255;  // Full volume
        MSU1_PLAY_CONTROLS = MSU1_PLAY_CONTROLS_PLAY_LOOP; // Play and loop
    }

    while (1)
    {
        WaitForVBlank();
        dmaCopyVram((u8 *)background0, 0x6800, sizeof(background0));
    }
    return 0;
}