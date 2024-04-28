#include "gtest/gtest.h"

#include "King.h"
#include "Board.h"

class KingTest : public testing::Test {

	void SetUp() override
	{
		king = new King(EPieceColor::White);
	}

	void TearDown() override
	{
		delete king;
	}

protected:
	King* king;

};

TEST_F(KingTest, InvalidMove) {
	Board board;
	board.SetPiece(Position(3, 4), EPieceColor::White, EPieceType::King);

	EXPECT_FALSE(king->CanMove(Position(3, 4), Position(2, 4), false, board));
}

TEST_F(KingTest, InvalidMove1) {
	Board board;
	std::vector<std::pair<Position, Position>> possibleMoves = { {Position(4,4), Position(2,5)}, {Position(4,4), Position(6,6)},{Position(3,6), Position(1,6)} , {Position(7,1), Position(7,2)} };

	for (auto it : possibleMoves)
	{
		board.SetPiece(it.first, EPieceColor::White, EPieceType::King);
		EXPECT_FALSE(king->CanMove(it.first, it.second, false, board));
		board.SetPieceToNullptr(it.first);
	}
}

TEST_F(KingTest, ValidMoveCheck2)
{
	Board board;
	board.SetPiece(Position(3, 2), EPieceColor::White, EPieceType::King);
	board.SetPiece(Position(4, 3), EPieceColor::Black, EPieceType::Queen);

	EXPECT_TRUE(king->CanMove(Position(3, 2), Position(4, 3), false, board));
}

TEST_F(KingTest, InValidMoveCheck) {
	Board board;
	board.SetPiece(Position(5, 5), EPieceColor::White, EPieceType::King);
	board.SetPiece(Position(3, 4), EPieceColor::Black, EPieceType::Bishop);
	board.SetPiece(Position(4, 4), EPieceColor::Black, EPieceType::Rook);

	EXPECT_FALSE(king->CanMove(Position(5, 5), Position(5, 4), false, board));
}

TEST_F(KingTest, InValidMoveCheck2) {
	Board board;
	board.SetPiece(Position(4, 6), EPieceColor::White, EPieceType::King);
	board.SetPiece(Position(2, 7), EPieceColor::White, EPieceType::Bishop);
	board.SetPiece(Position(3, 4), EPieceColor::Black, EPieceType::Rook);

	EXPECT_FALSE(king->CanMove(Position(4, 6), Position(3, 6), false, board));
}

TEST_F(KingTest, InValidMoveCheck3) {
	Board board;
	board.SetPiece(Position(5, 4), EPieceColor::White, EPieceType::King);
	board.SetPiece(Position(7, 5), EPieceColor::Black, EPieceType::Queen);
	board.SetPiece(Position(3, 4), EPieceColor::Black, EPieceType::Rook);
	board.SetPieceToNullptr(Position(6, 5));

	EXPECT_FALSE(king->CanMove(Position(5, 4), Position(5, 5), false, board));
}

TEST_F(KingTest, InValidMoveCheck4) {
	Board board;
	board.SetPiece(Position(3, 2), EPieceColor::White, EPieceType::King);
	board.SetPiece(Position(4, 3), EPieceColor::Black, EPieceType::Queen);
	board.SetPiece(Position(3, 5), EPieceColor::Black, EPieceType::Knight);

	EXPECT_FALSE(king->CanMove(Position(3, 2), Position(4, 3), false, board));
}

TEST_F(KingTest, ValidMoveCheck3) {
	Board board;
	board.SetPiece(Position(5, 5), EPieceColor::White, EPieceType::King);
	board.SetPiece(Position(3, 4), EPieceColor::Black, EPieceType::Bishop);
	board.SetPiece(Position(6, 6), EPieceColor::Black, EPieceType::Rook);

	EXPECT_TRUE(king->CanMove(Position(5, 5), Position(5, 4), false, board));
}

TEST_F(KingTest, ValidCastlingSmall) {
	Board board;
	board.SetPieceToNullptr(Position(7, 1));
	board.SetPieceToNullptr(Position(7, 2));
	board.SetPieceToNullptr(Position(7, 3));
	board.SetPieceToNullptr(Position(7, 5));
	board.SetPieceToNullptr(Position(7, 6));

	EXPECT_TRUE(king->CanMove(Position(7, 4), Position(7, 6), false, board));
}

TEST_F(KingTest, ValidCastlingBig) {
	Board board;
	board.SetPieceToNullptr(Position(7, 1));
	board.SetPieceToNullptr(Position(7, 2));
	board.SetPieceToNullptr(Position(7, 3));
	board.SetPieceToNullptr(Position(7, 5));
	board.SetPieceToNullptr(Position(7, 6));

	EXPECT_TRUE(king->CanMove(Position(7, 4), Position(7, 2), false, board));
}

TEST_F(KingTest, InvalidCastlingSmall) {
	Board board;
	board.SetPieceToNullptr(Position(7, 1));
	board.SetPieceToNullptr(Position(7, 2));
	board.SetPieceToNullptr(Position(7, 3));
	board.SetPieceToNullptr(Position(7, 5));
	board.SetPieceToNullptr(Position(7, 6));
	board.SetPieceToNullptr(Position(6, 5));
	board.SetPiece(Position(4, 5), EPieceColor::Black, EPieceType::Queen);
	EXPECT_FALSE(king->CanMove(Position(7, 4), Position(7, 6), false, board));
}

TEST_F(KingTest, InvalidCastlingSmall1) {
	Board board;
	board.SetPieceToNullptr(Position(7, 1));
	board.SetPieceToNullptr(Position(7, 2));
	board.SetPieceToNullptr(Position(7, 3));
	board.SetPieceToNullptr(Position(7, 5));
	board.SetPieceToNullptr(Position(7, 6));
	board.SetPieceToNullptr(Position(6, 4));
	board.SetPiece(Position(4, 4), EPieceColor::Black, EPieceType::Queen);
	EXPECT_FALSE(king->CanMove(Position(7, 4), Position(7, 6), false, board));
}

TEST_F(KingTest, InvalidCastlingBig) {
	Board board;
	board.SetPieceToNullptr(Position(7, 1));
	board.SetPieceToNullptr(Position(7, 2));
	board.SetPieceToNullptr(Position(7, 3));
	board.SetPieceToNullptr(Position(7, 5));
	board.SetPieceToNullptr(Position(7, 6));
	board.SetPieceToNullptr(Position(6, 2));
	board.SetPiece(Position(4, 2), EPieceColor::Black, EPieceType::Queen);
	EXPECT_FALSE(king->CanMove(Position(7, 4), Position(7, 2), false, board));
}


TEST_F(KingTest, Castle1)
{
	ConfigMatrix m = { {
	{'-', '-', '-', '-', 'K', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'r', '-', '-', '-', 'k', '-', '-', 'r'}
	} };

	Board board(m);
	EXPECT_TRUE(king->CanMove(Position(7, 4), Position(7, 6), false, board));
}

TEST_F(KingTest, Castle3)
{
	ConfigMatrix m = { {
	{'-', '-', '-', '-', 'K', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'r', '-', '-', '-', 'k', '-', '-', 'r'}
	} };

	Board board(m);
	EXPECT_TRUE(king->CanMove(Position(7,4), Position(7, 2), false, board));
}

TEST_F(KingTest, Castle4)
{
	ConfigMatrix m = { {
	{'R', '-', '-', '-', 'K', '-', '-', 'R'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'r', '-', '-', '-', 'k', '-', '-', 'r'}
	} };

	Board board(m);
	EXPECT_TRUE(king->CanMove(Position(7, 4), Position(7,2), false, board));
}

TEST_F(KingTest, Castle5)
{
	ConfigMatrix m = { {
	{'-', '-', '-', '-', 'K', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'r', '-', '-', '-', 'k', 'b', '-', 'r'}
	} };

	Board board(m);
	EXPECT_FALSE(king->CanMove(Position(7, 4), Position(7, 6), false, board));
}

TEST_F(KingTest, Castle6)
{
	ConfigMatrix m = { {
	{'-', '-', '-', '-', 'K', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', 'Q', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'r', '-', '-', '-', 'k', '-', '-', 'r'}
	} };

	Board board(m);
	EXPECT_FALSE(king->CanMove(Position(7, 4), Position(7, 6), false, board));
}

TEST_F(KingTest, Castle7)
{
	ConfigMatrix m = { {
	{'-', '-', '-', '-', 'K', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', 'K', '-', '-', '-', '-'},
	{'-', '-', '-', '-', 'k', '-', '-', '-'}
	} };

	Board board(m);
	EXPECT_FALSE(king->CanMove(Position(7, 4), Position(7, 6), false, board));
}