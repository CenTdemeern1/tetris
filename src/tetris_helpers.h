#define FOR_ALL_SURROUNDING(E) do {E(-1, -1); E(0, -1); E(1, -1); E(-1, 0); E(1, 0); E(-1, 1); E(0, 1); E(1, 1);} while (0)
#define VEC2D_ADD(a, b) { (a).x + (b).x, (a).y + (b).y }
#define VEC2D_SUB(a, b) { (a).x - (b).x, (a).y - (b).y }
#define VEC2D_PTR_SUB(a, b) { (a)->x - (b)->x, (a)->y - (b)->y }

struct Vec2Di8 {
  char x;
  char y;
};
struct Vec2Du8 {
  unsigned char x;
  unsigned char y;
};
struct Vec2Di16 {
  short x;
  short y;
};
struct Vec2Du16 {
  unsigned short x;
  unsigned short y;
};

typedef struct Vec2Di8 TetrominoData[4]; // Slow, allegedly
typedef TetrominoData TetrominoRotationsData[4];
