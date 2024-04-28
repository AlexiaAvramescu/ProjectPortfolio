#include "gtest/gtest.h"   

#include "Pawn.h"
#include "Board.h"
#include "Piece.h"


//possible moves for bishop
TEST(PossibleMovesTest, ValidMovesPosibilitiesBishop) 
{
	Board board;
	board.SetPiece(Position(5, 5), EPieceColor::Black, EPieceType::Bishop);
	std::vector<Position> vector = { Position(4,6),Position(3,7), Position(6,6), Position(6,4),
		 Position(4,4),Position(3,3), Position(2,2) };
	EXPECT_EQ(board.GetBoard()[5][5]->GetPossibleMoves(Position(5, 5), false, board) , vector);
}

TEST(PossibleMovesTest, ValidMovesPosibilitiesBishop2)
{
	Board board;
	board.SetPiece(Position(5, 5), EPieceColor::Black, EPieceType::Bishop);
	board.SetPiece(Position(3, 7), EPieceColor::Black, EPieceType::Rook);
	std::vector<Position> vector = { Position(4,6), Position(6,6), Position(6,4),
		 Position(4,4),Position(3,3), Position(2,2) };
	EXPECT_EQ(board.GetBoard()[5][5]->GetPossibleMoves(Position(5, 5), false, board), vector);
}

TEST(PossibleMovesTest, ValidMovesPosibilitiesBishop3)
{
	Board board;
	board.SetPiece(Position(5, 5), EPieceColor::Black, EPieceType::Bishop);
	board.SetPiece(Position(3, 7), EPieceColor::White, EPieceType::Rook);
	std::vector<Position> vector = { Position(4,6),Position(3,7), Position(6,6), Position(6,4),
		 Position(4,4),Position(3,3), Position(2,2) };
	EXPECT_EQ(board.GetBoard()[5][5]->GetPossibleMoves(Position(5, 5), false, board), vector);
}

//possible moves for knight

TEST(PossibleMovesTest, ValidMovesPosibilitiesKnight)
{
	Board board;
	board.SetPiece(Position(4, 4), EPieceColor::White, EPieceType::Knight);
	std::vector<Position> vector = { Position(2,3), Position(2, 5), Position(3,2),Position(3,6),Position(5,2),Position(5,6)};
	EXPECT_EQ(board.GetBoard()[4][4]->GetPossibleMoves(Position(4, 4), false, board), vector);
}

TEST(PossibleMovesTest, ValidMovesPosibilitiesKnight2)
{
	Board board;
	board.SetPiece(Position(4, 4), EPieceColor::Black, EPieceType::Knight);
	std::vector<Position> vector = { Position(2, 3), Position(2, 5), Position(3,2),Position(3,6),Position(5,2),Position(5,6),
		Position(6,3),Position(6,5) };
	EXPECT_EQ(board.GetBoard()[4][4]->GetPossibleMoves(Position(4, 4), false, board), vector);
}

//knight in corner
TEST(PossibleMovesTest, ValidMovesPosibilitiesKnight3)
{
	Board board;
	board.SetPiece(Position(1, 1), EPieceColor::Black, EPieceType::Knight);
	std::vector<Position> vector = { Position(2, 3), Position(3,0), Position(3,2)};
	EXPECT_EQ(board.GetBoard()[1][1]->GetPossibleMoves(Position(1, 1), false, board), vector);
}

TEST(PossibleMovesTest, ValidMovesPosibilitiesKnight4)
{
	Board board;
	board.SetPiece(Position(1, 1), EPieceColor::White, EPieceType::Knight);
	std::vector<Position> vector = { Position(0, 3), Position(2, 3), Position(3,0), Position(3,2) };
	EXPECT_EQ(board.GetBoard()[1][1]->GetPossibleMoves(Position(1, 1), false, board), vector);
}

////on the right side
TEST(PossibleMovesTest, ValidMovesPosibilitiesKnight5)
{
	Board board;
	board.SetPiece(Position(3, 7), EPieceColor::White, EPieceType::Knight);
	std::vector<Position> vector = { Position(1, 6), Position(2,5),Position(4,5),Position(5,6) };
	EXPECT_EQ(board.GetBoard()[3][7]->GetPossibleMoves(Position(3, 7), false, board), vector);
}

TEST(PossibleMovesTest, ValidMovesPosibilitiesKnight6)
{
	Board board;
	board.SetPiece(Position(3, 7), EPieceColor::Black, EPieceType::Knight);
	std::vector<Position> vector = {Position(2,5),Position(4,5),Position(5,6) };
	EXPECT_EQ(board.GetBoard()[3][7]->GetPossibleMoves(Position(3, 7), false, board), vector);
}
////possible moves for king
TEST(PossibleMovesTest, ValidMovesPosibilitiesKing)
{
	Board board;
	board.SetPiece(Position(3, 7), EPieceColor::White, EPieceType::King);
	board.SetPiece(Position(2, 4), EPieceColor::Black, EPieceType::Rook);
	std::vector<Position> vector = {Position(3,6), Position(4,6), Position(4,7) };
	auto vect1 = board.GetBoard()[3][7]->GetPossibleMoves(Position(3, 7), false, board);
	EXPECT_EQ(board.GetBoard()[3][7]->GetPossibleMoves(Position(3, 7), false, board), vector);
}

TEST(PossibleMovesTest, ValidMovesPosibilitiesKing2)

{
	Board board;
	board.SetPiece(Position(5, 7), EPieceColor::White, EPieceType::King);
	std::vector<Position> vector = { Position(4,6) , Position(4,7) ,Position(5,6) };
	auto vect1 = board.GetBoard()[5][7]->GetPossibleMoves(Position(5, 7), false, board);
	EXPECT_EQ(board.GetBoard()[5][7]->GetPossibleMoves(Position(5, 7), false, board), vector);
}


TEST(PossibleMovesTest, ValidMovesPosibilitiesKing4)
{

	ConfigMatrix m = {
				/*0    1    2    3    4    5    6    7*/
		/*0*/	{'-', '-', '-', 'Q', 'K', 'B', '-', '-'},
		/*1*/	{'-', '-', '-', 'P', 'P', '-', '-', '-'},
		/*2*/	{'-', '-', '-', '-', '-', '-', '-', '-'},
		/*3*/	{'-', '-', '-', '-', '-', '-', '-', 'q'},
		/*4*/	{'-', '-', '-', '-', '-', '-', '-', '-'},
		/*5*/	{'-', '-', '-', '-', '-', '-', '-', '-'},
		/*6*/	{'-', '-', '-', '-', '-', '-', '-', '-'},
		/*7*/	{'-', '-', '-', 'k', '-', '-', '-', '-'}
	};

	Board board(m);

	std::vector<Position> vector = {};
	auto vect1 = board.GetBoard()[0][4]->GetPossibleMoves(Position(0, 4), false, board);
	EXPECT_EQ(board.GetBoard()[0][4]->GetPossibleMoves(Position(0, 4), false, board), vector);
}

TEST(PossibleMovesTest, ValidMovesPosibilitiesKing3)
{
	Board board;
	board.SetPiece(Position(3, 5), EPieceColor::Black, EPieceType::King);
	board.SetPiece(Position(2, 4), EPieceColor::White, EPieceType::Bishop);
	board.SetPiece(Position(5, 7), EPieceColor::White, EPieceType::Bishop);
	board.SetPiece(Position(2, 5), EPieceColor::White, EPieceType::Rook);
	board.SetPiece(Position(3, 4), EPieceColor::White, EPieceType::Pawn);
	board.SetPiece(Position(4, 4), EPieceColor::White, EPieceType::Pawn);
	board.SetPiece(Position(4, 5), EPieceColor::White, EPieceType::Pawn);
	board.SetPiece(Position(3, 6), EPieceColor::White, EPieceType::Pawn);
	std::vector<Position> vector = { Position(4, 4)};
	auto vect1 = board.GetBoard()[3][5]->GetPossibleMoves(Position(3, 5), false, board);
	//incorrect vect1 
	EXPECT_EQ(board.GetBoard()[3][5]->GetPossibleMoves(Position(3, 5), false, board), vector);
}

////possible moves for rook
TEST(PossibleMovesTest, ValidMovesPosibilitiesRook)
{
	Board board;
	board.SetPiece(Position(5, 5), EPieceColor::Black, EPieceType::Rook);
	std::vector<Position> vector = { Position(6,5), Position(5,4),Position(5,3),Position(5,2),Position(5,1),Position(5,0),
		Position(4,5),Position(3,5),Position(2,5) ,Position(5,6),Position(5,7) };
	EXPECT_EQ(board.GetBoard()[5][5]->GetPossibleMoves(Position(5, 5), false, board), vector);
}

////rook in corner
TEST(PossibleMovesTest, ValidMovesPosibilitiesRook1)
{
	Board board;
	board.SetPiece(Position(1, 1), EPieceColor::Black, EPieceType::Rook);
	std::vector<Position> vector = {Position(2, 1), Position(3,1), Position(4,1), Position(5, 1), Position(6,1) };
	std::vector<Position> vector1 = board.GetBoard()[1][1]->GetPossibleMoves(Position(1, 1), false, board);
	EXPECT_EQ(board.GetBoard()[1][1]->GetPossibleMoves(Position(1, 1), false, board), vector);
}

TEST(PossibleMovesTest, ValidMovesPosibilitiesRook2)
{
	Board board;
	board.SetPiece(Position(1, 1), EPieceColor::White, EPieceType::Rook);
	std::vector<Position> vector = {Position(2, 1), Position(3,1), Position(4,1), Position(5, 1), Position(1, 0), Position(0,1), Position(1,2) };
	std::vector<Position> vector1 = board.GetBoard()[1][1]->GetPossibleMoves(Position(1, 1), false, board);
	EXPECT_EQ(board.GetBoard()[1][1]->GetPossibleMoves(Position(1, 1), false, board), vector);
}

TEST(PossibleMovesTest, ValidMovesPosibilitiesRook3)
{
	ConfigMatrix m = {
	    /*0    1    2    3    4    5    6    7*/
  /*0*/	{'-', 'K', '-', '-', '-', '-', '-', '-'},
  /*1*/	{'-', '-', '-', '-', '-', '-', '-', '-'},
  /*2*/	{'-', '-', 'b', '-', '-', '-', '-', '-'},
  /*3*/	{'-', '-', '-', '-', 'n', '-', 'r', '-'},
  /*4*/	{'-', '-', '-', 'k', '-', '-', '-', '-'},
  /*5*/	{'-', '-', '-', '-', '-', '-', '-', '-'},
  /*6*/	{'-', '-', 'B', '-', '-', '-', '-', '-'},
  /*7*/	{'-', '-', '-', '-', '-', '-', '-', '-'}
	};

	Board board(m);
	std::vector<Position> vector = {Position(4,6),Position(5,6), Position(6,6), Position(7,6), Position(3, 5), Position(2,6), Position(1,6), Position(0,6), Position(3,7) };
	std::vector<Position> vector1 = board.GetBoard()[3][6]->GetPossibleMoves(Position(3, 6), false, board);
	EXPECT_EQ(board.GetBoard()[3][6]->GetPossibleMoves(Position(3, 6), false, board), vector);
}

////possible moves for queen
TEST(PossibleMovesTest, ValidMovesPosibilitiesQueen)
{
	Board board;
	board.SetPiece(Position(5, 1), EPieceColor::White, EPieceType::Queen);
	board.SetPiece(Position(5, 4), EPieceColor::White, EPieceType::Rook);
	board.SetPiece(Position(3,3), EPieceColor::Black, EPieceType::Bishop);

	std::vector<Position> vector = {Position(5, 0), Position(4, 1),Position(3, 1),Position(2, 1), Position(1, 1),Position(5, 2),Position(5, 3),
		Position(4,2),Position(3, 3), Position(4, 0) };
	auto ceva = board.GetBoard()[5][1]->GetPossibleMoves(Position(5, 1), false, board);
	EXPECT_EQ(board.GetBoard()[5][1]->GetPossibleMoves(Position(5, 1), false, board), vector);
}