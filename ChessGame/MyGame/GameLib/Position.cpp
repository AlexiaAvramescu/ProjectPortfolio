#include "Position.h"

Position::Position(int x /*= -1*/, int y /*= -1*/)
	: x(x)
	, y(y)
{

}

bool Position::operator!=(const Position& other) const
{
	return !(*this == other);
}

bool Position::operator==(const Position& other) const
{
	return x == other.x && y == other.y;
}