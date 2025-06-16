#ifndef TETRIS_H
#define TETRIS_H

#include <snes.h>
#include "vectors.h"
#include "snes_helpers.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 22
#define BOARD_INDEX(board, x, y) ((board)[(u16)(x) + ((u16)(y) << 4)])
#define BOARD_INDEX_VEC2(board, vec) ((board)[(u16)(vec).x + ((u16)(vec).y << 4)])

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

enum Tetrominoes
{
    TETROMINO_I,
    TETROMINO_T,
    TETROMINO_S,
    TETROMINO_J,
    TETROMINO_O,
    TETROMINO_Z,
    TETROMINO_L,
};

typedef u8 TetrominoData[4];
typedef TetrominoData TetrominoRotationsData[4];

typedef u8 OffsetTableRotation[8];
typedef OffsetTableRotation OffsetTable[4];

struct NextQueue
{
    u8 next_queue[8];
    u8 piece_bag[14]; // Support at most 14-bags
    u8 pieces_left_in_bag;
};

struct PlayerGameplayData
{
    struct NextQueue next_queue;
    u8 current_piece;
    u8 held_piece;
    u8 rotation;
    struct Vec2Du8 piece_position;
    u8 mino_positions_x[4];
    u8 mino_positions_y[4];
    struct Vec2Du8 board_position;
    struct Vec2Du16 board_offset;
    u8 board[16 * 22];
};

extern struct PlayerGameplayData player1;

const u8 SPACE_ABOVE_BOARD = 2;
const u8 HORIZONTAL_BOARD_OFFSET = 1;

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
    0, 0, 0, 0, 1, 1, 1};

#endif
