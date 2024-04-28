#pragma once

#include "ChessExceptions.h"

class FileException :
    public ChessExceptions
{
public:
    FileException(const std::string& errorMessage);
};

