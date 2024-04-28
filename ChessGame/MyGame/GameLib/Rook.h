#pragma once

#include "Piece.h"

class Board;

class Rook : public Piece
{
public:
	Rook(EPieceColor color);
	bool CanMove(Position startPos, Position endPos, bool isKingAttacking, const Board& board);
	PositionList GetPossibleMoves(Position piecePos, bool isKingAttacking, const Board& board);
};