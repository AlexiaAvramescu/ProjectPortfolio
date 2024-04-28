#include "gtest/gtest.h"

#include "Queen.h"
#include "Board.h"


class QueenTest : public testing::Test {
	void SetUp() override
	{
		queen = new Queen(EPieceColor::Black);
	}

	void TearDown() override
	{
		delete queen;
	}

protected:
	Queen* queen;
};

//vertical upwards


TEST_F(QueenTest, ValidMove1)
{
	Board board;

	Position startPos(4,3);
	Position endPos(2,5);
	board.SetPiece(startPos, EPieceColor::Black, EPieceType::Queen);

	EXPECT_TRUE(queen->CanMove(startPos, endPos, false, board));
}
//vertical downwards
TEST_F(QueenTest, ValidMove2)
{
	Board board;

	Position startPos(4, 6);
	Position endPos(6, 6);
	board.SetPiece(startPos, EPieceColor::Black, EPieceType::Queen);

	EXPECT_TRUE(queen->CanMove(startPos, endPos, false, board));
}

//diagonal right upwards
TEST_F(QueenTest, ValidMove3)
{
	Board board;

	Position startPos(4, 6);
	Position endPos(3,7);
	board.SetPiece(startPos, EPieceColor::Black, EPieceType::Queen);

	EXPECT_TRUE(queen->CanMove(startPos, endPos, false, board));
}


TEST_F(QueenTest, InvalidMove)
{
	Board board;

	Position startPos(7, 3);
	Position endPos(5, 5);
	board.SetPiece(startPos, EPieceColor::Black, EPieceType::Queen);

	EXPECT_FALSE(queen->CanMove(startPos, endPos, false, board));
}

TEST_F(QueenTest, InvalidMove2)
{
	Board board;

	Position startPos(7, 3);
	Position endPos(7, 5);
	board.SetPiece(startPos, EPieceColor::Black, EPieceType::Queen);

	EXPECT_FALSE(queen->CanMove(startPos, endPos, false, board));
}

TEST_F(QueenTest, InvalidMove3)
{
	Board board;

	Position startPos(4,6);
	Position endPos(7,3);
	board.SetPiece(startPos, EPieceColor::Black, EPieceType::Queen);

	EXPECT_FALSE(queen->CanMove(startPos, endPos, false, board));
}