module;
export module uniquerandom;
import <random>;
import <unordered_set>;
import <string>;

export template <typename T> class __declspec(dllexport) UniqueRandom
{
public:
	using UniqueRandomPtr = std::unique_ptr<UniqueRandom<T>>;

public:
	UniqueRandom<T>() {};
	UniqueRandom<T>(int number): m_mt(m_rand()) {
		m_dist = std::uniform_int_distribution<int>(0, number-1);
		Reset();
	};

	UniqueRandom<T>(int lnumber, int rnumber): m_mt(m_rand()) {
		m_dist = std::uniform_int_distribution<int>(lnumber, rnumber);
		Reset();
	}

	void ResetToInitialState()
	{
		m_usedNumber.clear();
		m_currentValue = 0;
	}

	void SetLimit(int number)
	{
		m_dist = std::uniform_int_distribution<int>(0, number - 1);
	}

	T GetValue() {
		int current = m_currentValue;
		Reset();
		if constexpr (std::is_same<T, std::string>::value) {
			return std::to_string(m_currentValue);
		}
		else if constexpr (std::is_same<T, int>::value)
			return m_currentValue;
	}

	void Reset() {
		do
		{
			m_currentValue = m_dist(m_mt);
		} while (m_usedNumber.find(m_currentValue) != m_usedNumber.end());

		m_usedNumber.emplace(m_currentValue);
	}

	void RemoveValue(int value)
	{
		m_usedNumber.erase(value);
	}
	
private:
	std::random_device m_rand;
	std::uniform_int_distribution<int> m_dist;
	std::mt19937 m_mt;
	std::unordered_set<size_t> m_usedNumber;
	size_t m_currentValue = 0;
};


template class UniqueRandom<int>;
template class UniqueRandom<std::string>;