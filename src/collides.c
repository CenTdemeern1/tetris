#include <snes.h>
#include "tetris.h"
#include "vectors.h"

bool p1IsTileSolid(struct Vec2Du8 position)
{
    if (position.x >= 10)
        return true;
    if (position.y >= 22)
        return true;
    return BOARD_INDEX_VEC2(player1.board, position) != TILE_EMPTY;
}

bool p1CheckCollision(struct Vec2Du8 at_offset)
{
#define C(n) p1IsTileSolid((struct Vec2Du8){player1.mino_positions_x[n] + at_offset.x, player1.mino_positions_y[n] + at_offset.y})
    return C(0) || C(1) || C(2) || C(3);
#undef C
}
