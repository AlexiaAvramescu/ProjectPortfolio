#pragma once

#include "IGame.h"
#include "Board.h"
#include "EGameState.h"
#include "EGameResult.h"
#include "PGNBuilder.h"
#include "ChessTimer.h"

using ConfigMatrix = std::vector<std::vector<char>>;
using ObserversList = std::vector<IGameListenerWeakPtr>;

class Game : public IGame, public IGameStatus 
{
public:
	Game();
	Game(EGameType type);
	Game(int turn, EGameState state, ConfigMatrix m);
	
	// subject methods
	void AddListener(IGameListenerPtr listener) override;
	void RemoveListener(IGameListener* listener) override;

	void ResetGame() override;

	void InitializeGameFEN(const std::string& strFEN) override;
	void InitializeGamePGN(const std::string& movesPGN) override;

	void LoadFromPGNFile(const std::string& filePath) override;
	void SaveToPGNFile(const std::string& filePath) override;

	Board GetBoard() const;
	
	// IGame methods
	EPlayer GetWinner() const override;
	EPlayer GetCurrentPlayer() const override;
	PositionList GetPossibleMoves(Position pos) const override;
	IPieceInfoPtrList GetCapturedPieces(EPieceColor color) const override;
	IPieceInfoPtr GetPieceInfo(Position pos) const override;
	int GetTime(EPlayer player) const override;

	EGameType GetGameType() const override;
	std::string GetFEN() const override;
	std::string GetPGN() const override;

	MoveList GetMovesPGN(const std::string& pgnStr) const override;
	std::string GetPGNMovesSection() const override;

	bool IsPlaying() const override;
	bool IsDrawProposed() const override;
	bool IsWaitingForPawnUpgrade() const override;
	bool IsDraw() const override;
	bool IsGameOver() const override;
	
	void PlayerComand(EComand respons) override;
	void MakeMove(Position startPos, Position endPos, bool isLoadingPGN = false) override;
	void UpgradePawnTo(EPieceType type) override;

	void NotifyMoveMade(Position startPos, Position endPos, PositionList prevPossibleMoves);
	void NotifyCaptureMade(EPieceColor color, IPieceInfoPtrList capturedPieces);
	void NotifyPawnUpgrade();
	void NotifyGameOver();
	void NotifyDraw();
	void NotifyPawnUpgradePGN();
	void NotifyUITimer();
	void NotifyPaused();

	void PauseGame();
	void ResumeGame();
	Position FindPieceStartPos(int startRow, int startCol, Position endPos, EPieceType type, bool turn) const;
	std::tuple<Position, Position, EPieceType> ConvertPGNMoveToInfoMove(std::string move, bool turn);

	const IGameStatus* GetStatus() const override;

private:
	// to move in PGNBuilder?
	MoveStr GeneratePGNMove(Position startPos, Position endPos);
	void UpdatePGN(Position startPos, Position endPos);
	void UpdatePGNUpgradePawn(EPieceType type);
	void UpdatePGNDraw();
	void UpdatePGNMate(const Board& board);
	void UpdatePGNCheck(const Board& board);

	bool CanUpgradePawn(Position pos) const;
	bool IsState(EGameState state) const;
	void UpdateState(EGameState state);

private:
	Board m_board;
	int m_turn;
	int m_moves;
	MoveStr m_pgn;
	FullMoveList m_pgnMovesVect;
	EGameState m_state;
	ObserversList m_observers;
	ChessTimer m_timer;
	PGNBuilder m_pgnBuilder;
};