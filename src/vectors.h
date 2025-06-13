#ifndef VECTORS_H
#define VECTORS_H

#include <snes.h>

#define FOR_ALL_SURROUNDING(E) do {E(-1, -1); E(0, -1); E(1, -1); E(-1, 0); E(1, 0); E(-1, 1); E(0, 1); E(1, 1);} while (0)
#define VEC2D_ADD(a, b) { (a).x + (b).x, (a).y + (b).y }
#define VEC2D_SUB(a, b) { (a).x - (b).x, (a).y - (b).y }
#define VEC2D_PTR_SUB(a, b) { (a)->x - (b)->x, (a)->y - (b)->y }

struct Vec2Du8 {
  u8 x;
  u8 y;
};

struct Vec2Du16 {
  u16 x;
  u16 y;
};

#endif
