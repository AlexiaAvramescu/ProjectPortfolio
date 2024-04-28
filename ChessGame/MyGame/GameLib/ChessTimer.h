#pragma once

#include <thread>
#include <chrono>
#include <atomic>
#include "EPlayer.h"
#include "EGameType.h"
#include "TimeInfo.h"

#include <vector>
#include <functional>
#include <mutex>
#include <memory>


using Callback = std::function<void()>;
using TimeConfig = std::chrono::milliseconds;

class ChessTimer
{
public:
	ChessTimer(EGameType type);
	~ChessTimer();

	void StartTimer();

	void PauseTimer();
	void ResumeTimer();

	int GetTimerDuration(EPlayer player) const;
	EGameType GetType() const;

	bool IsTimeOut() const;

	void Reset();

	void SetCallbackNotifyUI(Callback cb);
	void SetCallbackNotifyGameOver(Callback cb);

	void UpdateTurn();

	//bool IsTimerRunning() const;


private:
	void StopTimer();

	void InitializeTimerDuration();

	void NotifyTimer();
	void NotifyGameOver();

	void TimerThread();

private:
	EGameType m_type;

	std::thread m_timerThread;

	std::atomic<bool> isTimerRunning;
	std::atomic<bool> currentPlayerTurn;
	std::atomic<bool> isPaused;

	std::atomic<TimeConfig> whiteTimerDuration;
	std::atomic<TimeConfig> blackTimerDuration;

	Callback notifyUI;
	Callback notifyGameOver;
	std::mutex m_mutex;
	std::condition_variable m_condition;
};
