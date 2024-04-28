#pragma once
#include "PieceMovementExceptions.h"
class InvalidStartPositionExcepton :
    public PieceMovementExceptions
{
public:
    InvalidStartPositionExcepton();
};

