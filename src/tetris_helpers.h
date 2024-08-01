#define FOR_ALL_SURROUNDING(E) do {E(-1, -1); E(0, -1); E(1, -1); E(-1, 0); E(1, 0); E(-1, 1); E(0, 1); E(1, 1);} while (0)

struct Vec2Di8 {
  char x;
  char y;
};

typedef struct Vec2Di8 TetrominoData[4];
typedef TetrominoData TetrominoRotationsData[4];
