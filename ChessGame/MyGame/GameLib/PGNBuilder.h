#include <iostream>
#include <string>
#include <vector>

#include "CanNotOpenFileException.h"

#include "EInfoPGN.h"
#include "EPieceType.h"

class PGNBuilder {
public:
	PGNBuilder();
	PGNBuilder(const std::string& playerWhite, const std::string& playerBlack, const std::string& location);

	// Getter methods
	const std::string& GetInfoPGN(EInfoPGN infoType) const;

	// Set the result of the game (e.g., "1-0" for white wins, "0-1" for black wins, "1/2-1/2" for draw)
	void SetResult(const std::string& result);
	void SetPGNString(const std::string& pgnStr);

	// Add a move to the moves vector
	void AddMove(const std::string& move);
	void AddPawnUpgrade(char type);
	void AddCheck();
	void AddCheckMate(bool color);

	// Generate the PGN file content as a string
	std::string GetPGN() const;
	std::string GetPGNMovesSection() const;
	std::vector<std::string> GetMoves() const;

	std::vector<std::string> GenerateMoves(std::string pgnStr) const;

	void SavePGNToFile(const std::string& filePath);
	void LoadPGNFromFile(const std::string& filePath);

	void Reset();

private:
	int m_moveCount = 1;
	std::string m_playerWhite;
	std::string m_playerBlack;
	std::string m_location;
	std::string m_result;
	std::string m_pgnStr;
	std::vector<std::string> m_moves;
};
