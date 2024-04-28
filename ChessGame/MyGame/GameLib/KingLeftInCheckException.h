#pragma once
#include "PieceMovementExceptions.h"

class KingLeftInCheckException :
    public PieceMovementExceptions
{
public:
    KingLeftInCheckException();
};

