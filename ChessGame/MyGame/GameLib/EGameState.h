#pragma once

enum class EGameState
{
	Playing,
	DrawProposed,
	WhiteWon,
	BlackWon,
	Draw,
	WaitingForPawnUpgrade,
	Paused
};
