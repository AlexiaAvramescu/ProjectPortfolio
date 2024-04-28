#pragma once

#include "Piece.h"

class Board;

class Queen: public Piece
{
public:
	Queen(EPieceColor color);
	bool CanMove(Position startPos, Position endPos, bool isKingAttacking, const Board& board);
	PositionList GetPossibleMoves(Position piecePos, bool isKingAttacking, const Board& board);

};