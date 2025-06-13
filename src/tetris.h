#ifndef TETRIS_H
#define TETRIS_H

#include <snes.h>
#include "vectors.h"

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

#endif
