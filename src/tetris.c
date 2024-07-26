#include <snes.h>
#include <string.h>
#include "snes_helpers.h"
#include "math_tables.h"
#include "tetris_helpers.h"
#include "msu1.h"

extern char tilfont, palfont;
extern char boardedges_img, boardedges_img_end, boardedges_pal, boardedges_pal_end, minoset1_img, minoset1_img_end, minoset1_pal, minoset1_pal_end, minoset2_img, minoset2_img_end, minoset2_pal, minoset2_pal_end;
extern char board_tilemap, board_tilemap_end;
extern u16 outline_table[256], outline_table_end;

const u8 SPACE_ABOVE_BOARD = 2;
const u8 HORIZONTAL_BOARD_OFFSET = 1;

enum Tiles
{
    TILE_EMPTY,
    TILE_CYAN,
    TILE_PURPLE,
    TILE_GREEN,
    TILE_BLUE,
    TILE_YELLOW,
    TILE_RED,
    TILE_GRAY,
    TILE_ORANGE,
};

const u16 BACKGROUND_TILES[8] = {
    TILE(0x20, 1, 0, 0, 0),
    TILE(0x21, 1, 0, 0, 0),
    TILE(0x22, 1, 0, 0, 0),
    TILE(0x23, 1, 0, 0, 0),
    TILE(0x24, 2, 0, 0, 0),
    TILE(0x25, 2, 0, 0, 0),
    TILE(0x26, 2, 0, 0, 0),
    TILE(0x27, 2, 0, 0, 0),
};

const u16 BACKGROUND_TILE_EMPTY = TILE(0, 0, 0, 0, 0);
const u16 BACKGROUND_TILE_EMPTY_BOARD = TILE(1, 0, 0, 0, 0);

void setBackgroundTile(u16 background[TILEMAP_TILE_NUMBER_32x32], u8 x, u8 y, u16 tile)
{
    background[x + y * 32] = tile;
}

void setTile(u8 board[], u16 background[TILEMAP_TILE_NUMBER_32x32], u8 width, u8 height, u8 x, u8 y, enum Tiles tile)
{
    if (x >= width || y >= height)
        return;
    board[x + y * width] = (u8)tile;
    setBackgroundTile(
        background,
        x + HORIZONTAL_BOARD_OFFSET,
        y,
        tile != TILE_EMPTY ? BACKGROUND_TILES[(u8)tile - 1] : y < SPACE_ABOVE_BOARD ? BACKGROUND_TILE_EMPTY
                                                                                    : BACKGROUND_TILE_EMPTY_BOARD);
}

enum Tiles getTile(u8 board[], u8 width, u8 height, u8 x, u8 y)
{
    if (x >= width || y >= height)
        return TILE_EMPTY;
    return (enum Tiles)board[x + y * width];
}

u16 getOutlineForTile(u8 board[], u8 width, u8 height, u8 x, u8 y)
{
    u8 pattern = 0;
    // P = pattern. Temporary definition
#define P(X, Y) (pattern = (pattern << 1) | ((getTile(board, width, height, x + X, y + Y) != TILE_EMPTY) & 1))
    FOR_ALL_SURROUNDING(P);
#undef P
    return outline_table[pattern];
}

void outlineTile(u8 board[], u16 background[TILEMAP_TILE_NUMBER_32x32], u8 width, u8 height, u8 x, u8 y)
{
    // O = outline. Temporary definition
#define O(X, Y)                                                                                                       \
    do                                                                                                                \
    {                                                                                                                 \
        if ((u8)(x + X) < width && (u8)(y + Y) < height && getTile(board, width, height, x + X, y + Y) == TILE_EMPTY) \
            setBackgroundTile(                                                                                        \
                background,                                                                                           \
                x + HORIZONTAL_BOARD_OFFSET + X,                                                                      \
                y + Y,                                                                                                \
                getOutlineForTile(                                                                                    \
                    board,                                                                                            \
                    width,                                                                                            \
                    height,                                                                                           \
                    x + X,                                                                                            \
                    y + Y));                                                                                          \
    } while (0)
    FOR_ALL_SURROUNDING(O);
#undef O
}

int main(void)
{
    // Init SPC700
    spcBoot();

    // Init SNES
    consoleInit();

    // Init Sprites gfx and palette with default size of 16x16
    oamInitGfxSet(&minoset1_img, (&minoset1_img_end - &minoset1_img), &minoset1_pal, (&minoset1_pal_end - &minoset1_pal), 0, 0x0000, OBJ_SIZE8_L16);
    dmaCopyVram(&minoset2_img, 0x0040, (&minoset2_img_end - &minoset2_img));
    setPalette(&minoset2_pal, 0x90, (&minoset2_pal_end - &minoset2_pal));
    // oamInitGfxAttr(0x6800, OBJ_SIZE8_L16);

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

    u8 board_width = 10;
    u8 board_height = 22;
    u8 board[10 * 22];
    memset(&board, TILE_EMPTY, sizeof(board));

    setTile(board, background0, board_width, board_height, 1, 20, TILE_CYAN);
    setTile(board, background0, board_width, board_height, 2, 20, TILE_PURPLE);
    setTile(board, background0, board_width, board_height, 0, 21, TILE_GREEN);
    setTile(board, background0, board_width, board_height, 1, 21, TILE_BLUE);
    setTile(board, background0, board_width, board_height, 7, 20, TILE_YELLOW);
    setTile(board, background0, board_width, board_height, 8, 20, TILE_RED);
    setTile(board, background0, board_width, board_height, 8, 21, TILE_GRAY);
    setTile(board, background0, board_width, board_height, 9, 21, TILE_ORANGE);
    outlineTile(board, background0, board_width, board_height, 1, 20);
    outlineTile(board, background0, board_width, board_height, 2, 20);
    outlineTile(board, background0, board_width, board_height, 0, 21);
    outlineTile(board, background0, board_width, board_height, 1, 21);
    outlineTile(board, background0, board_width, board_height, 7, 20);
    outlineTile(board, background0, board_width, board_height, 8, 20);
    outlineTile(board, background0, board_width, board_height, 8, 21);
    outlineTile(board, background0, board_width, board_height, 9, 21);
    oamSet(0 * OAM_ENTRY_SIZE, 0, 0, 3, false, false, 0x00, 0);

    free(board);

    /*// Draw a wonderful text :P
    consoleDrawText(13, 14, "Cooool!");
    consoleDrawText(13, 15, "%x", (int)(char *)background0);*/

    char ident[7] = "\0\0\0\0\0\0";
    strncpy(ident, MSU1_IDENT, 6);
    bool msu1_found;
    if (strcmp(ident, "S-MSU1") != 0)
    {
        consoleDrawText(0, 1, "MSU-1 not found");
        msu1_found = false;
    }
    else
    {
        MSU1_TRACK = 1; // Track 1
        msu1_found = true;
    }
    free(ident);

    // dmaFillVram((u8*)0x2001, 0x0000, 65536);
    // setPaletteColor(0, RGB5(0x10 >> 3, 0x10 >> 3, 0x12 >> 3));

    // Wait for nothing :P
    setScreenOn();

    if (msu1_found)
    {
        do
        {
        } while (MSU1_STATUS & MSU1_STATUS_AUDIO_BUSY);
        if (MSU1_STATUS & MSU1_STATUS_TRACK_MISSING)
            consoleDrawText(0, 1, "Track missing");
        MSU1_VOLUME = 255;                                 // Full volume
        MSU1_PLAY_CONTROLS = MSU1_PLAY_CONTROLS_PLAY_LOOP; // Play and loop
    }

    u16 rotation_timer = 0;

    while (1)
    {
        WaitForVBlank();
        dmaCopyVram((u8 *)background0, 0x6800, sizeof(background0));

        rotation_timer++;
        if (rotation_timer == 360) rotation_timer = 0;
        oamSetXY(0, COSINE[rotation_timer] >> 1, SINE[rotation_timer] >> 1);
    }
    return 0;
}