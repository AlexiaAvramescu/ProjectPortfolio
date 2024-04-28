#include "Piece.h"

#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Pawn.h"


EPieceType Piece::GetType() const
{
	return m_type;
}

bool Piece::Is(EPieceType type, EPieceColor color) const
{
	return m_type == type && m_color == color;
}

bool Piece::Is(EPieceType type) const
{
	return m_type == type;
}

bool Piece::Is(EPieceColor color) const
{
	return m_color == color;
}

bool Piece::IsOpposite(EPieceColor color, std::vector<EPieceType> list) const
{
	if (m_color == color)
		return false;

	for (const auto& el : list)
		if (m_type == el)
			return true;

	return false;
}

Piece::Piece(EPieceType type, EPieceColor color)
{
	m_type = type;
	m_color = color;
}

PiecePtr Piece::Produce(EPieceType type, EPieceColor color)
{
	switch (type)
	{
	case EPieceType::Rook:
		return std::make_shared<Rook>(color);
		break;
	case EPieceType::Bishop:
		return std::make_shared<Bishop>(color);
		break;
	case EPieceType::Knight:
		return std::make_shared<Knight>(color);
		break;
	case EPieceType::King:
		return std::make_shared<King>(color);
		break;
	case EPieceType::Queen:
		return std::make_shared<Queen>(color);
		break;
	case EPieceType::Pawn:
		return std::make_shared<Pawn>(color);
		break;
	}
}

EPieceColor Piece::GetColor() const
{
	return m_color;
}