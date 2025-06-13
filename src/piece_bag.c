#include <snes.h>
#include "tetris.h"
#include "extern.h"
#include "snes_helpers.h"

u8 p1GetNextPiece(u8 next_queue_length, u8 piece_bag_length)
{
    if (piece_bag_length == 0)
        return rand() % 7; // No piece bag = complete chaos
    if (player1.next_queue.pieces_left_in_bag == 0)
    {
        // TODO: Load the piece bag from somewhere instead of doing this
        u8 i;
        for (i = 0; i < piece_bag_length; i++)
        {
            player1.next_queue.piece_bag[i] = i;
        }
        player1.next_queue.pieces_left_in_bag = piece_bag_length;
    }
    u8 random_piece_index = rand() % player1.next_queue.pieces_left_in_bag;
    u8 random_piece;
    u8 i;
    // Finds the random_piece_index-th non-255 item in the piece bag, counting from 0
    for (i = 0; i < piece_bag_length; i++)
    {
        if (player1.next_queue.piece_bag[i] == 255)
            continue;
        if (random_piece_index == 0)
        {
            random_piece = player1.next_queue.piece_bag[i];
            player1.next_queue.piece_bag[i] = 255;
            break;
        }
        random_piece_index--;
    }
    player1.next_queue.pieces_left_in_bag--;
    if (next_queue_length == 0)
        return random_piece;
    u8 next_piece = player1.next_queue.next_queue[0];
    if (next_queue_length > 1)
        memmove(player1.next_queue.next_queue, player1.next_queue.next_queue + 1, next_queue_length - 1);
    player1.next_queue.next_queue[next_queue_length - 1] = random_piece;
    return next_piece;
}

#define P1_SPRITE_ID_START 0
#define P1_SPRITE_ID_END 4

static u8 next_piece;

void p1NextPiece()
{
    next_piece = p1GetNextPiece(5, 7);
    u8 i;
    for (i = P1_SPRITE_ID_START; i < P1_SPRITE_ID_END; i++)
    {
        oamSet(i * OAM_ENTRY_SIZE, 0, 0, 3, false, false, TETROMINO_TILES[next_piece], TETROMINO_PALETTES[next_piece]);
        oamSet((i + 4) * OAM_ENTRY_SIZE, 0, 0, 3, false, false, TETROMINO_TILES[next_piece] + 8, TETROMINO_PALETTES[next_piece] + 4);
    }
    player1.current_piece = next_piece;
    player1.rotation = 0;
    player1.piece_position.x = 5 /* + piece_offset->x*/;
    player1.piece_position.y = 1 /* + piece_offset->y*/;
    memcpy(&player1.mino_positions_x, &tetromino_table_x[next_piece][0], sizeof(player1.mino_positions_x));
    memcpy(&player1.mino_positions_y, &tetromino_table_y[next_piece][0], sizeof(player1.mino_positions_y));
#define M(n)                          \
    player1.mino_positions_x[n] += 5; \
    player1.mino_positions_y[n] += 1
    M(0);
    M(1);
    M(2);
    M(3);
#undef M
}
