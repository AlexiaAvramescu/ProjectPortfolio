module;
export module turn;

import timer;
import <vector>;
import <memory>;
import <numeric>;
import <functional>;

namespace skribbl
{
	export class Turn
	{
	public:
		using TurnPtr = std::unique_ptr<Turn>;

		static constexpr uint8_t kMaxScore = 100;

	public:
		Turn(const std::function<void()>& callbackEndTurn, const std::function<void()>& callbackHint);
		Turn(const Turn& other) = delete;
		Turn& operator=(const Turn& other) = delete;
		Turn(Turn&& other) noexcept;
		Turn& operator=(Turn&& other) noexcept;
		~Turn() = default;

		void Start();
		void Reset();
		void StopTimer();
		
		void SetCurrentWord(const std::string& word);
		
		bool IsOver() const;
		bool VerifyGuess(const std::string& guess);
		
		int16_t GetGuessingPlayerScore();
		int16_t GetMinimGuessingScore();
		int16_t GetDrawingPlayerScore();
		int16_t GetTurnScore() const;
		uint8_t AvrageAnswerTime() const;
		uint8_t GetRemainingTime() const;

	private:
		std::vector<uint8_t> m_answerTimestamps;
		std::string m_currentWord;
		std::unique_ptr<Timer> m_timer;
		int m_turnScore;
	};
}