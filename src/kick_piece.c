#include <snes.h>
#include "tetris.h"
#include "extern.h"
#include "collides.h"

const OffsetTable *const OFFSET_TABLE_POINTERS_X[7] = {
    &i_offset_table_x, &jlstz_offset_table_x, &jlstz_offset_table_x, &jlstz_offset_table_x, &o_offset_table_x, &jlstz_offset_table_x, &jlstz_offset_table_x};

const OffsetTable *const OFFSET_TABLE_POINTERS_Y[7] = {
    &i_offset_table_y, &jlstz_offset_table_y, &jlstz_offset_table_y, &jlstz_offset_table_y, &o_offset_table_y, &jlstz_offset_table_y, &jlstz_offset_table_y};

static u8 current_piece_mino_absolute_positions_x[4];
static u8 current_piece_mino_absolute_positions_y[4];
static u8 kick_x;
static u8 kick_y;
static u8 kick_mino_index;
static u8 kick_index;

bool p1KickPiece(u8 goal_rotation)
{
    const OffsetTable *const kick_table_pointer_x = OFFSET_TABLE_POINTERS_X[player1.current_piece];
    const OffsetTable *const kick_table_pointer_y = OFFSET_TABLE_POINTERS_Y[player1.current_piece];
    memcpy(&current_piece_mino_absolute_positions_x, &tetromino_table_x[player1.current_piece][goal_rotation], sizeof(current_piece_mino_absolute_positions_x));
    memcpy(&current_piece_mino_absolute_positions_y, &tetromino_table_y[player1.current_piece][goal_rotation], sizeof(current_piece_mino_absolute_positions_y));
    for (kick_mino_index = 0; kick_mino_index < 4; kick_mino_index++)
    {
        current_piece_mino_absolute_positions_x[kick_mino_index] += player1.piece_position.x;
        current_piece_mino_absolute_positions_y[kick_mino_index] += player1.piece_position.y;
    }
    for (kick_index = 0; kick_index < 5; kick_index++)
    {
        kick_x = (*kick_table_pointer_x)[player1.rotation][kick_index] - (*kick_table_pointer_x)[goal_rotation][kick_index];
        kick_y = (*kick_table_pointer_y)[player1.rotation][kick_index] - (*kick_table_pointer_y)[goal_rotation][kick_index];
        for (kick_mino_index = 0; kick_mino_index < 4; kick_mino_index++)
        {
            if (p1IsTileSolid((struct Vec2Du8){
                current_piece_mino_absolute_positions_x[kick_mino_index] + kick_x,
                current_piece_mino_absolute_positions_y[kick_mino_index] - kick_y
            }))
            {
                goto fail;
            }
        }
        player1.piece_position.x += kick_x;
        player1.piece_position.y -= kick_y;
#define K(n)                                                                           \
    player1.mino_positions_x[n] = current_piece_mino_absolute_positions_x[n] + kick_x; \
    player1.mino_positions_y[n] = current_piece_mino_absolute_positions_y[n] - kick_y
        K(0);
        K(1);
        K(2);
        K(3);
#undef K
        return true;
    fail:;
    }
    return false;
}

static u8 attempt_goal_rotation;

void p1AttemptRotate(u8 relative_rotation) {
    attempt_goal_rotation = (player1.rotation + relative_rotation) & 0b11;
    if (p1KickPiece(attempt_goal_rotation))
    {
        player1.rotation = attempt_goal_rotation;
    }
}
