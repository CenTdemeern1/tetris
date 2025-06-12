.include "hdr.asm"

.section ".rodata1" superfree

tilfont: .incbin "res/pvsneslibfont.pic"
palfont: .incbin "res/pvsneslibfont.pal"

boardedges_img: .incbin "res/boardedges.pic"
boardedges_img_end:
boardedges_pal: .incbin "res/boardedges.pal"
boardedges_pal_end:
minoset1_img: .incbin "res/minoset1.pic"
minoset1_img_end:
minoset1_pal: .incbin "res/minoset1.pal"
minoset1_pal_end:
minoset2_img: .incbin "res/minoset2.pic"
minoset2_img_end:
minoset2_pal: .incbin "res/minoset2.pal"
minoset2_pal_end:
ghostpieceset1_img: .incbin "res/ghostpieceset1.pic"
ghostpieceset1_img_end:
ghostpieceset1_pal: .incbin "res/ghostpieceset1.pal"
ghostpieceset1_pal_end:
ghostpieceset2_img: .incbin "res/ghostpieceset2.pic"
ghostpieceset2_img_end:
ghostpieceset2_pal: .incbin "res/ghostpieceset2.pal"
ghostpieceset2_pal_end:

board_tilemap: .incbin "data/generated/emptyboard.bin"
board_tilemap_end:

outline_table: .incbin "data/generated/outline_table.bin"
outline_table_end:

tetromino_table_x: .incbin "data/generated/tetromino_table_x.bin"
tetromino_table_x_end:

tetromino_table_y: .incbin "data/generated/tetromino_table_y.bin"
tetromino_table_y_end:

.ends
