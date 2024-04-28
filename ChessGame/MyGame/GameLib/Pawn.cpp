#include "Pawn.h"
#include "Board.h"


Pawn::Pawn(EPieceColor color)
	: Piece(EPieceType::Pawn, color)
{

}

bool Pawn::CanMove(Position startPos, Position endPos, bool isKingAttacking, const Board& board)
{
	PositionList possibleMoves;
	possibleMoves = GetPossibleMoves(startPos, isKingAttacking, board);
	if (std::find(possibleMoves.begin(), possibleMoves.end(), endPos) != possibleMoves.end())
		return true;

	return false;
}

PositionList Pawn::GetPossibleMoves(Position piecePos, bool isKingAttacking, const Board& board)
{
	PositionList possibleMoves;

	//check pawn threat
	if (GetColor() == EPieceColor::Black)
	{
		if (piecePos.x + 1 < 8)
		{
			if (board.GetBoard()[piecePos.x + 1][piecePos.y] == nullptr && !board.IsKingLeftInCheck(piecePos, { piecePos.x + 1, piecePos.y }, GetColor()))
				possibleMoves.push_back(Position(piecePos.x + 1, piecePos.y));

			if (piecePos.y + 1 < 8 && !board.IsKingLeftInCheck(piecePos, { piecePos.x + 1, piecePos.y + 1 }, GetColor()))
				if (board.GetBoard()[piecePos.x + 1][piecePos.y + 1] != nullptr && board.GetBoard()[piecePos.x + 1][piecePos.y + 1]->GetColor() != GetColor())
					possibleMoves.push_back(Position(piecePos.x + 1, piecePos.y + 1));

			if (piecePos.y - 1 >= 0 && !board.IsKingLeftInCheck(piecePos, { piecePos.x + 1, piecePos.y - 1 }, GetColor()))
				if (board.GetBoard()[piecePos.x + 1][piecePos.y - 1] != nullptr && board.GetBoard()[piecePos.x + 1][piecePos.y - 1]->GetColor() != GetColor())
					possibleMoves.push_back(Position(piecePos.x + 1, piecePos.y - 1));

			//verify for lines 2
			if (piecePos.x == 1 && !board.IsKingLeftInCheck(piecePos, { piecePos.x + 2, piecePos.y }, GetColor()))
			{
				if (board.GetBoard()[piecePos.x + 1][piecePos.y] == nullptr)
				{
					if (board.GetBoard()[piecePos.x + 2][piecePos.y] == nullptr)
						possibleMoves.push_back(Position(piecePos.x + 2, piecePos.y));
				}
			}
		}
	}

	if (GetColor() == EPieceColor::White)
	{
		if (piecePos.x - 1 >= 0)
		{
			if (board.GetBoard()[piecePos.x - 1][piecePos.y] == nullptr && !board.IsKingLeftInCheck(piecePos, { piecePos.x - 1, piecePos.y  }, GetColor()))
				possibleMoves.push_back(Position(piecePos.x - 1, piecePos.y));

			if (piecePos.y + 1 < 8 && !board.IsKingLeftInCheck(piecePos, { piecePos.x - 1, piecePos.y + 1 }, GetColor()))
				if (board.GetBoard()[piecePos.x - 1][piecePos.y + 1] && board.GetBoard()[piecePos.x - 1][piecePos.y + 1]->GetColor() != GetColor())
					possibleMoves.push_back(Position(piecePos.x - 1, piecePos.y + 1));

			if (piecePos.y - 1 >= 0 && !board.IsKingLeftInCheck(piecePos, { piecePos.x - 1, piecePos.y - 1 }, GetColor()))
				if (board.GetBoard()[piecePos.x - 1][piecePos.y - 1] && board.GetBoard()[piecePos.x - 1][piecePos.y - 1]->GetColor() != GetColor())
					possibleMoves.push_back(Position(piecePos.x - 1, piecePos.y - 1));

			//verify for lines 7
			if (piecePos.x == 6 && !board.IsKingLeftInCheck(piecePos, { piecePos.x + 2, piecePos.y }, GetColor()))
			{
				if (board.GetBoard()[piecePos.x - 1][piecePos.y] == nullptr)
				{
					if (board.GetBoard()[piecePos.x - 2][piecePos.y] == nullptr)
						possibleMoves.push_back(Position(piecePos.x - 2, piecePos.y));
				}
			}
		}
	}

	return possibleMoves;
}