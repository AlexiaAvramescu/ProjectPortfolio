module;
export module timer;

export import <chrono>;
import <thread>;
import <atomic>;
import <mutex>;
import <functional>;

using namespace std::chrono;
using namespace std::chrono_literals;

namespace skribbl
{
	export class Timer
	{
	public:
		using TimeConfig = milliseconds;

	public:
		Timer(const std::function<void()>& callbackEndTurn, const std::function<void()>& callbackHint);

		Timer(const Timer& other) = delete;
		Timer& operator=(const Timer& other) = delete;

		~Timer();

		void Start(uint8_t hintCount);
		void Pause();
		void Restart(uint8_t hintCount);
		void Stop();

		milliseconds GetElapsedTime() const;
		uint8_t GetRemainingTime() const;

		bool IsTimeUp() const;

	private:
		std::thread m_timerThread;
		std::condition_variable m_condition;
		std::mutex m_mutex;

		std::function<void()> m_handleTimeOut;
		std::function<void()> m_handleHint;

		uint8_t m_hintCount;

		std::atomic<bool> m_isRunning, m_isPaused;
		std::atomic<TimeConfig> m_elapsedTime;

		 const milliseconds kRefreshingRate{ 10 };
		 const TimeConfig kDuration{ 60s };
		 const seconds kFirstHintTime{ 30 };
	};
}