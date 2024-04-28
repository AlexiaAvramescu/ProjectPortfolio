#include "Queen.h"

#include "Bishop.h"
#include "Rook.h"

#include "Board.h"

Queen::Queen(EPieceColor color) 
    : Piece(EPieceType::Queen,color)
{

}

bool Queen::CanMove(Position startPos, Position endPos, bool isKingAttacking, const Board& board)
{
	auto ceva = board.GetBoard()[startPos.x][startPos.y]->GetColor();
	Bishop bishop(ceva);
	Rook rook(board.GetBoard()[startPos.x][startPos.y]->GetColor());

	if (bishop.CanMove(startPos, endPos, isKingAttacking, board) == false && rook.CanMove(startPos, endPos,isKingAttacking, board) == false)
		return false;

	return true;
}

PositionList Queen::GetPossibleMoves(Position piecePos, bool isKingAttacking, const Board& board)
{
    PositionList possibleMoves;
	PositionList possibleMovesBishop;
	EPieceColor pieceColor = GetColor();
	auto localBoard = board.GetBoard();

	// rook direction
	Rook rook(pieceColor);
	Bishop bishop(pieceColor);
	possibleMoves = rook.GetPossibleMoves(piecePos, isKingAttacking, board);
	possibleMovesBishop = bishop.GetPossibleMoves(piecePos, isKingAttacking, board);
	possibleMoves.insert(possibleMoves.end(), possibleMovesBishop.begin(), possibleMovesBishop.end());

	return possibleMoves;
}