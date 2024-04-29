#include "Routing.h"

#include <iostream>
#include <numeric>
#include <sstream>
#include <regex>
#include <string>
#include <QDebug>
#include <QString>

Routing::Routing()
	: m_url{ "http://localhost:18080" }
	, m_playerName{ "Not_Initialized" }
	, m_lobbyCode{ "" }
	, m_isDrawing{ false }
{
	// empty
}


bool Routing::SendLogin(const std::string& username, const std::string& password) {
	auto response = cpr::Put(
		cpr::Url{ m_url + "/login" },
		cpr::Parameters{
			{"username", username},
			{"password", password}
		}
	);

	if (response.status_code == 200 || response.status_code == 201) {
		m_playerName = response.text;
		return true;
	}

	return false;
}

bool Routing::SendRegister(const std::string& username, const std::string& password) {
	auto response = cpr::Put(
		cpr::Url{ m_url + "/register" },
		cpr::Parameters{
			{"username", username},
			{"password", password}
		}
	);

	if (response.status_code == 200 || response.status_code == 201) {
		m_playerName = response.text;
		return true;
	}

	return false;
}

bool Routing::SendCreateLobby(const std::string& username)
{
	auto response = cpr::Put(
		cpr::Url{ m_url + "/create_lobby" },
		cpr::Parameters{
			{"playerName", username}
		}
	);

	if (response.status_code == 200 || response.status_code == 201) {
		m_lobbyCode = response.text;
		return true;
	}

	return false;
}


bool Routing::SendJoinLobby(const std::string lobbyCode)
{
	auto response = cpr::Put(
		cpr::Url{ m_url + "/join_lobby" },
		cpr::Parameters{
			{"lobbyCode", lobbyCode},
			{"playerName", m_playerName}
		}
	);

	if (response.status_code == 200 || response.status_code == 201) {
		m_lobbyCode = lobbyCode;
		return true;
	}

	return false;
}

void Routing::SendCustomWord(const std::string& word)
{
	auto response = cpr::Put(
		cpr::Url{ m_url + "/custom_word" },
		cpr::Parameters{
			{ "lobbyCode", m_lobbyCode },
			{ "word", word }
		}
	);
}

bool Routing::SendStartGame()
{
	auto response = cpr::Put(
		cpr::Url{ m_url + "/start" },
		cpr::Parameters{
			{"lobbyCode", m_lobbyCode}
		}
	);

	if (response.status_code == 204)
		return true;
	return false;
}

bool Routing::ResetGame() const
{
	auto response = cpr::Put(
		cpr::Url{ m_url + "/reset" },
		cpr::Parameters{
			{ "lobbyCode",  m_lobbyCode}
		}
	);
	if (response.status_code == 204) {
		return true;
	}
	return false;
}

bool Routing::ExitGame()
{
	if (m_lobbyCode != "x") {

		auto response = cpr::Put(
			cpr::Url{ m_url + "/remove" },
			cpr::Parameters{
				{ "lobbyCode",  m_lobbyCode},
				{ "playerName", m_playerName },
			}
		);
		if (response.status_code == 204) {
			m_lobbyCode = "x";
			return true;
		}
	}
	return false;
}

bool Routing::SendAnswer(const std::string& answer)
{
	auto response = cpr::Put(
		cpr::Url{ m_url + "/send_answer" },
		cpr::Parameters{
			{ "lobbyCode",  m_lobbyCode},
			{ "playerName", m_playerName },
			{ "answer", answer }
		}
	);
	if (response.status_code == 200 || response.status_code == 201) {
		return true;
	}
	return false;
}

void Routing::SendDrawing(const DrawingConfig& drawing)
{
	std::string drawingStr;
	for (const auto& line : drawing)
	{
		for (const auto& point : line)
		{
			drawingStr.append(std::to_string(point.x()));
			drawingStr.append(" ");
			drawingStr.append(std::to_string(point.y()));
			drawingStr.append(" ");
		}
		drawingStr.append("/");
	}

	auto response = cpr::Put(
		cpr::Url{ m_url + "/send_drawing" },
		cpr::Parameters{
			{ "lobbyCode", m_lobbyCode },
			{ "drawing", drawingStr }
		}
	);
}

QString Routing::GetWord() const
{
	auto response = cpr::Get(
		cpr::Url{ m_url + "/get_word" },
		cpr::Parameters{
			{ "lobbyCode", m_lobbyCode },
			{ "playerName", m_playerName }
		}
	);
	return QString::fromLatin1(response.text.data());
}

DrawingConfig Routing::GetDrawing()
{
	auto response = cpr::Get(
		cpr::Url{ m_url + "/get_drawing" },
		cpr::Parameters{
			{ "lobbyCode", m_lobbyCode },
		}
	);

	auto answers = crow::json::load(response.text);

	DrawingConfig result;
	if (response.status_code == 404)
		return result;
	std::istringstream ss(response.text);
	std::string line;

	while (std::getline(ss, line, '/'))
	{
		std::vector<QPoint> points;
		std::istringstream lineStream(line);
		std::string pointX, pointY;

		while (lineStream >> pointX)
		{
			lineStream >> pointY;

			QPoint point(std::stoi(pointX), std::stoi(pointY));
			points.push_back(point);
		}

		result.push_back(points);
	}

	return std::move(result);
}

std::vector<QString> Routing::GetAnswers()
{
	cpr::Response response = cpr::Get(
		cpr::Url{ m_url + "/get_answers" },
		cpr::Parameters{ { {"lobbyCode"}, m_lobbyCode } }
	);

	if (response.status_code == 204)
		return std::vector<QString>();

	std::vector<QString> answerList;
	auto answers = crow::json::load(response.text);
	for (auto answer : answers)
	{
		std::string mess = std::string(answer["playerName"]);
		std::string message = mess + ": " + std::string(answer["answer"]);
		QString text = QString::fromLatin1(message.data());
		answerList.push_back(text);
	}
	return answerList;
}

bool Routing::IsDrawingPlayer()
{
	auto response = cpr::Get(
		cpr::Url{ m_url + "/drawing_player" },
		cpr::Parameters{
			{ "lobbyCode", m_lobbyCode },
		}
	);

	if (response.status_code == 200 || response.status_code == 201) {
		if (response.text == m_playerName) {
			m_isDrawing = true;
			return true;
		}
		else {
			m_isDrawing = false;
			return false;
		}
	}
}

QString Routing::GetRound() const
{
	auto response = cpr::Get(
		cpr::Url{ m_url + "/game_state" },
		cpr::Parameters{
			{ "lobbyCode", m_lobbyCode },
		}
	);
	if (response.status_code == 200)
		return QString::fromLatin1(response.text.data());
	return QString("No Server Answer");
}

QString Routing::GetTime()
{
	cpr::Response response = cpr::Get(
		cpr::Url{ m_url + "/get_time" },
		cpr::Parameters{ { {"lobbyCode"}, m_lobbyCode } }
	);

	return QString::fromLatin1(response.text.data());
}

std::vector<uint8_t> Routing::GetHint() const
{
	auto response = cpr::Get(
		cpr::Url{ m_url + "/get_hint" },
		cpr::Parameters{
			{ "lobbyCode", m_lobbyCode },
		}
	);
	std::vector<uint8_t> hints;
	
	for (const auto& el : response.text) {
		hints.push_back(static_cast<int>(el));
	}

	return hints;
}

std::vector<QString> Routing::GetPlayers()
{
	auto response = cpr::Get(
		cpr::Url{ m_url + "/players" },
		cpr::Parameters{
			{ "lobbyCode", m_lobbyCode },
		}
	);

	if (response.status_code != 200)
	{
		return std::vector<QString>();
	}

	std::vector<QString> players;
	auto jsonResponse = crow::json::load(response.text);
	for (const auto& playerName : jsonResponse)
	{
		QString player = QString::fromLatin1(std::string(playerName["playerName"]).data());
		players.push_back(player);
	}

	return players;
}

std::vector<std::pair<QString, int16_t>> Routing::GetLeaderBoard()
{
	auto response = cpr::Get(
		cpr::Url{ m_url + "/players" },
		cpr::Parameters{
			{ "lobbyCode", m_lobbyCode },
		}
	);

	if (response.status_code != 200)
	{
		return std::vector<std::pair<QString, int16_t>>();
	}

	std::vector<std::pair<QString, int16_t>> leaderBoard;
	auto jsonResponse = crow::json::load(response.text);
	for (const auto& playerInfo : jsonResponse)
	{
		QString playerName = QString::fromLatin1(std::string(playerInfo["playerName"]).data());
		int16_t score = playerInfo["score"].i();
		leaderBoard.emplace_back(playerName, score);
	}

	return leaderBoard;
}

std::vector<GameHistory> Routing::GetGamesHistory()
{
	auto response = cpr::Get(
		cpr::Url{ m_url + "/games_history" },
		cpr::Parameters{
			{"username", m_playerName}
		}
	);

	if (response.status_code != 200)
	{
		return std::vector<GameHistory>();
	}

	auto jsonResponse = crow::json::load(response.text);
	std::vector<GameHistory> gameHistory;
	for (const auto& history : jsonResponse)
	{
		std::string points = std::string(history["playerScore"]);
		std::string date = std::string(history["date"]);
		gameHistory.emplace_back(std::make_pair(points, date));
	}

	return gameHistory;
}

uint8_t Routing::GetCustomWordsCount() const
{
	auto response = cpr::Get(
		cpr::Url{ m_url + "/custom_words_count" },
		cpr::Parameters{
			{"lobbyCode", m_lobbyCode}
		}
	);

	if (response.status_code == 200)
	{
		auto jsonResponse = crow::json::load(response.text);
		if (jsonResponse)
		{
			auto numberCustomWords = jsonResponse["numberCustomWords"].u();
			return static_cast<uint8_t>(numberCustomWords);
		}
	}

	return 0;
}

std::string Routing::GetPlayerName() const
{
	return m_playerName;
}

std::string Routing::GetLobbyCode() const
{
	return m_lobbyCode;
}

bool Routing::GetIsDrawing() const
{
	return m_isDrawing;
}
