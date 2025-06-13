#ifndef COLLIDES_H
#define COLLIDES_H

#include <snes.h>
#include "tetris.h"
#include "vectors.h"

extern bool p1IsTileSolid(struct Vec2Du8 position);
extern bool p1CheckCollision(struct Vec2Du8 at_offset);

#endif
