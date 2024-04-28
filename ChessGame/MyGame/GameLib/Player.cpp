#include "Player.h"

Player::Player(bool whiteSide)
{
    this->whiteSide = whiteSide;
}

bool Player::isWhiteSide()
{
    return this->whiteSide;
}