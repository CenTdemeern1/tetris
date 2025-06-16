#include <snes.h>
#include "tetris.h"
#include "snes_helpers.h"
#include "extern.h"

void p1SetTile(u8 x, u8 y, enum Tiles tile)
{
    BOARD_INDEX(player1.board, x, y) = (u8)tile;
    BACKGROUND_TILE_32(background0, x + HORIZONTAL_BOARD_OFFSET, y) = tile != TILE_EMPTY
                                                                          ? BACKGROUND_TILES[(u8)tile - 1]
                                                                      : y < SPACE_ABOVE_BOARD
                                                                          ? BACKGROUND_TILE_EMPTY
                                                                          : BACKGROUND_TILE_EMPTY_BOARD;
}

enum Tiles p1GetTile(u8 x, u8 y)
{
    if (x >= BOARD_WIDTH || y >= BOARD_HEIGHT)
        return TILE_EMPTY;
    return (enum Tiles)BOARD_INDEX(player1.board, x, y);
}

u16 p1GetOutlineForTile(u8 x, u8 y)
{
    u8 pattern = 0;
#define P(X, Y) (pattern = (pattern << 1) | ((p1GetTile(x + X, y + Y) != TILE_EMPTY) & 1))
    FOR_ALL_SURROUNDING(P);
#undef P
    return outline_table[pattern];
}

void p1OutlineTile(u8 x, u8 y)
{
#define O(X, Y)                                                                                                                               \
    if ((u8)(x + X) < BOARD_WIDTH && (u8)(y + Y) < BOARD_HEIGHT && (u8)(y + Y) >= SPACE_ABOVE_BOARD && p1GetTile(x + X, y + Y) == TILE_EMPTY) \
        BACKGROUND_TILE_32(                                                                                                                   \
            background0,                                                                                                                      \
            (u8)(x + HORIZONTAL_BOARD_OFFSET + X),                                                                                            \
            (u8)(y + Y)) =                                                                                                                    \
            p1GetOutlineForTile(                                                                                                              \
                x + X,                                                                                                                        \
                y + Y);
    FOR_ALL_SURROUNDING(O);
#undef O
}
