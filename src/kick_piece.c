#include <snes.h>
#include "extern.h"

const OffsetTable *const OFFSET_TABLE_POINTERS_X[7] = {
    &i_offset_table_x, &jlstz_offset_table_x, &jlstz_offset_table_x, &jlstz_offset_table_x, &o_offset_table_x, &jlstz_offset_table_x, &jlstz_offset_table_x
};

const OffsetTable *const OFFSET_TABLE_POINTERS_Y[7] = {
    &i_offset_table_y, &jlstz_offset_table_y, &jlstz_offset_table_y, &jlstz_offset_table_y, &o_offset_table_y, &jlstz_offset_table_y, &jlstz_offset_table_y
};

static u8 current_piece_mino_absolute_positions_x[4];
static u8 current_piece_mino_absolute_positions_y[4];
static u8 kick_x;
static u8 kick_y;
static u8 kick_mino_index;
static u8 kick_index;

bool kickPiece(struct PlayerGameplayData *const player, u8 goal_rotation)
{
    const OffsetTable *const kick_table_pointer_x = OFFSET_TABLE_POINTERS_X[player->current_piece];
    const OffsetTable *const kick_table_pointer_y = OFFSET_TABLE_POINTERS_Y[player->current_piece];
    memcpy(&current_piece_mino_absolute_positions_x, &tetromino_table_x[player->current_piece][goal_rotation], sizeof(current_piece_mino_absolute_positions_x));
    memcpy(&current_piece_mino_absolute_positions_y, &tetromino_table_y[player->current_piece][goal_rotation], sizeof(current_piece_mino_absolute_positions_y));
    for (kick_mino_index = 0; kick_mino_index < 4; kick_mino_index++)
    {
        current_piece_mino_absolute_positions_x[kick_mino_index] += player->piece_position.x;
        current_piece_mino_absolute_positions_y[kick_mino_index] += player->piece_position.y;
    }
    for (kick_index = 0; kick_index < 5; kick_index++)
    {
        kick_x = (*kick_table_pointer_x)[player->rotation][kick_index] - (*kick_table_pointer_x)[goal_rotation][kick_index];
        kick_y = (*kick_table_pointer_y)[player->rotation][kick_index] - (*kick_table_pointer_y)[goal_rotation][kick_index];
        for (kick_mino_index = 0; kick_mino_index < 4; kick_mino_index++)
        {
            u8 mino_position_x = current_piece_mino_absolute_positions_x[kick_mino_index] + kick_x;
            u8 mino_position_y = current_piece_mino_absolute_positions_y[kick_mino_index] - kick_y;
            if (mino_position_x >= 10) {
                goto fail;
            }
            if (mino_position_y >= 22) {
                goto fail;
            }
            if (player->board[(u16)mino_position_x + ((u16)mino_position_y << 4)] != TILE_EMPTY)
            {
                goto fail;
            }
        }
        player->piece_position.x += kick_x;
        player->piece_position.y -= kick_y;
        return true;
    fail:;
    }
    return false;
}
