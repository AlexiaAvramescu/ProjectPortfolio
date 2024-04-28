#include "ChessExceptions.h"

ChessExceptions::ChessExceptions(const std::string& errorMessage)
	: std::runtime_error(errorMessage)
{

}
