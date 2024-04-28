#include "KingLeftInCheckException.h"

KingLeftInCheckException::KingLeftInCheckException()
	: PieceMovementExceptions("[CAN'T MAKE MOVE] King is left in check.\n")
{
}
