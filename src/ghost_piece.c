#include <snes.h>
#include "tetris.h"
#include "vectors.h"
#include "collides.h"

static u8 ghost_piece_offset_check;

void p1RelocateGhostPiece()
{
    ghost_piece_offset_check = 0;
    do
    {
    } while (!p1CheckCollision((struct Vec2Du8){0, ++ghost_piece_offset_check}));
    player1.ghost_piece_height_offset = --ghost_piece_offset_check;
    player1.ghost_piece_dirty = false;
}
