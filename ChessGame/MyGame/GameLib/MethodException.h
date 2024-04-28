#pragma once

#include "ChessExceptions.h"

class MethodException :
    public ChessExceptions
{
public:
    MethodException(const std::string& errorMessage);
};

