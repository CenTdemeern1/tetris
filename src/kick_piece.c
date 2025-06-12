#include <snes.h>
#include "extern.h"

const OffsetTable *const OFFSET_TABLE_POINTERS_X[7] = {
    &i_offset_table_x, &jlstz_offset_table_x, &jlstz_offset_table_x, &jlstz_offset_table_x, &o_offset_table_x, &jlstz_offset_table_x, &jlstz_offset_table_x
};

const OffsetTable *const OFFSET_TABLE_POINTERS_Y[7] = {
    &i_offset_table_y, &jlstz_offset_table_y, &jlstz_offset_table_y, &jlstz_offset_table_y, &o_offset_table_y, &jlstz_offset_table_y, &jlstz_offset_table_y
};

bool kickPiece(struct PlayerGameplayData *player, u8 goal_rotation)
{
    const OffsetTable *const kick_table_pointer_x = OFFSET_TABLE_POINTERS_X[player->current_piece];
    const OffsetTable *const kick_table_pointer_y = OFFSET_TABLE_POINTERS_Y[player->current_piece];
    const struct Vec2Du8 *current_piece_offset = kick_table_pointer[player->rotation << 3];
    const struct Vec2Du8 *goal_piece_offset = kick_table_pointer[goal_rotation << 3];
    struct Vec2Du8 current_piece_mino_absolute_positions[4];
    memcpy(&current_piece_mino_absolute_positions, &tetromino_table[player->current_piece][goal_rotation], sizeof(current_piece_mino_absolute_positions));
    u8 m;
    for (m = 0; m < 4; m++)
    {
        struct Vec2Du8 *mino = &current_piece_mino_absolute_positions[m];
        *mino = (struct Vec2Du8)VEC2D_ADD(*mino, player->piece_position);
    }
    u8 i;
    for (i = 0; i < 5; i++)
    {
        struct Vec2Du8 kick = VEC2D_PTR_SUB(current_piece_offset + i, goal_piece_offset + i);
        WaitForVBlank();
        int x = kick.x;
        int y = kick.y;
        consoleDrawText(1, 1, "%i, %i", x, y);
        for (m = 0; m < 4; m++)
        {
            struct Vec2Du8 mino_position = VEC2D_ADD(current_piece_mino_absolute_positions[m], kick);
            if (player->board[mino_position.x + (mino_position.y << 4)] != TILE_EMPTY)
            {
                goto fail;
            }
        }
        player->piece_position = (struct Vec2Du8)VEC2D_ADD(player->piece_position, kick);
        return true;
    fail:;
    }
    return false;
}
