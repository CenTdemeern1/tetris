#include <snes.h>

#define FOR_ALL_SURROUNDING(E) do {E(-1, -1); E(0, -1); E(1, -1); E(-1, 0); E(1, 0); E(-1, 1); E(0, 1); E(1, 1);} while (0)

struct Vec2Di8 {
  i8 x;
  i8 y;
};

typedef Vec2Di8[4] TetrominoData;
