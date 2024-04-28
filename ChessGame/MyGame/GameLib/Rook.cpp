#include "Rook.h"
#include "Board.h"

Rook::Rook(EPieceColor color)
	: Piece(EPieceType::Rook, color)
{

}

bool Rook::CanMove(Position startPos, Position endPos, bool isKingAttacking, const Board& board)
{
	PositionList possibleMoves;
	possibleMoves = GetPossibleMoves(startPos, false, board);
	if (std::find(possibleMoves.begin(), possibleMoves.end(), endPos) != possibleMoves.end())
		return true;

	return false;
}

PositionList Rook::GetPossibleMoves(Position piecePos, bool isKingAttacking, const Board& board)
{
	PositionList possibleMoves;
	EPieceColor pieceColor = GetColor();
	auto localBoard = board.GetBoard();

	int i = piecePos.x + 1;
	while (i < 8)
	{
		if (!board.IsKingLeftInCheck(piecePos, { i, piecePos.y }, GetColor()))
		{
			if (localBoard[i][piecePos.y] != nullptr)
			{
				if (localBoard[i][piecePos.y]->GetColor() != pieceColor)
					possibleMoves.push_back(Position(i, piecePos.y));
				break;
			}
			else
				possibleMoves.push_back(Position(i, piecePos.y));

		}
		else if(localBoard[i][piecePos.y] != nullptr && localBoard[i][piecePos.y] != nullptr)
			break;
		i++;
	}

	i = piecePos.y - 1;
	while (i >= 0)
	{
		if (!board.IsKingLeftInCheck(piecePos, { piecePos.x, i }, GetColor()))
		{
			if (localBoard[piecePos.x][i] != nullptr)
			{
				if (localBoard[piecePos.x][i]->GetColor() != pieceColor)
					possibleMoves.push_back(Position(piecePos.x, i));
				break;
			}
			else
				possibleMoves.push_back(Position(piecePos.x, i));
		}
		else if (localBoard[piecePos.x][i] != nullptr && localBoard[piecePos.x][i] != nullptr)
			break;
		i--;
	}

	i = piecePos.x - 1;
	while (i >= 0)
	{
		if (!board.IsKingLeftInCheck(piecePos, { i, piecePos.y }, GetColor()))
		{
			if (localBoard[i][piecePos.y] != nullptr)
			{
				if (localBoard[i][piecePos.y]->GetColor() != pieceColor)
					possibleMoves.push_back(Position(i, piecePos.y));
				break;
			}
			else
				possibleMoves.push_back(Position(i, piecePos.y));
		}
		else if (localBoard[i][piecePos.y] != nullptr && localBoard[i][piecePos.y] != nullptr)
			break;
		i--;
	}

	i = piecePos.y + 1;
	while (i < 8)
	{
		if (!board.IsKingLeftInCheck(piecePos, { piecePos.x, i }, GetColor()))
		{
		if (localBoard[piecePos.x][i] != nullptr)
		{
			if (localBoard[piecePos.x][i]->GetColor() != pieceColor)
				possibleMoves.push_back(Position(piecePos.x, i));
			break;
		}
		else
			possibleMoves.push_back(Position(piecePos.x, i));
		}
		else if (localBoard[piecePos.x][i] != nullptr && localBoard[piecePos.x][i] != nullptr)
			break;
		i++;
	}

	return possibleMoves;
}