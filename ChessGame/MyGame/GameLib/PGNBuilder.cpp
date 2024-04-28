#include "PGNBuilder.h"
#include <fstream>

PGNBuilder::PGNBuilder()
{
}

PGNBuilder::PGNBuilder(const std::string& playerWhite, const std::string& playerBlack, const std::string& location)
	: m_playerWhite(playerWhite)
	, m_playerBlack(playerBlack)
	, m_location(location)
{
	m_result = "*"; // Default result as "*" (unfinished game)
}

const std::string& PGNBuilder::GetInfoPGN(EInfoPGN infoType) const
{
	std::vector<std::string> InfoVect = { m_playerWhite, m_playerBlack, m_location, m_result };
	return InfoVect[(int)infoType];
}

void PGNBuilder::AddMove(const std::string& move)
{
	if(m_moveCount!=1)
		m_pgnStr += ' ';
	if (m_moveCount % 2 )
	{
		m_pgnStr += std::to_string(m_moveCount/2+1);
		m_pgnStr += ". ";
	}
	m_pgnStr += move;
	m_moveCount++;
}

void PGNBuilder::AddPawnUpgrade(char type)
{
	m_pgnStr += '=';
	m_pgnStr += type;
}

void PGNBuilder::AddCheck()
{
	m_pgnStr += '+';
}

void PGNBuilder::AddCheckMate(bool color)
{
	if (color)
		m_pgnStr += "# 1-0";
	else
		m_pgnStr += "# 0-1";
}

void PGNBuilder::SetResult(const std::string& result)
{
	m_result = result;
}

std::string PGNBuilder::GetPGN() const
{
	std::string pgn;

	// Add event tag
	pgn += "[Event \"Chess Game\"]\n";

	// Add players and location tags
	pgn += "[White \"" + m_playerWhite + "\"]\n";
	pgn += "[Black \"" + m_playerBlack + "\"]\n";
	pgn += "[Site \"" + m_location + "\"]\n";

	// Add result tag
	pgn += "[Result \"" + m_result + "\"]\n\n";

	pgn += m_pgnStr;

	return pgn;
}

std::string PGNBuilder::GetPGNMovesSection() const
{
	return m_pgnStr;
}

std::vector<std::string> PGNBuilder::GetMoves() const
{
	return m_moves;
}

void PGNBuilder::SavePGNToFile(const std::string& filePath)
{
	std::ofstream file(filePath);
	if (file.is_open()) {
		file << GetPGN();
		file.close();
	}
	else {
		throw CanNotOpenFileException();
	}
}

void PGNBuilder::Reset()
{
	m_playerWhite.clear();
	m_playerBlack.clear();
	m_location.clear();
	m_result.clear();
	m_pgnStr.clear();
	m_moves.clear();
	m_moveCount = 1;
}

std::vector<std::string> PGNBuilder::GenerateMoves(std::string pgnStr) const
{
	// Vector to store individual moves
	std::vector<std::string> moves;

	// Temporary string to hold a move while processing
	std::string currentMove;

	// Iterate through each character in the PGN string
	for (char c : pgnStr) {
		if (c == '+' || c == '#') {
			// Skip the '+' sign for check and '#' sign for mate
			continue;
		}
		else if (c == ' ' || c == '\n') {
			// If space or newline is encountered, the move is complete
			if (!currentMove.empty()) {
				// Remove the move number prefix (e.g., "1. ", "2. ", etc.)
				size_t prefixPos = currentMove.find('.');
				if (prefixPos != std::string::npos) {
					currentMove.erase(0, prefixPos + 1);
				}

				if (currentMove.size())
					moves.push_back(currentMove);
				currentMove.clear();
			}
		}
		else {
			// Add the character to the current move
			currentMove += c;
		}
	}

	// Add the last move if it exists
	if (!currentMove.empty()) {
		// Remove the move number prefix from the last move
		size_t prefixPos = currentMove.find('.');
		if (prefixPos != std::string::npos) {
			currentMove.erase(0, prefixPos + 1);
		}

		moves.push_back(currentMove);
	}
	return moves;
}

void PGNBuilder::SetPGNString(const std::string& pgnStr)
{
	m_pgnStr = pgnStr;
}

void PGNBuilder::LoadPGNFromFile(const std::string& filePath)
{
	std::ifstream file(filePath);
	if (file.is_open()) {
		std::string pgnString((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();
		//std::cout << "PGN loaded from: " << filePath << std::endl;
		for (int i = 0; i < pgnString.size(); i++)
		{
			if (i == 0 && pgnString[0] == '1')
			{
				m_pgnStr = pgnString;
			}
			if (pgnString[i] == '\n' && pgnString[i + 1] == '\n')
			{
				m_pgnStr = pgnString.substr(i + 2, pgnString.size() - i + 2);
				i = pgnString.size();
			}
		}
	}
	else {
		m_pgnStr = "";
		throw CanNotOpenFileException();
	}
}
