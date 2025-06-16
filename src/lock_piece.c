#include <snes.h>
#include "tetris.h"
#include "piece_bag.h"
#include "set_tile.h"

void p1LockPiece()
{
#define L(n)                                                                                                                       \
    p1SetTile(player1.mino_positions_x[n], player1.mino_positions_y[n], (enum Tiles)(TETROMINO_TILES[player1.current_piece] + 1)); \
    p1OutlineTile(player1.mino_positions_x[n], player1.mino_positions_y[n]);
    L(0);
    L(1);
    L(2);
    L(3);
#undef L
    p1NextPiece();
}