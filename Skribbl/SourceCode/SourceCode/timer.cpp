module timer;

using namespace skribbl;

Timer::Timer(const std::function<void()>& callbackEndTurn, const std::function<void()>& callbackHint)
	: m_isRunning{ false }
	, m_elapsedTime{ 0s }
	, m_handleTimeOut{ callbackEndTurn }
	, m_handleHint{ callbackHint }
{
	// empty
}

Timer::~Timer()
{
	Stop();
}

void Timer::Start(uint8_t hintCount)
{
	if (m_isRunning)
		return;

	m_hintCount = hintCount;
	m_isRunning = true;
	m_condition.notify_one();
	m_timerThread = std::thread([this]
		{
			while (m_isRunning)
			{
				time_point<steady_clock> startLock = steady_clock::now();
				std::unique_lock<std::mutex> lock(m_mutex);
				m_condition.wait_for(lock, kRefreshingRate, [&] {return !m_isRunning || m_isPaused; });
				time_point<steady_clock> endLock = steady_clock::now();


				if (IsTimeUp() & !m_isPaused)
				{
					m_isPaused = true;
					m_condition.notify_one();
					m_handleTimeOut();
				}

				if (duration_cast<seconds>(m_elapsedTime.load()) >= kFirstHintTime &&
					(m_elapsedTime.load() - duration_cast<milliseconds>(kFirstHintTime)) % duration_cast<milliseconds>((kFirstHintTime / (m_hintCount-1)) - seconds(0)) == 0s) {
					m_handleHint();
				}

				milliseconds timePassed = duration_cast<milliseconds>(endLock - startLock);
				m_elapsedTime.store(m_elapsedTime.load() + timePassed);
			}
		});
}

void Timer::Pause()
{
	if (m_isRunning && !m_isPaused) {
		m_isPaused = true;
		m_condition.notify_all();
	}
}

void Timer::Restart(uint8_t hintCount)
{
	m_hintCount = hintCount;
	m_elapsedTime.store(0s);
	m_isPaused = false;
	m_condition.notify_all();
}

void Timer::Stop()
{
	if (!m_isRunning)
		return; // exception?

	m_isRunning = false;
	m_condition.notify_one();
	if (m_timerThread.joinable())
		m_timerThread.join();
}

milliseconds Timer::GetElapsedTime() const
{
	return m_elapsedTime.load();
}

uint8_t Timer::GetRemainingTime() const
{
	auto millisecondsRemaining = kDuration - m_elapsedTime.load();
	auto secondsRemaining = duration_cast<seconds>(millisecondsRemaining);

	return static_cast<uint8_t>(secondsRemaining.count());
}

bool Timer::IsTimeUp() const
{
	return m_elapsedTime.load() >= kDuration;
}
