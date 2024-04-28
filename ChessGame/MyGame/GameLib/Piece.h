#pragma once

#include "IPiece.h"

#include <string>
#include <memory>
#include <cmath>

class Board;
using PiecePtr = std::shared_ptr<class Piece>;

class Piece : public IPiece
{
public:
	Piece(EPieceType type, EPieceColor color);

	static PiecePtr Produce(EPieceType type, EPieceColor color);

	// IPiece implementation
	EPieceColor GetColor() const override;
	EPieceType GetType() const override;

	bool Is(EPieceType type, EPieceColor color) const override;
	bool Is(EPieceType type) const override;
	bool Is(EPieceColor color) const override;
	bool IsOpposite(EPieceColor color, std::vector<EPieceType> list) const;

protected:
	EPieceType m_type;
	EPieceColor m_color;
	PositionList m_possibleMoves;
};
