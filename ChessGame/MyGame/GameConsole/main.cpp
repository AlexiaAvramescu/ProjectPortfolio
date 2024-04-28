#include<iostream>
#include <iomanip>

#include "IGame.h"
#include "ChessExceptions.h"


std::string PieceToStr(IPieceInfoPtr pieceInfo)
{
	std::string result;

	switch (pieceInfo->GetType())
	{
	case EPieceType::Rook:
		result = "  R";
		break;
	case EPieceType::Bishop:
		result = "  B";
		break;
	case EPieceType::Knight:
		result = " Kn";
		break;
	case EPieceType::King:
		result = " Ki";
		break;
	case EPieceType::Queen:
		result = "  Q";
		break;
	case EPieceType::Pawn:
		result = "  P";
		break;
	}
	//if (pieceInfo->GetColor() == EPieceColor::White)
	//	result += "w ";
	//else
	//	result += "b ";
	return result;
}

std::string PlayerToStr(EPieceColor player)
{
	if (player == EPieceColor::White)
		return "PlayerWhite";
	else
		if (player == EPieceColor::Black)
			return "PlayerBlack";
	return "None";
}

void PrintBoard(const IGamePtr& game) {
	// ANSI color 
	const std::string resetColor = "\033[0m";
	const std::string blackColor = "\033[30m";
	const std::string lightTextColor = "\033[37m"; // Lighter text color for white pieces
	const std::string backgroundColor1 = "\033[48;2;205;133;63m"; // Lighter brown background for the light squares (RGB: 205, 133, 63)
	const std::string backgroundColor2 = "\033[48;2;139;69;19m"; // Darker brown background for the dark squares (RGB: 139, 69, 19)

	const int cellWidth = 5; // Width of each cell (including spaces)

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			auto pieceInfo = game->GetPieceInfo(i, j);

			//piece info or empty id there is nullptr
			std::string pieceStr = pieceInfo ? PieceToStr(pieceInfo) : "";

			// Determine the background color based on the square's position
			std::string bgColor = (i + j) % 2 == 0 ? backgroundColor1 : backgroundColor2;

			// Determine the text color based on the piece's color
			std::string textColor = pieceInfo && pieceInfo->GetColor() == EPieceColor::White ? lightTextColor : blackColor;

			// Print the cell with colored background and centered piece
			std::cout << bgColor << textColor << std::left << std::setw(cellWidth) << pieceStr << resetColor;
		}
		std::cout << "\n";
	}
}

bool IsComandDrawConsole(std::string comand)
{
	return comand == "DRAW";
}

int main()
{
	IGamePtr game = IGame::Produce(TODO);
	PrintBoard(game);
	std::cout << "\n";
	while (!game->IsGameOver())
	{
		std::string comand;
		std::cout << "Insert your move " << PlayerToStr(game->GetCurrentPlayer()) << ": ";
		std::getline(std::cin, comand);
		std::cout << "\n";

		try
		{
			if (game->IsPlaying())
			{
				game->PlayerDrawComand(comand);
			}
			else if (game->IsDrawProposed())
			{
				game->PlayerDrawComand(comand);
			}
			else if (game->IsWaitingForPawnUpgrade())
			{
				game->UpgradePawnTo(comand);
			}
			PrintBoard(game);
			std::cout << "\n";
		}
		catch (ChessExceptions e)
		{
			std::cout << e.what();
		}
	}
	//std::cout << "Winner is: " << PlayerToStr(game->GetWinner()) << " !!!";

	return 0;
}