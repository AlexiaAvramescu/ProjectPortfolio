#pragma once

#include "PieceMovementExceptions.h"

class InvalidMovingPatternException :
    public PieceMovementExceptions
{
public:
    InvalidMovingPatternException();
};

