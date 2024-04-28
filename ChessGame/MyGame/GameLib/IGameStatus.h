#pragma once

#include "EPieceColor.h"
#include "EPieceType.h"
#include "Position.h"
#include "EPlayer.h"
#include "IPieceInfo.h"
#include "TimeInfo.h"
#include "EGameType.h"

#include <string>

using MoveStr = std::string;
using MoveList = std::vector<MoveStr>;

class IGameStatus
{
public:
	virtual ~IGameStatus() = default;

	virtual bool IsPlaying() const = 0;
	virtual bool IsWaitingForPawnUpgrade() const = 0;
	virtual bool IsDrawProposed() const = 0;
	virtual bool IsDraw() const = 0;
	virtual bool IsGameOver() const = 0;

	virtual EPlayer GetWinner() const = 0;
	virtual EPlayer GetCurrentPlayer() const = 0;
	virtual IPieceInfoPtrList GetCapturedPieces(EPieceColor color) const = 0;
	virtual PositionList GetPossibleMoves(Position pos) const = 0;

	virtual EGameType GetGameType() const = 0;

	virtual int GetTime(EPlayer player) const = 0; 

	virtual std::string GetPGN() const = 0;
	virtual std::string GetFEN() const = 0;

	virtual MoveList GetMovesPGN(const std::string& pgnStr) const = 0;
	virtual std::string GetPGNMovesSection() const = 0;

	virtual IPieceInfoPtr GetPieceInfo(Position pos) const = 0;
};