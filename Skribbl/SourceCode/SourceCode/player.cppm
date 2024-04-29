module;
export module player;

export import <string>;
export import <cstdint>;
export import <memory>;


namespace skribbl
{
	export class Player
	{
	public:
		using PlayerPtr = std::unique_ptr<Player>;

	public:
		Player();
		Player(const std::string& username);
		Player(const Player& player) = delete;
		Player& operator=(const Player& other) = delete;
		Player(Player&& player) noexcept = default;
		Player& operator=(Player&& player) noexcept = default;

		void Reset();

		void SetUsername(const std::string& username);
		void SetGuessed(bool guessed);
		void UpdateScore(int8_t score);
		
		bool HasGuessed() const;
		
		const std::string& GetUsername() const;
		int16_t GetScore() const;

	private:
		std::string m_username;
		int16_t m_score;
		bool m_guessed;
	};
}