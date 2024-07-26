#define OAM_ENTRY_SIZE 4
#define TILEMAP_TILE_NUMBER_32x32 1024
#define TILE(tile, palette, priority, fliph, flipv) (((tile & 0b1111111111) << 0) | ((palette & 0b111) << 10) | ((priority & 0b1) << 13) | ((flipv & 0b1) << 14) | ((fliph & 0b1) << 15))
