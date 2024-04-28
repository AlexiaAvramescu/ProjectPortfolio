#pragma once

#include "EPieceColor.h"
#include "EPieceType.h"
#include "Position.h"

class Board;


class IPiece
{
public:
	virtual EPieceColor GetColor() const = 0;
	virtual EPieceType GetType() const = 0;
	virtual PositionList GetPossibleMoves(Position piecePos, bool isKingAttacking, const Board& board) = 0;

	virtual bool CanMove(Position startPos, Position endPos, bool isKingAttacking, const Board& board) = 0;
	virtual bool Is(EPieceType type, EPieceColor color) const = 0;
	virtual bool Is(EPieceType type) const = 0;
	virtual bool Is(EPieceColor color) const = 0;

	virtual ~IPiece() = default;
};
