#include "gtest/gtest.h"

#include "Pawn.h"
#include "Board.h"

class PawnTest : public testing::Test {

	void SetUp() override
	{
		pawn = new Pawn(EPieceColor::Black);
	}

	void TearDown() override
	{
		delete pawn;
	}

protected:
	Pawn* pawn;

};


////moving upwards 2 spots
TEST_F(PawnTest, InvalidMove) {
	Board board;

	Position startPos(7, 2);
	Position endPos(5, 2);
	board.SetPiece(startPos, EPieceColor::Black, EPieceType::Pawn);
	bool canMove = pawn->CanMove(startPos, endPos, false, board);

	EXPECT_FALSE(canMove);
}
//
////upwards diagonally
TEST_F(PawnTest, InvalidMove1) {
	Board board;

	Position startPos(3, 5);
	Position endPos(2, 6);
	board.SetPiece(startPos, EPieceColor::Black, EPieceType::Pawn);
	bool canMove = pawn->CanMove(startPos, endPos, false, board);

	EXPECT_FALSE(canMove);
}
//
TEST_F(PawnTest, InvalidMove2) {
	Board board;
	board.SetPiece(Position(5, 4), EPieceColor::White, EPieceType::King);
	Position startPos(3, 4);
	Position endPos(5, 4);
	board.SetPiece(startPos, EPieceColor::Black, EPieceType::Pawn);
	bool canMove = pawn->CanMove(startPos, endPos, false, board);

	EXPECT_FALSE(canMove);
}

//moving backwards vertically
TEST_F(PawnTest, ValidMove) {
	Board board;
	Position startPos(4, 3);
	Position endPos(5, 3);
	board.SetPiece(startPos, EPieceColor::Black, EPieceType::Pawn);
	bool canMove = pawn->CanMove(startPos, endPos, false, board);

	EXPECT_TRUE(canMove);
}

//moving backwards diagonally
TEST_F(PawnTest, ValidMove1) {
	Board board;
	Position startPos(6, 4);
	Position endPos(7, 5);
	board.SetPiece(startPos, EPieceColor::Black, EPieceType::Pawn);
	bool canMove = pawn->CanMove(startPos, endPos, false, board);

	EXPECT_TRUE(canMove);
}

