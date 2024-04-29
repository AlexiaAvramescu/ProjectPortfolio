#include <vector>
#include <string>
#include <memory>
#include "Database.h"
#include <ranges>
import uniquerandom;

namespace skribbl
{
	class WordHandler
	{
	public:
		using WordHandlerPtr = std::unique_ptr<WordHandler>;

	public:
		WordHandler(skribbl::Database& db);
		~WordHandler() = default; 
		WordHandler(const WordHandler& wg)=delete; 
		WordHandler& operator=(const WordHandler&) = delete;
		WordHandler(WordHandler&& wg)noexcept = default;
		WordHandler& operator=(WordHandler&&)noexcept = default;

		void Reset();
		void ResetToInitialState();
		
		void AddCustomWord(const std::string& word);
		void AddCustomWordToDatabase();

		bool AreCustomWordsLeft() const;
		const std::string& GetWord();
		const std::vector<uint8_t>& GenerateHint();

		uint8_t GetCustomWordsCount() const;


	private:
		std::string m_currentWord;
		UniqueRandom<int>::UniqueRandomPtr m_ur;
		UniqueRandom<int>::UniqueRandomPtr m_randomPositions;
		skribbl::Database& m_db;
		std::vector<std::string> m_customWords;
		std::vector<uint8_t> m_hint;
		int m_customWordPos;
	};
}