#include "King.h"
#include "Board.h"

King::King(EPieceColor color)
	: Piece(EPieceType::King, color)
{

}

bool King::VerifyKingMovmentCheck(Position startPos, Position endPos, const Board& board) const
{
	Board localBoard(board);
	EPieceColor kingColor = GetColor();

	localBoard.SetPiece(endPos, kingColor, EPieceType::King);
	localBoard.SetPieceToNullptr(startPos);
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			auto piece = localBoard.GetBoard()[i][j];
			if (piece && !piece->Is(kingColor) && !piece->Is(EPieceType::King) && piece->CanMove(Position(i, j), endPos, true, localBoard))
				return true;
		}
	}
	localBoard.GetBoard()[endPos.x][endPos.y] = nullptr;
	localBoard.SetPiece(startPos, kingColor, EPieceType::King);
	return false;
}

bool King::CanMakeBigCastling(EPieceColor color, const Board& board) const
{
	if (color == EPieceColor::White)
	{
		if (board.GetCastlingVect()[0][0])
		{
			//check if the path is clear
			for (int i = 3; i > 0; i--)
			{
				if (board.GetBoard()[7][i] != nullptr)
					return false;
			}

			//check if king is in check on the way to the rook
			for (int i = 4; i > 0; i--)
			{
				if (board.IsKingInCheck(Position(7, i), EPieceColor::White))
					return false;
			}
			return true;
		}
		return false;
	}
	else
	{
		if (board.GetCastlingVect()[1][0])
		{
			//check if the path is clear
			for (int i = 3; i > 0; i--)
			{
				if (board.GetBoard()[0][i] != nullptr)
					return false;
			}

			//check if king is in check on the way to the rook
			for (int i = 4; i > 1; i--)
			{
				if (board.IsKingInCheck(Position(0, i), EPieceColor::Black))
					return false;
			}
			return true;
		}
		return false;
	}
}

bool King::CanMakeSmallCastling(EPieceColor color, const Board& board) const
{
	if (color == EPieceColor::White)
	{
		if (board.GetCastlingVect()[0][1])
		{
			//check if the path is clear
			for (int i = 5; i < 7; i++)
			{
				if (board.GetBoard()[7][i] != nullptr)
					return false;
			}

			//check if king is in check on the way to the rook
			for (int i = 4; i < 7; i++)
			{
				if (board.IsKingInCheck(Position(7,i),EPieceColor::White))
					return false;
			}
			return true;
		}
		return false;
	}
	else
	{
		if (board.GetCastlingVect()[1][1])
		{
			//check if the path is clear
			for (int i = 5; i < 7; i++)
			{
				if (board.GetBoard()[0][i] != nullptr)
					return false;
			}

			//check if king is in check on the way to the rook
			for (int i = 4; i < 7; i++)
			{
				if (board.IsKingInCheck(Position(0, i), EPieceColor::Black))
					return false;
			}
			return true;
		}
		return false;
	}
}

bool King::CanMove(Position startPos, Position endPos, bool isKingAttacking, const Board& board)
{
	auto possibleMoves = GetPossibleMoves(startPos, isKingAttacking, board);
	if (std::find(possibleMoves.begin(), possibleMoves.end(), endPos) != possibleMoves.end())
		return true;

	return false;
}

PositionList King::GetPossibleMoves(Position piecePos, bool isKingAttacking, const Board& board)
{
	PositionList possibleMoves;

	for (int i = piecePos.x - 1; i <= piecePos.x + 1; i++)
	{
		for (int j = piecePos.y - 1; j <= piecePos.y + 1; j++)
		{
			if (i >= 0 && i < 8 && j < 8 && j >= 0)
			{
				auto piece = board.GetBoard()[i][j];
				if (piece && !piece->Is(GetColor()) && !VerifyKingMovmentCheck(piecePos, Position(i, j), board))
					possibleMoves.push_back(Position(i, j));
				if (!piece && !VerifyKingMovmentCheck(piecePos, Position(i, j), board))
					possibleMoves.push_back(Position(i, j));
			}
		}
	}

	if (!isKingAttacking)
	{
		auto piece = board.GetBoard()[piecePos.x][piecePos.y];
		auto color = piece->GetColor();
		if (CanMakeBigCastling(color, board))
			possibleMoves.push_back(Position(piecePos.x ,piecePos.y -2));
		if (CanMakeSmallCastling(color, board))
			possibleMoves.push_back(Position(piecePos.x, piecePos.y + 2));
	}

	return possibleMoves;
}

