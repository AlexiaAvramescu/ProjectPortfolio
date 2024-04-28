#include "ChessTimer.h"


using namespace std::chrono_literals;

ChessTimer::ChessTimer(EGameType type)
	: isTimerRunning(false)
	, isPaused(false)
	, currentPlayerTurn(0)
	, m_type(type)
{
	InitializeTimerDuration();

	if (type != EGameType::NoTimer)
		StartTimer();
}

ChessTimer::~ChessTimer()
{
	StopTimer();
}

void ChessTimer::StartTimer()
{
	if (!isTimerRunning)
	{
		isTimerRunning = true;
		m_condition.notify_one();
		m_timerThread = std::thread(&ChessTimer::TimerThread, this);
	}
}

void ChessTimer::StopTimer()
{
	if (isTimerRunning)
	{
		isTimerRunning = false;
		m_condition.notify_one();
		if (m_timerThread.joinable())
			m_timerThread.join();
	}
}

void ChessTimer::InitializeTimerDuration()
{
	switch (m_type)
	{
	case EGameType::Rapid:
		whiteTimerDuration = std::chrono::duration_cast<TimeConfig>(30min);
		blackTimerDuration = std::chrono::duration_cast<TimeConfig>(30min);
		break;
	case EGameType::Blitz:
		whiteTimerDuration = std::chrono::duration_cast<TimeConfig>(10min);
		blackTimerDuration = std::chrono::duration_cast<TimeConfig>(10min);
		break;
	case EGameType::Bullet:
		whiteTimerDuration = std::chrono::duration_cast<TimeConfig>(15s);
		blackTimerDuration = std::chrono::duration_cast<TimeConfig>(3min);
		break;
	case EGameType::NoTimer:
		break;
	}
}

void ChessTimer::PauseTimer()
{
	isPaused = true;
	m_condition.notify_one();
}

void ChessTimer::ResumeTimer()
{
	isPaused = false;
	m_condition.notify_one();
}

void ChessTimer::SetCallbackNotifyUI(Callback cb)
{
	notifyUI = cb;
}

void ChessTimer::SetCallbackNotifyGameOver(Callback cb)
{
	notifyGameOver = cb;
}

void ChessTimer::NotifyTimer()
{
	if (notifyUI)
		notifyUI();
}

void ChessTimer::NotifyGameOver()
{

	if (notifyGameOver)
		notifyGameOver();
}

void ChessTimer::UpdateTurn()
{
	currentPlayerTurn = !currentPlayerTurn;
}

//bool ChessTimer::IsTimerRunning() const
//{
//	return isTimerRunning;
//}

void ChessTimer::Reset()
{
	InitializeTimerDuration();
	isPaused = true;
	m_condition.notify_one();
	currentPlayerTurn = 0;
}

int ChessTimer::GetTimerDuration(EPlayer player) const
{
	return (int)player ? blackTimerDuration.load().count() : whiteTimerDuration.load().count();
}

EGameType ChessTimer::GetType() const
{
	return m_type;
}

bool ChessTimer::IsTimeOut() const
{
	return whiteTimerDuration.load().count() <= 0 || blackTimerDuration.load().count() <= 0;
}

void ChessTimer::TimerThread()
{
	auto currentTurn = currentPlayerTurn.load();
	auto startTime = std::chrono::steady_clock::now();

	while (isTimerRunning)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_condition.wait_for(lock, 1ms, [&]
			{
				bool ceva = (currentTurn != currentPlayerTurn.load()) || isPaused.load() || !isTimerRunning.load() || IsTimeOut();
				return (currentTurn != currentPlayerTurn.load()) || isPaused.load() || !isTimerRunning.load() || IsTimeOut();
			});

		if (IsTimeOut() && !isPaused)
		{
			isPaused = true;
			m_condition.notify_all();
			NotifyGameOver();
		}

		currentTurn = currentPlayerTurn.load();

		auto currentTime = std::chrono::steady_clock::now();
		auto elapsedTime = TimeConfig(std::chrono::duration_cast<TimeConfig>(currentTime - startTime));

		startTime = std::chrono::steady_clock::now();

		auto& duration = currentTurn ? blackTimerDuration : whiteTimerDuration;
		duration.store(duration.load() - elapsedTime);

		if (duration.load().count() < 0)
			duration.store(0s);


		NotifyTimer();
	}
}