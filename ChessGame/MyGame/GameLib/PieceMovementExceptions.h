#pragma once
#include "ChessExceptions.h"
class PieceMovementExceptions :
    public ChessExceptions
{
public:
    PieceMovementExceptions(const std::string& errorMessage);
};

