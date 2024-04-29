#pragma once

#include <cpr/cpr.h>
#include <crow.h>

#include "User.h"
#include "DrawingAreaWidget.h";

class Routing
{
public:
	Routing();
	~Routing() = default;
	Routing(const Routing& other) = delete;
	Routing& operator=(const Routing& other) = delete;
	Routing(Routing&& other) noexcept = delete;
	Routing& operator=(Routing&& other) noexcept = delete;

	bool SendLogin(const std::string& username, const std::string& password);
	bool SendRegister(const std::string& username, const std::string& password);
	bool SendCreateLobby(const std::string& username);
	bool SendJoinLobby(const std::string lobbyCode);
	void SendCustomWord(const std::string& word);
	bool SendStartGame();
	bool ResetGame() const;
	bool ExitGame();
	bool SendAnswer(const std::string& answer);
	void SendDrawing(const DrawingConfig& drawing);
	QString GetWord() const;
	DrawingConfig GetDrawing();
	std::vector<QString> GetAnswers();
	bool IsDrawingPlayer();
	QString GetRound() const;
	QString GetTime();
	std::vector<uint8_t> GetHint() const;
	std::vector<QString> GetPlayers();
	std::vector<std::pair<QString, int16_t>> GetLeaderBoard();
	std::vector<GameHistory> GetGamesHistory();
	uint8_t GetCustomWordsCount() const;
	
	std::string GetPlayerName() const;
	std::string GetLobbyCode() const;
	bool GetIsDrawing() const;

private:
	std::string m_url;
	std::string m_playerName;
	std::string m_lobbyCode;
	bool m_isDrawing;
};