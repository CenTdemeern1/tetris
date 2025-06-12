#pragma once
#include <snes.h>
#include "vectors.h"

typedef u8 TetrominoData[4];
typedef TetrominoData TetrominoRotationsData[4];

typedef u8 OffsetTable[32];

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
    u8 rotation;
    struct Vec2Du8 piece_position;
    u8 held_piece;
    struct Vec2Du8 board_position;
    struct Vec2Du16 board_offset;
    u8 board[16 * 22];
};
