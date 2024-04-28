#include "gtest/gtest.h"

#include "Game.h"

TEST(FEN, Test1)
{
	ConfigMatrix m = {
	{'-', 'K', '-', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', 'b', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', 'n', '-', '-', '-'},
	{'-', '-', '-', 'k', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'},
	{'-', '-', 'B', '-', '-', '-', '-', '-'},
	{'-', '-', '-', '-', '-', '-', '-', '-'}
	};
	
	Game g(0, EGameState::Playing, m);
	
	auto str = g.GetFEN();
	std::string strFEN = "1k6/8/2B5/4N3/3K4/8/2b5/8 w QKqk";
	EXPECT_EQ(g.GetFEN(), strFEN);
}

TEST(FEN, Test2)
{

	ConfigMatrix m  = {
        {'r', 'n', '-', 'k', '-', '-', '-', 'r'},
        {'p', 'p', 'p', '-', 'p', 'p', 'p', 'p'},
        {'-', '-', '-', 'p', '-', '-', '-', '-'},
        {'-', '-', 'b', '-', 'b', 'n', '-', '-'},
        {'q', '-', 'P', '-', 'P', '-', 'P', 'N'},
        {'R', '-', '-', '-', '-', 'K', '-', '-'},
        {'P', '-', 'P', '-', 'P', 'P', 'P', '-'},
        {'-', '-', 'B', 'Q', '-', 'B', 'N', 'R'}
    };
	

	Game g(0, EGameState::Playing, m);

	auto str = g.GetFEN();
	std::string strFEN = "RN1K3R/PPP1PPPP/3P4/2B1BN2/Q1p1p1pn/r4k2/p1p1ppp1/2bq1bnr w QKqk";
	EXPECT_EQ(g.GetFEN(), strFEN);
}

TEST(FEN, Test3)
{

	ConfigMatrix m = {
		{'K', '-', '-', '-', '-', '-', '-', 'k'},
		{'-', '-', '-', 'R', '-', '-', '-', '-'},
		{'-', '-', '-', '-', '-', '-', 'n', '-'},
		{'Q', '-', '-', '-', 'p', '-', '-', '-'},
		{'-', 'p', 'P', '-', '-', '-', '-', '-'},
		{'-', '-', '-', 'N', '-', 'B', '-', '-'},
		{'-', '-', '-', '-', '-', '-', '-', '-'},
		{'-', 'r', '-', '-', '-', '-', 'r', '-'}
	};


	Game g(0, EGameState::Playing, m);

	auto str = g.GetFEN();
	std::string strFEN = "k6K/3r4/6N1/q3P3/1Pp5/3n1b2/8/1R4R1 w QKqk";
	EXPECT_EQ(g.GetFEN(), strFEN);
}

TEST(FEN, Test4)
{

	ConfigMatrix m = {
		{'R', '-', '-', '-', 'K', '-', '-', 'R'},
		{'-', '-', '-', 'R', '-', '-', '-', '-'},
		{'-', '-', '-', '-', '-', '-', 'n', '-'},
		{'Q', '-', '-', '-', 'p', '-', '-', '-'},
		{'-', 'p', 'P', '-', '-', '-', '-', '-'},
		{'-', '-', 'N', '-', '-', 'B', '-', '-'},
		{'-', '-', '-', '-', '-', '-', '-', '-'},
		{'r', '-', '-', '-', 'k', '-', '-', 'r'}
	};


	Game g(0, EGameState::Playing, m);

	g.MakeMove({ 7,0 }, { 6,0 });
	g.MakeMove({ 0,0 }, { 1,0 });
	

	auto str = g.GetFEN();
	std::string strFEN = "4k2r/r2r4/6N1/q3P3/1Pp5/2n2b2/R7/4K2R w -K-k";
	EXPECT_EQ(g.GetFEN(), strFEN);
}