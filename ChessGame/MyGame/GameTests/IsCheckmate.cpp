#include "gtest/gtest.h"

#include "Board.h"

TEST(IsCheckmate, KingIsNotInCheck)
{
	Board board(0);
	board.SetPiece(Position(7, 7), EPieceColor::White, EPieceType::King);

	board.SetPiece(Position(3, 5), EPieceColor::Black, EPieceType::King);
	board.SetPiece(Position(2, 4), EPieceColor::White, EPieceType::Bishop);
	board.SetPiece(Position(5, 7), EPieceColor::White, EPieceType::Bishop);
	board.SetPiece(Position(2, 5), EPieceColor::White, EPieceType::Rook);
	board.SetPiece(Position(3, 4), EPieceColor::White, EPieceType::Pawn);
	board.SetPiece(Position(4, 4), EPieceColor::White, EPieceType::Pawn);
	board.SetPiece(Position(4, 5), EPieceColor::White, EPieceType::Pawn);
	board.SetPiece(Position(3, 6), EPieceColor::White, EPieceType::Pawn);

	auto ceva = board.IsCheckmate(EPieceColor::Black);
	EXPECT_FALSE(ceva);
}

TEST(IsCheckmate, KingIsNotInCheck1)
{
	Board board(0);
	board.SetPiece(Position(7,7), EPieceColor::White, EPieceType::King);

	board.SetPiece(Position(3, 5), EPieceColor::Black, EPieceType::King);
	board.SetPiece(Position(2, 4), EPieceColor::White, EPieceType::Bishop);
	board.SetPiece(Position(5, 7), EPieceColor::White, EPieceType::Bishop);
	board.SetPiece(Position(2, 5), EPieceColor::White, EPieceType::Rook);
	board.SetPiece(Position(4, 4), EPieceColor::White, EPieceType::Pawn);
	board.SetPiece(Position(4, 5), EPieceColor::White, EPieceType::Pawn);
	board.SetPiece(Position(3, 6), EPieceColor::White, EPieceType::Pawn);

	auto ceva = board.IsCheckmate(EPieceColor::Black);
	EXPECT_FALSE(ceva);
}

TEST(IsCheckmate, InvalidMove1) {

	ConfigMatrix m = { 
	{'-', '-', '-', '-', 'K', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', 'r', '-', '-', '-', '-', '-', '-'},
	{'b', '-', '-', '-', 'k', '-', '-', 'r'}
	};

	Board board(m);
	EXPECT_FALSE(board.IsCheckmate(EPieceColor::White));
}

TEST(IsCheckMate, BishopDownRightAttack)
{
	ConfigMatrix m = {
		/*0    1    2    3    4    5    6    7*/
		/*0*/	{'-', '-', '-', 'Q', 'K', 'B', '-', '-'},
		/*1*/	{'-', '-', '-', 'P', 'P', '-', '-', '-'},
		/*2*/	{'-', '-', '-', '-', '-', '-', '-', '-'},
		/*3*/	{'-', '-', '-', '-', '-', '-', '-', 'b'},
		/*4*/	{'-', '-', '-', '-', '-', '-', '-', '-'},
		/*5*/	{'-', '-', '-', '-', '-', '-', '-', '-'},
		/*6*/	{'-', '-', '-', '-', '-', '-', '-', '-'},
		/*7*/	{'-', '-', '-', 'k', '-', '-', '-', '-'}
	};

	Board board(m);

	EXPECT_TRUE(board.IsCheckmate(EPieceColor::Black));
}

TEST(IsCheckMate, BishopDownLeftAttack)
{
	ConfigMatrix m = {
				/*0    1    2    3    4    5    6    7*/
		/*0*/	{'-', '-', '-', 'B', 'K', 'B', '-', '-'},
		/*1*/	{'-', '-', '-', '-', 'P', 'P', '-', '-'},
		/*2*/	{'-', '-', '-', '-', '-', '-', '-', '-'},
		/*3*/	{'-', 'q', '-', '-', '-', '-', '-', '-'},
		/*4*/	{'-', '-', '-', '-', '-', '-', '-', '-'},
		/*5*/	{'-', '-', '-', '-', '-', '-', '-', '-'},
		/*6*/	{'-', '-', '-', '-', '-', '-', '-', '-'},
		/*7*/	{'-', '-', '-', 'k', '-', '-', '-', '-'}
	};


	Board board(m);

	EXPECT_TRUE(board.IsCheckmate(EPieceColor::Black));
}

TEST(IsCheckMate, BishopUpLeftAttack)
{
	ConfigMatrix m = {
		/*0    1    2    3    4    5    6    7*/
		/*0*/	{'-', '-', '-', 'B', 'K', 'B', '-', '-'},
		/*1*/	{'-', '-', '-', '-', 'P', 'P', '-', '-'},
		/*2*/	{'-', '-', '-', '-', '-', '-', '-', '-'},
		/*3*/	{'B', '-', '-', '-', '-', '-', '-', '-'},
		/*4*/	{'-', '-', '-', '-', '-', '-', '-', '-'},
		/*5*/	{'-', '-', '-', '-', '-', '-', '-', '-'},
		/*6*/	{'-', '-', '-', '-', 'p', 'p', '-', '-'},
		/*7*/	{'-', '-', '-', 'b', 'k', 'q', '-', '-'}
	};

	Board board(m);

	EXPECT_TRUE(board.IsCheckmate(EPieceColor::White));
}

TEST(IsCheckMate, BishopUpRightAttack)
{
	ConfigMatrix m = {
		/*0    1    2    3    4    5    6    7*/
		/*0*/	{'-', '-', '-', 'Q', 'K', 'B', '-', '-'},
		/*1*/	{'-', '-', '-', '-', 'P', 'P', '-', '-'},
		/*2*/	{'-', '-', '-', '-', '-', '-', '-', '-'},
		/*3*/	{'-', '-', '-', '-', '-', '-', '-', '-'},
		/*4*/	{'-', '-', '-', '-', '-', '-', '-', 'B'},
		/*5*/	{'-', '-', '-', '-', '-', '-', '-', '-'},
		/*6*/	{'-', '-', '-', 'p', 'p', '-', '-', '-'},
		/*7*/	{'-', '-', '-', 'b', 'k', 'b', '-', '-'}
	};

	Board board(m);
	EXPECT_TRUE(board.IsCheckmate(EPieceColor::White));
}