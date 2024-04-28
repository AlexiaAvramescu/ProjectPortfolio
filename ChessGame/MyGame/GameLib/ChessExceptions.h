#pragma once

#include <stdexcept>

class ChessExceptions : public std::runtime_error
{
public:
	ChessExceptions(const std::string& errorMessage);
};

