#ifndef SET_TILE_H
#define SET_TILE_H

#include <snes.h>
#include "tetris.h"

extern void p1SetTile(u8 x, u8 y, enum Tiles tile);
extern enum Tiles p1GetTile(u8 x, u8 y);
extern u16 p1GetOutlineForTile(u8 x, u8 y);
extern void p1OutlineTile(u8 x, u8 y);

#endif
