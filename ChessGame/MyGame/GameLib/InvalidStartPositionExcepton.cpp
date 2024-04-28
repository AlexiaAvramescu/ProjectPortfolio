#include "InvalidStartPositionExcepton.h"

InvalidStartPositionExcepton::InvalidStartPositionExcepton()
	: PieceMovementExceptions("[CAN'T MAKE MOVE] Start position is null or not player's piece.\n")
{
}
