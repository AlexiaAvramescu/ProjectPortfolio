#include "Knight.h"
#include "Board.h"

Knight::Knight(EPieceColor color)
	:Piece(EPieceType::Knight, color)
{

}

bool Knight::CanMove(Position startPos, Position endPos, bool isKingAttacking, const Board& board)
{
	auto possibleMoves = GetPossibleMoves(startPos, isKingAttacking, board);
	if (std::find(possibleMoves.begin(), possibleMoves.end(), endPos) != possibleMoves.end())
		return true;

	return false;
}

PositionList Knight::GetPossibleMoves(Position piecePos, bool isKingAttacking, const Board& board)
{
	PositionList possibleMoves;

	for (int i = piecePos.x - 2; i <= piecePos.x + 2; i++)
		for (int j = piecePos.y - 2; j <= piecePos.y + 2; j++)
		{
			if (i < 8 && i >= 0 && j < 8 && j >= 0)
				if (abs(piecePos.x - i) == 2 && abs(piecePos.y - j) == 1 || abs(piecePos.x - i) == 1 && abs(piecePos.y - j) == 2)
				{
					if (!board.IsKingLeftInCheck(piecePos, { i, j }, GetColor()))
					{
						auto piece = board.GetBoard()[i][j];
						if (piece && !piece->Is( GetColor()))
							possibleMoves.push_back(Position(i, j));
						if (!piece)
							possibleMoves.push_back(Position(i, j));
					}
				}
		}
	return possibleMoves;
}