#include "gtest/gtest.h"

#include "Bishop.h"
#include "Board.h"

class BishopTest : public testing::Test {

	void SetUp() override
	{
		bishop = new Bishop(EPieceColor::White);
	}

	void TearDown() override
	{
		delete bishop;
	}

protected:
	Bishop* bishop;

};

TEST_F(BishopTest, ValidMove) {
	Board board;
	std::vector<std::pair<Position, Position>> possibleMoves = { {Position(2,2), Position(5,5)}, {Position(4,3), Position(5,4)} };

	for (auto it : possibleMoves)
	{
		board.SetPiece(it.first, EPieceColor::White, EPieceType::Bishop);
		EXPECT_TRUE(bishop->CanMove(it.first, it.second, false, board));
		board.SetPieceToNullptr(it.first);
	}
}

TEST_F(BishopTest, InvalidMove) {
	Board board;
	std::vector<std::pair<Position, Position>> possibleMoves = { {Position(4,4), Position(7,7)}, {Position(2,2), Position(4,5)} };

	for (auto it : possibleMoves)
	{
		board.SetPiece(it.first, EPieceColor::White, EPieceType::Bishop);
		EXPECT_FALSE(bishop->CanMove(it.first, it.second, false, board));
		board.SetPieceToNullptr(it.first);
	}
}


TEST_F(BishopTest, InvalidMove1) {

	ConfigMatrix m = { {
	{'-', '-', '-', '-', 'K', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', 'r', '-', '-', '-', '-', '-', '-'},
	{'b', '-', '-', '-', 'k', '-', '-', 'r'}
	} };

	Board board(m);
	EXPECT_FALSE(bishop->CanMove(Position(7, 0), Position(6, 1), false, board));
}