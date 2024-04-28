#pragma once

#include "IGameStatus.h"
#include "IGameListener.h"
#include "EPieceType.h"
#include "EComand.h"
#include "Position.h"
#include "EGameType.h"


#include <memory>
#include <string>
#include <vector>
#include <utility>

using IGamePtr = std::shared_ptr<class IGame>;

using FullMove = std::pair<MoveStr, MoveStr>;
using FullMoveList = std::vector<FullMove>;

class IGame
{
public:
	static IGamePtr Produce(EGameType type);

	// subject methods
	virtual void AddListener(IGameListenerPtr listener) = 0;
	virtual void RemoveListener(IGameListener* listener) = 0;

	virtual void ResetGame() = 0;

	virtual ~IGame() = default;

	//Game Actions
	virtual void PlayerComand(EComand respons) = 0;
	virtual void MakeMove(Position startPos, Position endPos, bool isLoadingPGN = 0) = 0;
	virtual void UpgradePawnTo(EPieceType type) = 0;

	virtual void InitializeGameFEN(const std::string& strFEN) = 0;
	virtual void InitializeGamePGN(const std::string& movesPGN) = 0; 

	virtual void LoadFromPGNFile(const std::string& filePath) = 0;
	virtual void SaveToPGNFile(const std::string& filePath) = 0;

	//Game Status
	virtual const IGameStatus* GetStatus() const = 0;

};