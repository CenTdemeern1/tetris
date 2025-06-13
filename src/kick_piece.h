#ifndef KICK_PIECE_H
#define KICK_PIECE_H

#include <snes.h>
#include "tetris.h"

extern bool p1KickPiece(u8 goal_rotation);
extern void p1AttemptRotate(u8 relative_rotation);

#endif
