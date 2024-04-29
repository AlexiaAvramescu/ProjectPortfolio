#pragma once
#include <memory>
#include <vector>
#include <string>
#include "Database.h"
#include "WordHandler.h"

import player;

namespace skribbl
{

	struct Answer {
		std::string playerName;
		std::string answer;
	};

	class IGame
	{
	public:

		using IGamePtr = std::unique_ptr<class IGame>;

		static IGamePtr Factory(skribbl::Database& db);

	public:
		virtual ~IGame() = default;

		virtual void Start() = 0;
		virtual void Restart() = 0;

		virtual void RemovePlayer(const std::string& name) = 0;
		virtual bool AddPlayer(const std::string& name) = 0;
		
		virtual void AddAnswer(const std::string& name, const std::string& answer) = 0;
		virtual void AddCustomWord(const std::string& word) = 0;
		virtual void SetDrawing(const std::string& drawing) = 0;
		
		virtual bool HasStarted() const = 0;
		virtual bool VerifyAnswer(const std::string& name, const std::string& answer) = 0;

		virtual const std::vector <Answer>& GetAnswers() const = 0;
		virtual const std::vector<uint8_t>& GetHint() const = 0;
		virtual const std::string& GetWord() const  = 0;
		virtual const std::string& GetDrawing() const = 0;
		virtual std::vector<std::pair<std::string, int16_t>> GetLeaderboard() const = 0;
		virtual std::vector<std::pair<std::string, int16_t>> GetPlayers() = 0;
		virtual std::string GetDrawingPlayer()const  = 0;
		virtual std::string GetState() const = 0;
		virtual uint8_t GetTime() const  = 0;
		virtual uint8_t GetCustomWordsCount() = 0;
	};
}