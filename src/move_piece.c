#include <snes.h>
#include "tetris.h"
#include "vectors.h"

void p1AttemptMove(struct Vec2Du8 to_offset)
{
    if (!p1CheckCollision(to_offset))
    {
        player1.piece_position.x += to_offset.x;
        player1.piece_position.y += to_offset.y;
#define A(n)                                    \
    player1.mino_positions_x[n] += to_offset.x; \
    player1.mino_positions_y[n] += to_offset.y
        A(0);
        A(1);
        A(2);
        A(3);
#undef A
    }
}
