#include <snes.h>
#include <string.h>
#include "snes/background.h"
#include "snes/console.h"
#include "snes/input.h"
#include "snes/video.h"
#include "snes_helpers.h"
#include "math_tables.h"
#include "tetris_helpers.h"
#include "msu1.h"

extern u8 tilfont, palfont;
extern u8 boardedges_img, boardedges_img_end, boardedges_pal, boardedges_pal_end;
extern u8 minoset1_img, minoset1_img_end, minoset1_pal, minoset1_pal_end, minoset2_img, minoset2_img_end, minoset2_pal, minoset2_pal_end;
extern u8 ghostpieceset1_img, ghostpieceset1_img_end, ghostpieceset1_pal, ghostpieceset1_pal_end, ghostpieceset2_img, ghostpieceset2_img_end, ghostpieceset2_pal, ghostpieceset2_pal_end;
extern char board_tilemap, board_tilemap_end;
extern u16 outline_table[256], outline_table_end;
extern TetrominoRotationsData tetromino_table[7], tetromino_table_end;

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

enum Tetrominoes {
    TETROMINO_I,
    TETROMINO_T,
    TETROMINO_S,
    TETROMINO_J,
    TETROMINO_O,
    TETROMINO_Z,
    TETROMINO_L,
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

const u8 TETROMINO_TILES[7] = {
    0, 1, 2, 3, 4, 5, 7 // Main purpose: skip the gray mino
};

const u8 TETROMINO_PALETTES[7] = {
    0, 0, 0, 0, 1, 1, 1
};

typedef struct Vec2Di8 OffsetTable[32];

const OffsetTable JLSTZ_OFFSET_TABLE = {
    {  0,  0 }, {  0,  0 }, {  0,  0 }, {  0,  0 }, {  0,  0 }, {  0,  0 }, {  0,  0 }, {  0,  0 },
    {  0,  0 }, { +1,  0 }, { +1, -1 }, {  0, +2 }, { +1, +2 }, {  0,  0 }, {  0,  0 }, {  0,  0 },
    {  0,  0 }, {  0,  0 }, {  0,  0 }, {  0,  0 }, {  0,  0 }, {  0,  0 }, {  0,  0 }, {  0,  0 },
    {  0,  0 }, { -1,  0 }, { -1, -1 }, {  0, +2 }, { -1, +2 }, {  0,  0 }, {  0,  0 }, {  0,  0 },
};

const OffsetTable I_OFFSET_TABLE = {
    {  0,  0 }, { -1,  0 }, { +2,  0 }, { -1,  0 }, { +2,  0 }, {  0,  0 }, {  0,  0 }, {  0,  0 },
    { -1,  0 }, {  0,  0 }, {  0,  0 }, {  0, +1 }, {  0, -2 }, {  0,  0 }, {  0,  0 }, {  0,  0 },
    { -1, +1 }, { +1, +1 }, { -2, +1 }, { +1,  0 }, { -2,  0 }, {  0,  0 }, {  0,  0 }, {  0,  0 },
    {  0, +1 }, {  0, +1 }, {  0, +1 }, {  0, -1 }, {  0, +2 }, {  0,  0 }, {  0,  0 }, {  0,  0 },
};

const OffsetTable O_OFFSET_TABLE = {
    {  0,  0 }, {  0,  0 }, {  0,  0 }, {  0,  0 }, {  0,  0 }, {  0,  0 }, {  0,  0 }, {  0,  0 },
    {  0, -1 }, {  0, -1 }, {  0, -1 }, {  0, -1 }, {  0, -1 }, {  0,  0 }, {  0,  0 }, {  0,  0 },
    { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, {  0,  0 }, {  0,  0 }, {  0,  0 },
    { -1,  0 }, { -1,  0 }, { -1,  0 }, { -1,  0 }, { -1,  0 }, {  0,  0 }, {  0,  0 }, {  0,  0 },
};

const OffsetTable *const OFFSET_TABLE_POINTERS[7] = {
    &I_OFFSET_TABLE, &JLSTZ_OFFSET_TABLE, &JLSTZ_OFFSET_TABLE, &JLSTZ_OFFSET_TABLE, &O_OFFSET_TABLE, &JLSTZ_OFFSET_TABLE, &JLSTZ_OFFSET_TABLE
};

struct NextQueue {
    u8 next_queue[8];
    u8 piece_bag[14]; // Support at most 14-bags
    u8 pieces_left_in_bag;
};

struct PlayerGameplayData {
    struct NextQueue next_queue;
    u8 current_piece;
    u8 rotation;
    struct Vec2Du8 piece_position;
    u8 held_piece;
    struct Vec2Du8 board_position;
    struct Vec2Di16 board_offset;
    u8 board[16 * 22];
};

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

u8 getNextPiece(struct NextQueue *next_queue, u8 next_queue_length, u8 piece_bag_length) {
    if (piece_bag_length == 0)
        return rand() % 7; // No piece bag = complete chaos
    if (next_queue->pieces_left_in_bag == 0) {
        // TODO: Load the piece bag from somewhere instead of doing this
        u8 i;
        for (i = 0; i < piece_bag_length; i++) {
            next_queue->piece_bag[i] = i;
        }
        next_queue->pieces_left_in_bag = piece_bag_length;
    }
    u8 random_piece_index = rand() % next_queue->pieces_left_in_bag;
    u8 random_piece;
    u8 i;
    // Finds the random_piece_index-th non-255 item in the piece bag, counting from 0
    for (i = 0; i < piece_bag_length; i++) {
        if (next_queue->piece_bag[i] == 255) continue;
        if (random_piece_index == 0) {
            random_piece = next_queue->piece_bag[i];
            next_queue->piece_bag[i] = 255;
            break;
        }
        random_piece_index--;
    }
    next_queue->pieces_left_in_bag--;
    if (next_queue_length == 0)
        return random_piece;
    u8 next_piece = next_queue->next_queue[0];
    if (next_queue_length > 1)
        memmove(next_queue->next_queue, next_queue->next_queue + 1, next_queue_length - 1);
    next_queue->next_queue[next_queue_length - 1] = random_piece;
    return next_piece;
}

void nextPiece(struct PlayerGameplayData *player, u16 sprite_id_start) {
    u8 piece = getNextPiece(&player->next_queue, 5, 7);
    u8 i;
    for (i = sprite_id_start; i < sprite_id_start + 4; i++) {
        oamSet(i * OAM_ENTRY_SIZE, 0, 0, 3, false, false, TETROMINO_TILES[piece], TETROMINO_PALETTES[piece]);
        oamSet((i + 4) * OAM_ENTRY_SIZE, 0, 0, 3, false, false, TETROMINO_TILES[piece] + 8, TETROMINO_PALETTES[piece] + 4);
    }
    player->current_piece = piece;
}

bool kickPiece(struct PlayerGameplayData *player, u8 goal_rotation) {
    const OffsetTable *const kick_table_pointer = OFFSET_TABLE_POINTERS[player->current_piece];
    const struct Vec2Di8 *current_piece_offset = kick_table_pointer[player->rotation << 3];
    const struct Vec2Di8 *goal_piece_offset = kick_table_pointer[goal_rotation << 3];
    struct Vec2Du8 current_piece_mino_absolute_positions[4];
    memcpy(&current_piece_mino_absolute_positions, &tetromino_table[player->current_piece][goal_rotation], sizeof(current_piece_mino_absolute_positions));
    u8 m;
    for (m = 0; m < 4; m++) {
        struct Vec2Du8 *mino = &current_piece_mino_absolute_positions[m];
        *mino = (struct Vec2Du8)VEC2D_ADD(*mino, player->piece_position);
    }
    u8 i;
    for (i = 0; i < 5; i++) {
        struct Vec2Di8 kick = VEC2D_PTR_SUB(current_piece_offset + i, goal_piece_offset + i);
        for (m = 0; m < 4; m++) {
            struct Vec2Du8 mino_position = VEC2D_ADD(current_piece_mino_absolute_positions[m], kick);
            if (player->board[mino_position.x + (mino_position.y << 4)] != TILE_EMPTY) {
                goto fail;
            }
        }
        player->piece_position = (struct Vec2Du8)VEC2D_ADD(player->piece_position, kick);
        return true;
        fail:;
    }
    return false;
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

    u16 frame_timer = 0;
    
    u16 previous_joypad1;
    u16 joypad1 = padsCurrent(0);
    while (1)
    {
        frame_timer++;
        WaitForVBlank();
        // background0[(2 << 5) + 2] = (u16)piece;
        previous_joypad1 = joypad1; 
        joypad1 = padsCurrent(0);
        if (joypad1 & KEY_A && (previous_joypad1 & KEY_A) == 0 && kickPiece(&player1, (player1.rotation + 1) & 0b11)) {
            player1.rotation++;
        }
        if (joypad1 & KEY_B && (previous_joypad1 & KEY_B) == 0 && kickPiece(&player1, (player1.rotation - 1) & 0b11)) {
            player1.rotation--;
        }
        player1.rotation &= 0b11;
        // if (joypad1 & KEY_LEFT | KEY_RIGHT)) {
        //     player1.board_offset.x |= 0b0000010000000000;
        // }
        // player1.board_offset.x >>= 1;
        struct Vec2Di8 board_offset = { *((char *)&player1.board_offset.x + 1), *((char *)&player1.board_offset.y + 1) }; // Use the upper bytes
        // if (joypad1 & KEY_RIGHT) board_offset.x = ~board_offset.x + 1;
        struct Vec2Du8 player_board_position = VEC2D_ADD(player1.board_position, board_offset);
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

        while (player1.current_piece != TETROMINO_I) {
            nextPiece(&player1, 0);
        }
        // const struct Vec2Di8 *piece_offset = &(*OFFSET_TABLE_POINTERS[player1.current_piece])[player1.rotation << 3];
        player1.piece_position.x = 5/* + piece_offset->x*/;
        player1.piece_position.y = 1/* + piece_offset->y*/;
        struct Vec2Du16 piece_position = {
            player_board_position.x + ((player1.piece_position.x + HORIZONTAL_BOARD_OFFSET) << 3),
            player_board_position.y + ((player1.piece_position.y) << 3)
        };
        u8 i;
        for (i = 0; i < 4; i++) {
            oamSetXY(
                 i * OAM_ENTRY_SIZE,
                 piece_position.x + (tetromino_table[player1.current_piece][player1.rotation][i].x << 3),
                 piece_position.y + (tetromino_table[player1.current_piece][player1.rotation][i].y << 3)
             );
            oamSetXY((i + 4) * OAM_ENTRY_SIZE, (tetromino_table[player1.current_piece][player1.rotation][i].x << 3) + 64, (tetromino_table[player1.current_piece][player1.rotation][i].y << 3) + 151);
        }
    }
    return 0;
}
