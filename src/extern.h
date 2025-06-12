#ifndef EXTERN_H
#define EXTERN_H

#include <snes.h>
#include "tetris.h"

extern u8 tilfont, palfont;
extern u8 boardedges_img, boardedges_img_end, boardedges_pal, boardedges_pal_end;
extern u8 minoset1_img, minoset1_img_end, minoset1_pal, minoset1_pal_end, minoset2_img, minoset2_img_end, minoset2_pal, minoset2_pal_end;
extern u8 ghostpieceset1_img, ghostpieceset1_img_end, ghostpieceset1_pal, ghostpieceset1_pal_end, ghostpieceset2_img, ghostpieceset2_img_end, ghostpieceset2_pal, ghostpieceset2_pal_end;
extern u8 board_tilemap, board_tilemap_end;
extern u16 outline_table[256], outline_table_end;
extern TetrominoRotationsData tetromino_table_x[7], tetromino_table_x_end;
extern TetrominoRotationsData tetromino_table_y[7], tetromino_table_y_end;
extern OffsetTable jlstz_offset_table_x, jlstz_offset_table_x_end;
extern OffsetTable jlstz_offset_table_y, jlstz_offset_table_y_end;
extern OffsetTable i_offset_table_x, i_offset_table_x_end;
extern OffsetTable i_offset_table_y, i_offset_table_y_end;
extern OffsetTable o_offset_table_x, o_offset_table_x_end;
extern OffsetTable o_offset_table_y, o_offset_table_y_end;

#endif
