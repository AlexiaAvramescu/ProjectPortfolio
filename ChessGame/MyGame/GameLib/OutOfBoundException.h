#pragma once

#include "ChessExceptions.h"

class OutOfBoundException : public ChessExceptions
{
public:
	OutOfBoundException();
};
