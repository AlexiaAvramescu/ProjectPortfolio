import player;
using skribbl::Player;

Player::Player()
	:m_username{ "-" },
	m_score{ 0 }
{
	// empty
}

Player::Player(const std::string& username)
	:m_username{ std::move(username) },
	m_score{ 0 }
{
	// empty
}

void Player::Reset()
{
	m_score = 0;
	m_guessed = false;
}

void Player::SetUsername(const std::string& username)
{
	m_username = std::move(username);
}

void Player::SetGuessed(bool guessed)
{
	m_guessed = guessed;
}

void Player::UpdateScore(int8_t score)
{
	m_score += score;
}

bool Player::HasGuessed() const
{
	return m_guessed;
}

const std::string& Player::GetUsername() const
{
	return m_username;
}

int16_t Player::GetScore() const
{
	return m_score;
}