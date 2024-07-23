.include "hdr.asm"

.section ".rodata1" superfree

tilfont: .incbin "pvsneslibfont.pic"
palfont: .incbin "pvsneslibfont.pal"

boardedges_img: .incbin "boardedges.pic"
boardedges_img_end:
boardedges_pal: .incbin "boardedges.pal"
boardedges_pal_end:
minoset1_img: .incbin "minoset1.pic"
minoset1_img_end:
minoset1_pal: .incbin "minoset1.pal"
minoset1_pal_end:
minoset2_img: .incbin "minoset2.pic"
minoset2_img_end:
minoset2_pal: .incbin "minoset2.pal"
minoset2_pal_end:

board_tilemap: .incbin "tilemap.bin"
board_tilemap_end:

.ends
