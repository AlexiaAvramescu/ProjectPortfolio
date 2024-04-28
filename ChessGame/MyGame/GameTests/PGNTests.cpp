#include "gtest/gtest.h"

#include "Game.h"

std::string EliminatHeader(const std::string& pgn)
{
	std::string pgnMoves = pgn;

	for (int i = 1; i < pgnMoves.size(); i++)
	{
		if (pgnMoves[i - 1] == '\n' && pgnMoves[i] == '\n')
		{
			pgnMoves = pgnMoves.substr(i + 1, pgnMoves.size() - i + 2);
			break;
		}
	}

	return pgnMoves;
}

TEST(PGN, Test1)
{
	Game g;
	
	std::string strPgn = "1. d4 Nf6 2. h4 e6 3. h5 Ba3 4. h6 O-O 5. hxg7 e5 6. gxf8=N Ne8 7. Nxh7 f5 8. Be3 f4 9. Nc3 f3 10. Qd2 fxg2 11. O-O-O gxh1=Q 12. Ne4 Bb4 13. Nf3 a6 14. Neg5 a5 15. Ne4 a4 16. Nfg5 a3 17. Nf3 axb2+ 18. Kb1 Bc5 19. Nhg5 Bd6 20. c4 b5 21. Kc2 b1=B+ 22. Kc1 Bxe4 23. a3 Bb4 24. a4 d5 25. a5 Be6 26. Nh7 Qf6 27. a6 Qg6 28. a7 Qh4 29. axb8=Q Qhg4 30. Qa7 B4f5 31. Qa6 Be4 32. Qa5 B6f5 33. Qa4 Be6 34. Qa3 Q6f5 35. Qaa2 Qfg6 36. Qxb4 Q6h5 37. Qd6 Nf6 38. Qxe6+ Kh8 39. Qxf6+ Kg8 40. cxd5 b4 41. d6+ Kxh7 42. Rd3 b3 43. Rc3 b2+ 44. Kd2 b1=N+ 45. Ke1 c5 46. Qa6 Na3 47. Qxa8 Nc2+ 48. Rxc2 c4 49. Rxc4 Bb1 50. Rc7+ Qd7 51. Rxd7+ Qf7 52. Rxf7# 1-0";
	g.InitializeGamePGN(strPgn);

	std::string gotPgn = EliminatHeader(g.GetPGN());
	
	EXPECT_EQ(gotPgn, strPgn);
}

TEST(PGN, Test2)
{
	Game g;

	std::string strPgn = "1. e4 c5 2. Nc3 Nc6 3. g3 g6 4. Bg2 Bg7 5. f4 Bd4 6. Nge2 Bf2+ 7. Kxf2 e5 8. d3 Nd4 9. Be3 d6 10. Nd5 Bg4 11. Ndc3 Qf6 12. Bxd4 cxd4 13. Nd5 Qe6 14. h3 Bxe2 15. Qxe2 Rc8 16. Rhf1 exf4 17. Nxf4 Qe5 18. Kg1 Nf6 19. Rf2 Rf8 20. Raf1 Rc7 21. h4 Nh5 22. Nxh5 gxh5 23. Kh2 Re7 24. Rf5 Qg7 25. Qxh5 Kd7 26. Bh3 Rg8";
	g.InitializeGamePGN(strPgn);

	std::string gotPgn = EliminatHeader(g.GetPGN());
	EXPECT_EQ(gotPgn, strPgn);
}

TEST(PGN, Test3)
{
	Game g;

	std::string strPgn = "1. e4 e6 2. d4 d5 3. Nd2 Nf6 4. e5 Nfd7 5. f4 c5 6. c3 Nc6 7. Ndf3 cxd4 8. cxd4 f6 9. Bd3 Bb4+ 10. Bd2 Qb6";
	g.InitializeGamePGN(strPgn);

	std::string gotPgn = EliminatHeader(g.GetPGN());

	EXPECT_EQ(gotPgn, strPgn);
}

TEST(PGN, Test4)
{
	Game g;

	g.MakeMove({6,5}, {4,5});
	g.MakeMove({1,4}, {3,4});
	g.MakeMove({6,6}, {4,6});
	g.MakeMove({0,3}, {4,7});

	std::string strPgn = "1. f4 e5 2. g4 Qh4# 0-1";

	std::string gotPgn = EliminatHeader(g.GetPGN());

	EXPECT_EQ(gotPgn, strPgn);
}

TEST(PGN, Test5)
{
	Game g;

	std::string strPgn = "1. e4 e6 2. d4 d5 3. Nd2 Nf6 4. e5 Nfd7 5. f4 c5 6. c3 Nc6 7. Ndf3 cxd4 8. cxd4 f6 9. Bd3 Bb4+ 10. Bd2 Qb6 11. Ne2 fxe5 12. fxe5 O-O 13. a3 Be7 14. Qc2 Rxf3 15. gxf3 Nxd4 16. Nxd4 Qxd4 17. O-O-O Nxe5 18. Bxh7+ Kh8 19. Kb1 Qh4 20. Bc3 Bf6 21. f4 Nc4 22. Bxf6 Qxf6 23. Bd3 b5 24. Qe2 Bd7 25. Rhg1 Be8 26. Rde1 Bf7 27. Rg3 Rc8 28. Reg1 Nd6 29. Rxg7 Nf5 30. R7g5 Rc7 31. Bxf5 exf5 32. Rh5+";
	g.InitializeGamePGN(strPgn);

	std::string gotPgn = EliminatHeader(g.GetPGN());

	EXPECT_EQ(gotPgn, strPgn);
}
