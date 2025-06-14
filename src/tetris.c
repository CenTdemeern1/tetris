#include <snes.h>
#include <string.h>
#include "snes/background.h"
#include "snes/console.h"
#include "snes/input.h"
#include "snes/interrupt.h"
#include "snes/sprite.h"
#include "snes/video.h"
#include "snes_helpers.h"
#include "math_tables.h"
#include "vectors.h"
#include "msu1.h"
#include "extern.h"
#include "kick_piece.h"
#include "collides.h"
#include "move_piece.h"
#include "piece_bag.h"
#include "tetris.h"

static u16 background0[TILEMAP_TILE_NUMBER_32x32];
static u8 board_width = 10;
static u8 board_height = 22;
static struct PlayerGameplayData player1;

void setBackgroundTile(u16 background[TILEMAP_TILE_NUMBER_32x32], u8 x, u8 y, u16 tile)
{
    background[x + y * 32] = tile;
}

void setTile(u8 board[], u16 background[TILEMAP_TILE_NUMBER_32x32], u8 width, u8 height, u8 x, u8 y, enum Tiles tile)
{
    if (x >= width || y >= height)
        return;
    board[x + (y << 4)] = (u8)tile;
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
    return (enum Tiles)board[x + (y << 4)];
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

static bool move_left_bumped = false;
static bool move_right_bumped = false;
static bool last_bump_was_left = false;

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
    dmaCopyVram(&ghostpieceset1_img, 0x0080, (&ghostpieceset1_img_end - &ghostpieceset1_img));
    setPalette(&ghostpieceset1_pal, 0xC0, (&ghostpieceset1_pal_end - &ghostpieceset1_pal));
    dmaCopyVram(&ghostpieceset2_img, 0x00C0, (&ghostpieceset2_img_end - &ghostpieceset2_img));
    setPalette(&ghostpieceset2_pal, 0xD0, (&ghostpieceset2_pal_end - &ghostpieceset2_pal));
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
    // bgSetDisable(4); // OAM
    bgSetEnableSub(0);
    setColorEffect(CM_SUBBGOBJ_ENABLE, CM_MSCR_PAL47);

    // memset((void *)background0, 0, sizeof(background0));
    memcpy((void *)background0, &board_tilemap, sizeof(background0));

    memset(&player1, 0, sizeof(player1));

    setTile(player1.board, background0, board_width, board_height, 1, 20, TILE_CYAN);
    setTile(player1.board, background0, board_width, board_height, 2, 20, TILE_PURPLE);
    setTile(player1.board, background0, board_width, board_height, 0, 21, TILE_GREEN);
    setTile(player1.board, background0, board_width, board_height, 1, 21, TILE_BLUE);
    setTile(player1.board, background0, board_width, board_height, 7, 20, TILE_YELLOW);
    setTile(player1.board, background0, board_width, board_height, 8, 20, TILE_RED);
    setTile(player1.board, background0, board_width, board_height, 8, 21, TILE_GRAY);
    setTile(player1.board, background0, board_width, board_height, 9, 21, TILE_ORANGE);
    outlineTile(player1.board, background0, board_width, board_height, 1, 20);
    outlineTile(player1.board, background0, board_width, board_height, 2, 20);
    outlineTile(player1.board, background0, board_width, board_height, 0, 21);
    outlineTile(player1.board, background0, board_width, board_height, 1, 21);
    outlineTile(player1.board, background0, board_width, board_height, 7, 20);
    outlineTile(player1.board, background0, board_width, board_height, 8, 20);
    outlineTile(player1.board, background0, board_width, board_height, 8, 21);
    outlineTile(player1.board, background0, board_width, board_height, 9, 21);

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

    player1.board_position.y = 16;
    do
    {
        p1NextPiece(); // FIXME: This changes sprites and this needs to be called during vblank. This should be refactored
    } while (player1.current_piece != TETROMINO_I);

    u16 frame_timer = 0;

    u16 previous_joypad1;
    u16 joypad1 = padsCurrent(0);
    while (1)
    {
        // Processing
        frame_timer++;
        // scanPads(); // ?
        previous_joypad1 = joypad1;
        joypad1 = padsCurrent(0);

        if (joypad1 & KEY_LEFT && (previous_joypad1 & KEY_LEFT) == 0)
        {
            move_left_bumped = p1AttemptMove((struct Vec2Du8){-1, 0});
        }
        if (joypad1 & KEY_RIGHT && (previous_joypad1 & KEY_RIGHT) == 0)
        {
            move_right_bumped = p1AttemptMove((struct Vec2Du8){1, 0});
        }
        if (joypad1 & KEY_UP && (previous_joypad1 & KEY_UP) == 0)
        {
            p1AttemptMove((struct Vec2Du8){0, -1});
        }
        if (joypad1 & KEY_DOWN && (previous_joypad1 & KEY_DOWN) == 0)
        {
            p1AttemptMove((struct Vec2Du8){0, 1});
        }

        if (joypad1 & KEY_A && (previous_joypad1 & KEY_A) == 0)
        {
            p1AttemptRotate(1);
        }
        if (joypad1 & KEY_B && (previous_joypad1 & KEY_B) == 0)
        {
            p1AttemptRotate(-1);
        }
        player1.rotation &= 0b11;
        if (joypad1 & KEY_LEFT && move_left_bumped)
        {
            player1.board_offset.x |= 0b0000010000000000;
            last_bump_was_left = true;
        }
        else if (joypad1 & KEY_RIGHT && move_right_bumped)
        {
            player1.board_offset.x |= 0b0000010000000000;
            last_bump_was_left = false;
        }
        player1.board_offset.x >>= 1;
        struct Vec2Du8 board_offset = {*((char *)&player1.board_offset.x + 1), *((char *)&player1.board_offset.y + 1)}; // Use the upper bytes
        if (last_bump_was_left)
            board_offset.x = -board_offset.x;
        struct Vec2Du8 player_board_position = VEC2D_ADD(player1.board_position, board_offset);

        // Set up graphics during vblank
        WaitForVBlank();

        // background0[(2 << 5) + 2] = (u16)piece;

        bgSetScroll(0, -player_board_position.x, -player_board_position.y);
        dmaCopyVram((u8 *)background0, 0x6800, sizeof(background0));
        showFPScounter();
        // consoleDrawText(1, 1, "%x ", (u32)player1.next_queue.piece_bag[0]);
        // consoleDrawText(4, 1, "%x ", (u32)player1.next_queue.piece_bag[1]);
        // consoleDrawText(7, 1, "%x ", (u32)player1.next_queue.piece_bag[2]);
        // consoleDrawText(10, 1, "%x ", (u32)player1.next_queue.piece_bag[3]);
        // consoleDrawText(13, 1, "%x ", (u32)player1.next_queue.piece_bag[4]);
        // consoleDrawText(16, 1, "%x ", (u32)player1.next_queue.piece_bag[5]);
        // consoleDrawText(19, 1, "%x ", (u32)player1.next_queue.piece_bag[6]);
        // WaitForVBlank();

        // const struct Vec2Du8 *piece_offset = &(*OFFSET_TABLE_POINTERS[player1.current_piece])[player1.rotation << 3];
        struct Vec2Du8 piece_position = {
            player_board_position.x + ((player1.piece_position.x + HORIZONTAL_BOARD_OFFSET) << 3),
            player_board_position.y + ((player1.piece_position.y) << 3) - 1};
        u8 i;
        for (i = 0; i < 4; i++)
        {
            oamSetXY(
                OAM_ID(i),
                piece_position.x + (tetromino_table_x[player1.current_piece][player1.rotation][i] << 3),
                piece_position.y + (tetromino_table_y[player1.current_piece][player1.rotation][i] << 3));
            oamSetXY(
                OAM_ID(i + 4),
                (tetromino_table_x[player1.current_piece][player1.rotation][i] << 3) + 64,
                (tetromino_table_y[player1.current_piece][player1.rotation][i] << 3) + 151);
        }
        oamSetXY(OAM_ID(8), player1.piece_position.x, player1.piece_position.y);
    }
    return 0;
}
