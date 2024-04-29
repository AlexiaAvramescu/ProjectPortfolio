#pragma once
#include <crow.h>
#include <random>
#include <vector>
#include <unordered_map>
#include "IGame.h"
#include "Database.h"
#include <mutex>

import uniquerandom;

namespace skribbl
{
	class Routing
	{
	public:
		//Routing() = default;
		Routing(skribbl::Database& db);
		~Routing() = default;
		Routing(const Routing& other) = delete;
		Routing& operator=(const Routing& other) = delete;
		Routing(Routing&& other) noexcept = delete;
		Routing& operator=(Routing&& other) noexcept = delete;

		void Run();

		static constexpr uint8_t kmaxGamesSupported = 8;

	private:
		crow::response Login(const crow::request& req);
		crow::response Register(const crow::request& req);
		crow::response CreateLobby(const crow::request& req);
		crow::response JoinLobby(const crow::request& req);
		crow::response AddCustomWord(const crow::request& req);
		crow::response StartGame(const crow::request& req);
		crow::response ResetGame(const crow::request& req);
		crow::response RemovePlayer(const crow::request& req);
		crow::response ProcessAnswer(const crow::request& req);
		crow::response ProcessDrawing(const crow::request& req);
		crow::response GetWord(const crow::request& req);
		crow::response GetDrawing(const crow::request& req);
		crow::response GetAnswers(const crow::request& req);
		crow::response GetDrawingPlayer(const crow::request& req);
		crow::response GetGameState(const crow::request& req);
		crow::response GetTime(const crow::request& req);
		crow::response GetHint(const crow::request& req);
		crow::response GetGamePlayers(const crow::request& req);
		crow::response GetGameLeaderboard(const crow::request& req);

		crow::response GetGamesHistory(const crow::request& req) const;
		crow::response GetCustomWordsCount(const crow::request& req);

	private:
		crow::SimpleApp m_app;
		std::unordered_map<std::string, IGame::IGamePtr> m_games;
		skribbl::Database& m_db;
		UniqueRandom<std::string>::UniqueRandomPtr m_ur;
		std::mutex m_mutex;
	};
}