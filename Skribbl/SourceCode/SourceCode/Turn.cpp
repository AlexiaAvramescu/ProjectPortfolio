module turn;
using skribbl::Turn;

Turn::Turn(const std::function<void()>& callbackEndTurn, const std::function<void()>& callbackHint)
{
	m_timer = std::make_unique<Timer>(callbackEndTurn, callbackHint);
}

Turn::Turn(Turn&& other) noexcept
	: m_answerTimestamps{ std::move(other.m_answerTimestamps) }
{
	// empty
}

Turn& Turn::operator=(Turn&& other) noexcept
{
	if (this != &other)
	{
		m_answerTimestamps = std::move(other.m_answerTimestamps);
	}
	return *this;
}

void Turn::Start()
{
	m_turnScore = 0;
	m_timer->Start(m_currentWord.size() / 2);
}

void Turn::Reset()
{
	m_turnScore = 0;
	m_timer->Restart(m_currentWord.size() / 2);
	m_answerTimestamps.clear();
}

void Turn::StopTimer()
{
	m_timer->Stop();
}

void Turn::SetCurrentWord(const std::string& word)
{
	m_currentWord = word;
}

bool Turn::IsOver() const
{
	return m_timer->IsTimeUp();
}

bool Turn::VerifyGuess(const std::string& guess)
{
	if (guess == m_currentWord)
	{
		std::chrono::seconds timeInSeconds = std::chrono::duration_cast<std::chrono::seconds>(m_timer->GetElapsedTime());
		uint8_t time = timeInSeconds.count();
		m_answerTimestamps.push_back(time);
		return true;
	}
	else
		return false;
}

int16_t Turn::GetGuessingPlayerScore()
{
	std::chrono::seconds timeInSeconds = std::chrono::duration_cast<std::chrono::seconds>(m_timer->GetElapsedTime());
	uint8_t time = timeInSeconds.count();

	int16_t score = time < 30 ? kMaxScore : (60 - time) * 100 / 30;
	m_turnScore += score;
	return score;
}

int16_t Turn::GetMinimGuessingScore() 
{
	int16_t score = (-1 * kMaxScore) / 2;
	m_turnScore += score;
	return score;
}

uint8_t Turn::AvrageAnswerTime() const
{
	return std::accumulate(m_answerTimestamps.begin(), m_answerTimestamps.end(), 0.0) / m_answerTimestamps.size();
}

int16_t Turn::GetDrawingPlayerScore()
{
	int16_t score;
	if (m_answerTimestamps.size() != 0)
	{
		uint8_t avrageTime = AvrageAnswerTime();
		score = (60 - avrageTime) * 100 / 60;
	}
	else
		score = (-1 * kMaxScore);
	m_turnScore += score;
	return score;
}

uint8_t Turn::GetRemainingTime() const
{
	return m_timer->GetRemainingTime();
}

int16_t Turn::GetTurnScore() const
{
	return m_turnScore;
}