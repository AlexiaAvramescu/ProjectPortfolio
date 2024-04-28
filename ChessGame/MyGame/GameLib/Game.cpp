
#include "Game.h"
#include "PieceInfo.h"

#include "ChessExceptions.h"
#include "OutOfBoundException.h"
#include "InvalidDrawResponseException.h"
#include "InvalidUpgradeType.h"
#include "InvalidStartPositionExcepton.h"
#include "NotStateDrawProposedException.h"
#include "NotStateWaitingForPawnUpdate.h"
#include "NotStatePlayingException.h"
#include "NotStatePausedException.h"

#include <tuple>

Game::Game()
	: m_turn(0)
	, m_state(EGameState::Playing)
	, m_moves(1)
	, m_timer(EGameType::NoTimer)
{}

Game::Game(EGameType type)
	: m_turn(0)
	, m_state(EGameState::Playing)
	, m_moves(1)
	, m_timer(type)
{
	m_timer.SetCallbackNotifyUI(std::bind(&Game::NotifyUITimer, this));
	m_timer.SetCallbackNotifyGameOver(std::bind(&Game::NotifyGameOver, this));
}

Game::Game(int turn, EGameState state, ConfigMatrix m)
	: m_state(state)
	, m_turn(turn)
	, m_board(m)
	, m_moves(1)
	, m_timer(EGameType::NoTimer)
{
	if (m_turn)
		m_pgnMovesVect.push_back({ "-", "-" });
}

IGamePtr IGame::Produce(EGameType type)
{
	return std::make_shared<Game>(type);
}

Board Game::GetBoard() const
{
	return m_board;
}

EPlayer Game::GetWinner() const
{
	if (m_state == EGameState::BlackWon)
		return EPlayer::Black;
	if (m_state == EGameState::WhiteWon)
		return EPlayer::White;

	return EPlayer::None;
}

bool Game::IsGameOver() const
{
	return m_state == EGameState::WhiteWon || m_state == EGameState::BlackWon || m_state == EGameState::Draw;
}

static char PieceTypeToChar(EPieceType type)
{
	std::string TYPES = "PRNBQK";
	return TYPES[(int)type];
}

MoveStr Game::GeneratePGNMove(Position startPos, Position endPos)
{
	MoveStr pgnMove;

	//valid pos of the second rook or knight if it can move the same
	Position otherPiecePos = m_board.CanTheOtherPieceMove(startPos, endPos);

	// simulate the move to see if it can be done
	auto piece = GetPieceInfo(startPos);
	auto endPosPiece = GetPieceInfo(endPos);
	Board board(m_board);
	board.MakeMove(startPos, endPos);

	if (piece->GetType() == EPieceType::King)
	{
		// symbol for castling
		if (startPos.y - endPos.y == 2)
		{
			pgnMove += "O-O-O";
			return pgnMove;
		}
		if (startPos.y - endPos.y == -2)
		{
			pgnMove += "O-O";
			return pgnMove;
		}
	}
	if (piece->GetType() != EPieceType::Pawn)
	{
		pgnMove += PieceTypeToChar(piece->GetType());
		//verify if the second rook or knight can make the same move, add specifications
		if (otherPiecePos.y == startPos.y)
			pgnMove += '0' + (8 - startPos.x);
		else if (otherPiecePos.y != -1)
			pgnMove += 'a' + startPos.y;

		if (endPosPiece)
			pgnMove += 'x';
	}
	else
	{
		// pawn move representation
		if (endPosPiece)
		{
			pgnMove += 'a' + startPos.y;
			pgnMove += 'x';
		}
	}
	// end position representation
	pgnMove += 'a' + endPos.y;
	pgnMove += '0' + (8 - endPos.x);


	return pgnMove;
}

void Game::UpdatePGN(Position startPos, Position endPos)
{
	if (m_turn)
	{
		if (!m_pgnMovesVect.size())
			m_pgnMovesVect.push_back({ {" "}, {}});
		m_pgnMovesVect[m_pgnMovesVect.size() - 1].second = GeneratePGNMove(startPos, endPos);
	}
	else
	{
		m_pgnMovesVect.push_back({ GeneratePGNMove(startPos,	endPos), {} });
	}

	m_pgnBuilder.AddMove(GeneratePGNMove(startPos, endPos));

	////copy board to simulate the move
	//Board board(m_board);
	//board.MakeMove(startPos, endPos);
	//UpdatePGNCheckOrMate(board);
}

void Game::UpdatePGNUpgradePawn(EPieceType type)
{
	if (m_turn)
	{
		m_pgnMovesVect[m_pgnMovesVect.size() - 1].second += '=';
		m_pgnMovesVect[m_pgnMovesVect.size() - 1].second += PieceTypeToChar(type);
	}
	else
	{
		m_pgnMovesVect[m_pgnMovesVect.size() - 1].first += '=';
		m_pgnMovesVect[m_pgnMovesVect.size() - 1].first += PieceTypeToChar(type);
	}
	m_pgnBuilder.AddPawnUpgrade(PieceTypeToChar(type));
	UpdatePGNMate(m_board);
}

void Game::UpdatePGNDraw()
{
	if (m_turn && m_pgnMovesVect.size())
	{

		m_pgnMovesVect[m_pgnMovesVect.size() - 1].second = "1/2-1/2";
	}
	else
		m_pgnMovesVect.push_back({ "1/2-1/2" ,{} });

	m_pgnBuilder.AddMove("1/2-1/2");
}

void Game::UpdatePGNMate(const Board& board)
{
	auto colorUpdated = m_turn ? EPieceColor::Black : EPieceColor::White;
	Position kingPos = board.GetKingPos(colorUpdated);

	if (!board.IsCheckmate(colorUpdated))
		return;

	if (!(int)colorUpdated)
	{
		m_pgnMovesVect[m_pgnMovesVect.size() - 1].second += '#';
		m_pgnMovesVect.push_back({ "0-1", {} });
	}
	else
	{
		m_pgnMovesVect[m_pgnMovesVect.size() - 1].first += '#';
		m_pgnMovesVect[m_pgnMovesVect.size() - 1].second += "1-0";
	}
	m_pgnBuilder.AddCheckMate((int)colorUpdated);
}

void Game::UpdatePGNCheck(const Board& board)
{
	auto colorUpdated = m_turn ? EPieceColor::Black : EPieceColor::White;
	Position kingPos = board.GetKingPos(colorUpdated);

	if (!board.IsKingInCheck(kingPos, colorUpdated))
		return;
	if (!(int)colorUpdated)
		m_pgnMovesVect[m_pgnMovesVect.size() - 1].second += '+';
	else
		m_pgnMovesVect[m_pgnMovesVect.size() - 1].first += '+';
	m_pgnBuilder.AddCheck();

}

bool Game::CanUpgradePawn(Position pos) const
{
	return m_board.IsUpgradeablePawn(pos);
}

void Game::MakeMove(Position startPos, Position endPos, bool isLoadingPGN)
{
	if (m_state != EGameState::Playing)
		throw NotStatePlayingException();

	auto color = m_turn ? EPieceColor::Black : EPieceColor::White;

	if (!startPos.IsValid() || !endPos.IsValid())
		throw OutOfBoundException();

	if (!m_board.IsPieceColor(startPos, color))
		throw InvalidStartPositionExcepton();

	UpdatePGN(startPos, endPos);

	auto prevPossibleMoves = m_board.GetPossibleMoves(startPos);
	auto endPosPiece = GetPieceInfo(endPos);

	if (m_board.MakeMove(startPos, endPos))
		if (endPosPiece)
		{
			auto capturedColor = endPosPiece->GetColor();
			m_board.AddCapturedPiece(endPosPiece);
			NotifyCaptureMade(capturedColor, GetCapturedPieces(capturedColor));
		}

	if (CanUpgradePawn(endPos))
	{
		UpdateState(EGameState::WaitingForPawnUpgrade);
		if (!isLoadingPGN)
			NotifyPawnUpgrade();
		//else
			//build pgn for load?
	}
	else
	{
		m_turn = 1 - m_turn;
		m_timer.UpdateTurn();
		NotifyMoveMade(startPos, endPos, prevPossibleMoves);
	}


	if (m_board.IsStaleMate(color) || m_board.IsThreefoldRepetitionDraw() || m_board.IsInsufficientMaterial())
	{
		UpdateState(EGameState::Draw);
		m_timer.PauseTimer();
		UpdatePGNDraw();
		NotifyGameOver();
	}

	auto colorUpdated = m_turn ? EPieceColor::Black : EPieceColor::White;
	auto kingPos = m_board.GetKingPos(colorUpdated);

	if (m_board.IsCheckmate(colorUpdated))
	{
		UpdateState(colorUpdated == EPieceColor::White ? EGameState::BlackWon : EGameState::WhiteWon);
		m_timer.PauseTimer();
		UpdatePGNMate(m_board);
		NotifyGameOver();
	}
	else if (m_board.IsKingInCheck(kingPos, colorUpdated))
		UpdatePGNCheck(m_board);
}

void Game::ResetGame()
{
	m_turn = 0;
	m_pgn = {};
	m_pgnBuilder.Reset();
	m_pgnMovesVect.clear();
	m_moves = 1;
	UpdateState(EGameState::Playing);
	m_board.ResetBoard();
	m_timer.Reset();
	m_timer.ResumeTimer();
}

void Game::NotifyMoveMade(Position startPos, Position endPos, PositionList prevPossibleMoves)
{
	for (auto it = m_observers.begin(); it != m_observers.end();)
	{
		if (auto sp = it->lock())
		{
			sp->OnMoveMade(startPos, endPos, prevPossibleMoves);
			++it;
		}
		else
			it = m_observers.erase(it);
	}
}

void Game::NotifyCaptureMade(EPieceColor color, IPieceInfoPtrList capturedPieces)
{
	for (auto it = m_observers.begin(); it != m_observers.end();)
	{
		if (auto sp = it->lock())
		{
			sp->OnCaptureMade(color, capturedPieces);
			++it;
		}
		else
			it = m_observers.erase(it);
	}
}

void Game::NotifyPawnUpgrade()
{
	for (auto it = m_observers.begin(); it != m_observers.end();)
	{
		if (auto sp = it->lock())
		{
			sp->OnPawnUpgrade();
			++it;
		}
		else
			it = m_observers.erase(it);
	}
}

void Game::NotifyGameOver()
{
	if (m_timer.IsTimeOut())
	{
		if (m_timer.GetTimerDuration(EPlayer::White) <= 0)
			UpdateState(EGameState::BlackWon);
		else
			UpdateState(EGameState::BlackWon);
		//m_timer.PauseTimer();
	}

	for (auto it = m_observers.begin(); it != m_observers.end();)
	{
		if (auto sp = it->lock())
		{
			sp->OnGameOver();
			++it;
		}
		else
			it = m_observers.erase(it);
	}
}

void Game::NotifyDraw()
{
	for (auto it = m_observers.begin(); it != m_observers.end();)
	{
		if (auto sp = it->lock())
		{
			sp->OnDraw();
			++it;
		}
		else
			it = m_observers.erase(it);
	}
}

void Game::NotifyPawnUpgradePGN()
{
	for (auto it = m_observers.begin(); it != m_observers.end();)
	{
		if (auto sp = it->lock())
		{
			sp->OnPawnUpgradePGN();
			++it;
		}
		else
			it = m_observers.erase(it);
	}
}

void Game::NotifyPaused()
{
	for (auto it = m_observers.begin(); it != m_observers.end();)
	{
		if (auto sp = it->lock())
		{
			sp->OnPaused();
			++it;
		}
		else
			it = m_observers.erase(it);
	}
}

void Game::NotifyUITimer()
{
	for (auto it = m_observers.begin(); it != m_observers.end();)
	{
		if (auto sp = it->lock())
		{
			sp->OnTimerStart();
			++it;
		}
		else
			it = m_observers.erase(it);
	}
}

void Game::PauseGame()
{
	m_timer.PauseTimer();
	NotifyPaused();
}

void Game::ResumeGame()
{
	m_timer.ResumeTimer();
}

EPieceType CharToType(char c)
{
	static const EPieceType TYPES[] = { EPieceType::Rook, EPieceType::Knight, EPieceType::Bishop, EPieceType::Queen, EPieceType::King };
	char str[] = "RNBQK";
	char* p = strchr(str, c);

	return TYPES[p - str];
}

Position Game::FindPieceStartPos(int startRow, int startCol, Position endPos, EPieceType type, bool turn) const
{
	EPieceColor	color = turn ? EPieceColor::Black : EPieceColor::White;
	return m_board.FindPieceStartPos(startRow, startCol, endPos, type, color);
}

std::tuple<Position, Position, EPieceType> Game::ConvertPGNMoveToInfoMove(std::string move, bool turn)
{
	Position startPos = { -1, -1 }, endPos;
	EPieceType type, upgradeType = EPieceType::None;

	if (move[0] == 'O')
	{
		startPos.x = turn ? 0 : 7;
		startPos.y = 4;
		endPos.x = turn ? 0 : 7;

		if (move.size() == 3)
			endPos.y = 6;
		else
			endPos.y = 2;
	}
	else
	{
		// verif if it is game result
		if (move[1] == '-')
			return std::make_tuple(startPos, endPos, upgradeType);

		//verif upgrade
		if (move[move.size() - 2] == '=')
		{
			upgradeType = CharToType(move[move.size() - 1]);
			move.erase(move.end() - 2, move.end());
		}

		//take end position
		endPos.x = 8 - (move[move.size() - 1] - '0');
		endPos.y = move[move.size() - 2] - 'a';

		move.erase(move.end() - 2, move.end());

		// if it is uppercase it is not a pawn
		if (move.size() && move[0] < 91)
		{
			type = CharToType(move[0]);
			move.erase(move.begin() + 0);

			if (move.size() == 3)
			{
				startPos.x = 8 - (move[1] - '0');
				startPos.y = move[0] - 'a';
			}
			else if (move.size() == 2)
			{
				if (move[move.size() - 1] != 'x')
				{
					startPos.x = 8 - (move[1] - '0');
					startPos.y = move[0] - 'a';
				}
				else if (isdigit(move[0]))
					startPos.x = 8 - (move[0] - '0');
				else
					startPos.y = move[0] - 'a';
			}
			else if (move.size() == 1)
				if (isdigit(move[0]))
					startPos.x = 8 - (move[0] - '0');
				else if (move[0] != 'x')
					startPos.y = move[0] - 'a';
		}
		else
		{
			type = EPieceType::Pawn;
			if (move.size())
				startPos.y = move[0] - 'a';
		}
	}

	if (startPos.x == -1)
	{
		if (startPos.y == -1)
			startPos = FindPieceStartPos(-1, -1, endPos, type, turn);
		else
			startPos = FindPieceStartPos(-1, startPos.y, endPos, type, turn);
	}
	else if (startPos.y == -1)
		startPos = FindPieceStartPos(startPos.x, -1, endPos, type, turn);

	return std::make_tuple(startPos, endPos, upgradeType);
}

const IGameStatus* Game::GetStatus() const
{
	return this;
}

int Game::GetTime(EPlayer player) const
{
	return m_timer.GetTimerDuration(player);
}

EGameType Game::GetGameType() const
{
	return m_timer.GetType();
}

std::string Game::GetPGNMovesSection() const
{
	return m_pgnBuilder.GetPGNMovesSection();
}

void Game::LoadFromPGNFile(const std::string& filePath)
{
	m_pgnBuilder.LoadPGNFromFile(filePath);
	InitializeGamePGN(m_pgnBuilder.GetPGNMovesSection());
}

void Game::SaveToPGNFile(const std::string& filePath)
{
	m_pgnBuilder.SavePGNToFile(filePath);
}

void Game::InitializeGamePGN(const std::string& pgnStr)
{
	ResetGame();
	m_timer.PauseTimer();

	auto movesPGN = m_pgnBuilder.GenerateMoves(pgnStr);
	for (int i = 0; i < movesPGN.size(); i++)
	{
		std::tuple<Position, Position, EPieceType> movePos = ConvertPGNMoveToInfoMove(movesPGN[i], i % 2);

		if (std::get<0>(movePos).x != -1)
			MakeMove(std::get<0>(movePos), std::get<1>(movePos), true);
		if (std::get<2>(movePos) != EPieceType::None)
		{
			UpgradePawnTo(std::get<2>(movePos));
			UpdatePGNMate(m_board);

			auto color = m_turn ? EPieceColor::Black : EPieceColor::White;
			if (!m_board.IsCheckmate(color))
				UpdatePGNCheck(m_board);
		}
	}
}

std::string Game::GetPGN() const
{
	return m_pgnBuilder.GetPGN();
}

void Game::InitializeGameFEN(const std::string& strFEN)
{
	ResetGame();
	m_timer.PauseTimer();
	m_board.InitializeBoardFEN(strFEN);

	m_turn = (strFEN.find(" w") != std::string::npos) ? 0 : 1;
}

std::string Game::GetFEN() const
{
	auto fen = m_board.GenerateBoardFEN();
	fen += m_turn ? "b " : "w ";
	fen += m_board.GenerateCastlingPossibleFEN();
	return fen;
}

MoveList Game::GetMovesPGN(const std::string& pgnStr) const
{
	return m_pgnBuilder.GenerateMoves(pgnStr);
}

void Game::AddListener(IGameListenerPtr listener)
{
	m_observers.push_back(listener);
}

void Game::RemoveListener(IGameListener* listener)
{
	auto func = [listener](IGameListenerWeakPtr& weak)
	{
		auto sp = weak.lock();

		return !sp || sp.get() == listener;
	};

	m_observers.erase(std::remove_if(m_observers.begin(), m_observers.end(), func));

	/*for (auto it = m_observers.begin(); it != m_observers.end();)
	{
		if (auto sp = it->lock())
		{
			if (sp.get() == listener)
				it = m_observers.erase(it);
			else
				++it;
		}
		else
			it = m_observers.erase(it);
	}*/
}

PositionList Game::GetPossibleMoves(Position pos) const
{
	return m_board.GetPossibleMoves(pos);
}

IPieceInfoPtrList Game::GetCapturedPieces(EPieceColor color) const
{
	return m_board.GetCapturedPieces(color);
}

bool Game::IsPlaying() const
{
	return m_state == EGameState::Playing;
}

void Game::PlayerComand(EComand camand)
{
	switch (camand)
	{
	case EComand::Draw:
		if (!IsState(EGameState::Playing))
			throw NotStatePlayingException();
		UpdateState(EGameState::DrawProposed);
		PauseGame();
		m_turn = 1 - m_turn;
		break;
	case EComand::Accept:
		if (!IsState(EGameState::DrawProposed))
			throw NotStateDrawProposedException();
		UpdateState(EGameState::Draw);
		UpdatePGNDraw();
		NotifyDraw();
		break;
	case EComand::Decline:
		if (!IsState(EGameState::DrawProposed))
			throw NotStateDrawProposedException();
		UpdateState(EGameState::Playing);
		ResumeGame();
		m_turn = 1 - m_turn;
		break;
	case EComand::Pause:
		if (!IsState(EGameState::Playing))
			throw NotStatePlayingException();
		UpdateState(EGameState::Paused);
		PauseGame();
		break;
	case EComand::Resume:
		if (!IsState(EGameState::Paused))
			throw NotStatePausedException();
		UpdateState(EGameState::Playing);
		ResumeGame();
		break;
	}
}

void Game::UpgradePawnTo(EPieceType type)
{
	if (!IsState(EGameState::WaitingForPawnUpgrade))
		throw NotStateWaitingForPawnUpdate();

	auto color = m_turn ? EPieceColor::Black : EPieceColor::White;
	int i = m_turn ? 7 : 0;

	if (type == EPieceType::None || type == EPieceType::Pawn || type == EPieceType::King)
		throw InvalidUpgradeType();

	for (int j = 0; j < 8; j++)
	{
		if (GetPieceInfo({ i, j }) && GetPieceInfo({ i, j })->GetType() == EPieceType::Pawn)
		{
			m_board.SetPiece({ i, j }, color, type);
			break;
		}
	}

	// update PGN 
	UpdatePGNUpgradePawn(type);

	m_turn = 1 - m_turn;
	m_timer.UpdateTurn();
	UpdateState(EGameState::Playing);
}

bool Game::IsWaitingForPawnUpgrade() const
{
	return m_state == EGameState::WaitingForPawnUpgrade;
}

bool Game::IsDrawProposed() const
{
	return m_state == EGameState::DrawProposed;
}

bool Game::IsDraw() const
{
	return m_state == EGameState::Draw;
}

EPlayer Game::GetCurrentPlayer() const
{
	return m_turn ? EPlayer::Black : EPlayer::White;
}

bool Game::IsState(EGameState state) const
{
	return m_state == state;
}

void Game::UpdateState(EGameState state)
{
	m_state = state;
}

IPieceInfoPtr Game::GetPieceInfo(Position pos) const
{
	return m_board.GetPieceInfo(pos);
}
