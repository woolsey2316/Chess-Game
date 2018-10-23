#ifndef MOVETYPE_H
#define MOVETYPE_H

enum class MoveType {
  EmptySquare, Capture, Castle, Promotion, enPassant, Invalid, CheckMate, StaleMate, Draw
};

#endif // MOVETYPE_H
