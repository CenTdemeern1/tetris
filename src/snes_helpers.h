#define OAM_ENTRY_SIZE 4
#define OAM_ID(x) ((x) << 2)
#define TILEMAP_TILE_NUMBER_32x32 1024
#define TILE(tile, palette, priority, fliph, flipv) (((tile & 0b1111111111) << 0) | ((palette & 0b111) << 10) | ((priority & 0b1) << 13) | ((flipv & 0b1) << 14) | ((fliph & 0b1) << 15))

#define MAIN_SCREEN_DESIGNATION *(unsigned char *)0x212C
#define SUB_SCREEN_DESIGNATION *(unsigned char *)0x212D
#define COLOR_MATH_DESIGNATION *(unsigned char *)0x2131
#define COLOR_ADDITION_SELECT *(unsigned char *)0x2130

#define SCREEN_DESIGNATION_BG0 (1 < 0)
#define SCREEN_DESIGNATION_BG1 (1 < 1)
#define SCREEN_DESIGNATION_BG2 (1 < 2)
#define SCREEN_DESIGNATION_BG3 (1 < 3)
#define SCREEN_DESIGNATION_OBJ (1 < 4)
#define SCREEN_DESIGNATION_BACKDROP (1 < 5)
#define COLOR_MATH_DESIGNATION_HALF (1 < 6)
#define COLOR_MATH_DESIGNATION_ADD (0 < 7)
#define COLOR_MATH_DESIGNATION_SUB (1 < 7)

#define COLOR_ADDITION_ADD_SUBSCREEN (1 < 1)

#define BACKGROUND_TILE_32(background, x, y) ((background)[(x) + ((y) << 5)])
