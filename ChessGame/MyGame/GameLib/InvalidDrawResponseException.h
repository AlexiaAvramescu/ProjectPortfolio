#pragma once
#include "ChessExceptions.h"
class InvalidDrawResponseException :
    public ChessExceptions
{
public:
    InvalidDrawResponseException();
};

