#include "ChessUIQt.h"
#include <QPlainTextEdit>
#include <QFileDialog>
#include <QPlainTextEdit>
#include <QInputDialog>
#include <QMessageBox>
#include <QClipboard>
#include <QApplication>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QGraphicsBlurEffect>
#include <QGraphicsEffect>
#include <fstream>
#include <iostream>
#include <iomanip>

#include "ChessExceptions.h"

ChessUIQt::ChessUIQt(QWidget* parent)
	: QMainWindow(parent)
	, game(nullptr)
{

	//Widget containing everything
	QWidget* mainWidget = new QWidget();
	QGridLayout* mainGridLayout = new QGridLayout();

	InitializeBoard(mainGridLayout);
	InitializeMessage(mainGridLayout);
	InitializeButtons(mainGridLayout);
	InitializeTimers(mainGridLayout);
	InitializeHistory(mainGridLayout);
	InitializeCapturedPiecesDisplay(mainGridLayout);

	mainWidget->setLayout(mainGridLayout);
	this->setCentralWidget(mainWidget);

	CenterOnScreen();
}

ChessUIQt::~ChessUIQt()
{

	//No delete?
	//https://doc.qt.io/qt-6/objecttrees.html
}

void ChessUIQt::InitializeMessage(QGridLayout* mainGridLayout)
{
	m_MessageLabel = new QLabel();
	m_MessageLabel->setText("Waiting for white player");
	m_MessageLabel->setAlignment(Qt::AlignCenter);
	m_MessageLabel->setStyleSheet("font-size: 20px; font-weight: bold;");

	mainGridLayout->addWidget(m_MessageLabel, 0, 2, 1, 1);
}

void ChessUIQt::InitializeButtons(QGridLayout* mainGridLayout)
{
	QPushButton* saveButton = new QPushButton("Save");
	QPushButton* loadButton = new QPushButton("Load");
	QPushButton* restartButton = new QPushButton("Restart");
	QPushButton* drawButton = new QPushButton("Draw");
	QPushButton* copyButton = new QPushButton("Copy");


	QWidget* buttonContainer = new QWidget();
	QGridLayout* btnGrid = new QGridLayout();

	btnGrid->addWidget(saveButton, 0, 0);
	btnGrid->addWidget(loadButton, 0, 1);
	btnGrid->addWidget(restartButton, 0, 2);
	btnGrid->addWidget(drawButton, 0, 3);
	btnGrid->addWidget(copyButton, 0, 4);

	connect(saveButton, &QPushButton::pressed, this, &ChessUIQt::OnSaveButtonClicked);
	connect(loadButton, &QPushButton::pressed, this, &ChessUIQt::OnLoadButtonClicked);
	connect(restartButton, &QPushButton::pressed, this, &ChessUIQt::OnRestartButtonClicked);
	connect(drawButton, &QPushButton::pressed, this, &ChessUIQt::OnDrawButtonClicked);
	connect(copyButton, &QPushButton::pressed, this, &ChessUIQt::OnCopyButtonClicked);

	buttonContainer->setLayout(btnGrid);
	mainGridLayout->addWidget(buttonContainer, 0, 0, 1, 1);
}

static QString GetInitialTime(EGameType type)
{
	int time;
	switch (type)
	{
	case EGameType::Rapid:
		time = 30 * 60000;
		break;
	case EGameType::Blitz:
		time = 10 * 60000;
		break;
	case EGameType::Bullet:
		time = 3 * 60000;
		break;
	case EGameType::NoTimer:
		time = 0;
		break;
	}

	int minutes = (time / 1000) / 60;
	int seconds = (time / 1000) % 60;
	int remainingMilliseconds = time % 1000;

	QString timeStr = QString("%1:%2:%3").arg(minutes, 2, 10, QChar('0'))
		.arg(seconds, 2, 10, QChar('0')).arg(remainingMilliseconds / 10, 2, 10, QChar('0'));

	return timeStr;
}

void ChessUIQt::InitializeTimers(QGridLayout* mainGridLayout)
{
	QWidget* timerContainer = new QWidget();
	QGridLayout* timerGrid = new QGridLayout();

	QLabel* blackTimerLbl = new QLabel("Black timer: ");
	m_BlackTimer = new QLabel("00:00:00");

	QPushButton* pauseTimerBtn = new QPushButton(" Pause ");
	connect(pauseTimerBtn, &QPushButton::pressed, this, &ChessUIQt::OnPauseButtonClicked);

	QLabel* whiteTimerLbl = new QLabel("    White timer: ");
	m_WhiteTimer = new QLabel("00:00:00");

	timerContainer->setFixedWidth(400);

	timerGrid->addWidget(blackTimerLbl, 0, 0);
	timerGrid->addWidget(m_BlackTimer, 0, 1);
	timerGrid->addWidget(pauseTimerBtn, 0, 2);
	timerGrid->addWidget(whiteTimerLbl, 0, 3);
	timerGrid->addWidget(m_WhiteTimer, 0, 4);

	timerContainer->setLayout(timerGrid);
	mainGridLayout->addWidget(timerContainer, 2, 1, 1, 2, Qt::AlignCenter);
}

void ChessUIQt::DisplayTimers()
{
	auto gameType = game->GetStatus()->GetGameType();

	m_BlackTimer->setText(GetInitialTime(gameType));
	m_WhiteTimer->setText(GetInitialTime(gameType));
}

void ChessUIQt::InitializeHistory(QGridLayout* mainGridLayout)
{
	m_moveNumberList = new QListWidget(this);
	m_whiteMoveList = new QListWidget(this);
	m_blackMoveList = new QListWidget(this);

	m_moveNumberList->setFixedWidth(50);
	m_whiteMoveList->setFixedWidth(150);
	m_blackMoveList->setFixedWidth(150);

	// Create QLabel widgets for headers
	QLabel* moveNumberHeader = new QLabel("Nr.");
	QLabel* whiteMoveHeader = new QLabel("White Move");
	QLabel* blackMoveHeader = new QLabel("Black Move");

	moveNumberHeader->setAlignment(Qt::AlignCenter);
	whiteMoveHeader->setAlignment(Qt::AlignCenter);
	blackMoveHeader->setAlignment(Qt::AlignCenter);
	m_moveNumberList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_whiteMoveList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_blackMoveList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	// Create the layout for the three group boxes
	QVBoxLayout* moveNumberLayout = new QVBoxLayout();
	moveNumberLayout->addWidget(moveNumberHeader);
	moveNumberLayout->addWidget(m_moveNumberList);

	QVBoxLayout* whiteMoveLayout = new QVBoxLayout();
	whiteMoveLayout->addWidget(whiteMoveHeader);
	whiteMoveLayout->addWidget(m_whiteMoveList);

	QVBoxLayout* blackMoveLayout = new QVBoxLayout();
	blackMoveLayout->addWidget(blackMoveHeader);
	blackMoveLayout->addWidget(m_blackMoveList);

	// Create the layout for the entire window
	QHBoxLayout* layout = new QHBoxLayout();
	layout->addLayout(moveNumberLayout);
	layout->addLayout(whiteMoveLayout);
	layout->addLayout(blackMoveLayout);

	mainGridLayout->addLayout(layout, 1, 0, 1, 1);
}

void ChessUIQt::InitializeBoard(QGridLayout* mainGridLayout)
{
	QGridLayout* chessGridLayout;
	chessGridLayout = new QGridLayout();
	QWidget* board = new QWidget();

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			m_grid[i][j] = new GridButton({ i,j }, PieceType::none, PieceColor::none);
			chessGridLayout->addWidget(m_grid[i][j], i, j, 1, 1);
			connect(m_grid[i][j], &GridButton::Clicked, this, &ChessUIQt::OnButtonClicked);
		}
	}

	board->setLayout(chessGridLayout);
	mainGridLayout->addWidget(board, 1, 2, 1, 1);
}

void ChessUIQt::InitializeCapturedPiecesDisplay(QGridLayout* mainGridLayout)
{
	QGridLayout* leftCapturedPiecesGridLayout = new QGridLayout();
	QGridLayout* rightCapturedPiecesGridLayout = new QGridLayout();
	QWidget* leftCapturedPieces = new QWidget();
	QWidget* rightCapturedPieces = new QWidget();

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			m_capturedPieces[0][i][j] = new CapturedPiecesCellDisplay(PieceType::none, PieceColor::none);
			m_capturedPieces[1][i][j] = new CapturedPiecesCellDisplay(PieceType::none, PieceColor::none);

			leftCapturedPiecesGridLayout->addWidget(m_capturedPieces[0][i][j], i, j, 1, 1);
			rightCapturedPiecesGridLayout->addWidget(m_capturedPieces[1][i][j], i, j, 1, 1);
		}
	}

	leftCapturedPieces->setLayout(leftCapturedPiecesGridLayout);
	rightCapturedPieces->setLayout(rightCapturedPiecesGridLayout);

	mainGridLayout->addWidget(leftCapturedPieces, 1, 1, 1, 1);
	mainGridLayout->addWidget(rightCapturedPieces, 1, 3, 1, 1);
}

void ChessUIQt::CenterOnScreen()
{
	QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();

	// Calculate the center position
	int x = (screenGeometry.width() - width()) / 2;
	int y = (screenGeometry.height() - height()) / 2;

	// Move the main widget to the center
	move(x - 300, y - 120);
}

static PieceColor ConvertColorEnum(EPieceColor color)
{
	if ((int)color)
		return PieceColor::black;
	if (!(int)color)
		return PieceColor::white;
	return PieceColor::none;
}

static PieceColor ConvertPlayerToColor(EPlayer player)
{
	if ((int)player)
		return PieceColor::black;
	if (!(int)player)
		return PieceColor::white;
	return PieceColor::none;
}

static PieceType ConvertTypeEnum(EPieceType type)
{
	std::vector<PieceType> TYPES = { PieceType::pawn, PieceType::rook, PieceType::knight,PieceType::bishop, PieceType::queen, PieceType::king, PieceType::none, };
	return TYPES[(int)type];
}

static QString ConvertEGameResultToQStr(EPlayer result)
{
	switch (result)
	{
	case EPlayer::White:
		return "The winner is: Player white";
		break;
	case EPlayer::Black:
		return "The winner is: Player black";
		break;
	case EPlayer::None:
		return "The winner is: None";
		break;
	}
}

void ChessUIQt::OnButtonClicked(const Position& position)
{
	auto status = game->GetStatus();
	//At second click
	if (m_selectedCell.has_value())
	{
		if (m_selectedCell->x == position.x && m_selectedCell->y == position.y)
		{
			//deselect cells
			m_grid[m_selectedCell.value().x][m_selectedCell.value().y]->setSelected(false);
			m_selectedCell.reset();
			auto possibleMoves = status->GetPossibleMoves(position);
			UnhighlightPossibleMoves(possibleMoves);
		}
		else
		{
			try
			{
				game->MakeMove(Position(m_selectedCell->x, m_selectedCell->y), position, false);
			}
			catch (ChessExceptions e)
			{
				/*QMessageBox msgBox;
				msgBox.setText(e.what());
				msgBox.exec();*/
			}

			//Unselect prev. pressed button and  unhighlight board
			m_grid[m_selectedCell.value().x][m_selectedCell.value().y]->setSelected(false);
			m_selectedCell.reset();
			UpdateBoard();
		}
	}
	//At first click
	else {
		if (ConvertPlayerToColor(status->GetCurrentPlayer()) == m_grid[position.x][position.y]->GetPieceColor())
		{
			m_selectedCell = position;
			m_grid[position.x][position.y]->setSelected(true);

			//TODO Show possible moves here
			HighlightPossibleMoves(status->GetPossibleMoves(position));
		}
	}
}

void ChessUIQt::OnPauseButtonClicked()
{
	try
	{
		game->PlayerComand(EComand::Pause);
	}
	catch (ChessExceptions e)
	{
		/*QMessageBox msgBox;
		msgBox.setText(e.what());
		msgBox.exec();*/
	}
}

void ChessUIQt::OnSaveButtonClicked()
{
	QString fileSave = QFileDialog::getSaveFileName(this, "Save File", "", "FEN Files(*.fen);; PGN Files(*.pgn)");
	if (!fileSave.isEmpty()) {
		std::string data;

		QString fileExtension = QFileInfo(fileSave).suffix();
		if (fileExtension == "fen")
		{
			data = game->GetStatus()->GetFEN();
			std::ofstream outputFile(fileSave.toStdString());
			if (outputFile.is_open()) {
				outputFile << data;
				outputFile.close();
			}
		}
		if (fileExtension == "pgn")
		{
			try
			{
				game->SaveToPGNFile(fileSave.toStdString());
			}
			catch (ChessExceptions e)
			{
				QMessageBox msgBox;
				msgBox.setText(e.what());
				msgBox.exec();
			}
		}
	}
}

void ChessUIQt::OnLoadButtonClicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open File", "", "FEN Files (*.fen);; PGN Files (*.pgn)");
	if (!fileName.isEmpty()) {
		QFileInfo fileInfo(fileName);
		QString fileExtension = fileInfo.suffix().toLower();

		if (fileExtension == "fen") {
			QString fileData;
			QFile file(fileName);

			// this if should be in PGN builder 
			if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
				QTextStream stream(&file);
				fileData = stream.readAll();
				file.close();
			}

			OnRestartButtonClicked();
			game->InitializeGameFEN(fileData.toStdString());

			m_MessageLabel->setText(game->GetStatus()->GetCurrentPlayer() == EPlayer::Black ? "Waiting for black player" : "Waiting for white player");
			UpdateCapturedPiecesDispay();
			UpdateBoard();
		}
		else if (fileExtension == "pgn") {

			OnRestartButtonClicked();
			try
			{
				game->LoadFromPGNFile(fileName.toStdString());

			}
			catch (ChessExceptions e)
			{
				QMessageBox msgBox;
				msgBox.setText(e.what());
				msgBox.exec();
			}

			UpdateCapturedPiecesDispay();
			UpdateBoard();
			UpdateHistory();
		}
		else {
			QMessageBox::critical(this, "Error", "Unsupported file extension: " + fileExtension, QMessageBox::Ok);
		}
	}
}

void ChessUIQt::RunMethod(std::function<void(void)> func)
{
	QMetaObject::invokeMethod(this, func, Qt::QueuedConnection);
}

void ChessUIQt::ResetTimerDisplay()
{
	int time = game->GetStatus()->GetTime(EPlayer::White);
	int minutes = (time / 1000) / 60;
	int seconds = (time / 1000) % 60;
	int remainingMilliseconds = time % 1000;

	// Convert minutes and seconds to a nicely formatted string
	QString timeStr = QString("%1:%2:%3").arg(minutes, 2, 10, QChar('0'))
		.arg(seconds, 2, 10, QChar('0')).arg(remainingMilliseconds / 10, 2, 10, QChar('0'));

	m_WhiteTimer->setText(timeStr);


	time = game->GetStatus()->GetTime(EPlayer::Black);
	minutes = (time / 1000) / 60;
	seconds = (time / 1000) % 60;
	remainingMilliseconds = time % 1000;

	// Convert minutes and seconds to a nicely formatted string
	timeStr = QString("%1:%2:%3").arg(minutes, 2, 10, QChar('0'))
		.arg(seconds, 2, 10, QChar('0')).arg(remainingMilliseconds / 10, 2, 10, QChar('0'));

	m_BlackTimer->setText(timeStr);
}

void ChessUIQt::ResetHistory()
{
	m_moveNumberList->clear();
	m_whiteMoveList->clear();
	m_blackMoveList->clear();
}

void ChessUIQt::ResetCapturedPiecesDisplay()
{
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 2; j++)
		{
			m_capturedPieces[0][i][j]->Update(PieceType::none, PieceColor::none);
			m_capturedPieces[1][i][j]->Update(PieceType::none, PieceColor::none);
		}
}

void ChessUIQt::UpdateCapturedPiecesDispay()
{
	auto status = game->GetStatus();
	OnCaptureMade(EPieceColor::Black, status->GetCapturedPieces(EPieceColor::Black));
	OnCaptureMade(EPieceColor::White, status->GetCapturedPieces(EPieceColor::White));
}

void ChessUIQt::OnRestartButtonClicked()
{
	game->ResetGame();

	ResetCapturedPiecesDisplay();
	ResetHistory();

	m_MessageLabel->setText("Waiting for white player");

	UpdateBoard();
	ResetTimerDisplay();
	UpdateTimers();
}

void ChessUIQt::OnDrawButtonClicked()
{

	try
	{
		game->PlayerComand(EComand::Draw);
		QMessageBox::StandardButton reply;
		reply = QMessageBox::question(this, "Draw proposal", "Do you accept a draw?", QMessageBox::Yes | QMessageBox::No);

		if (reply == QMessageBox::Yes) {
			QMessageBox msgBox;
			msgBox.setText("Draw Accepted. Winner is: None.");
			msgBox.exec();
			game->PlayerComand(EComand::Accept);
		}
		else
			game->PlayerComand(EComand::Decline);
	}
	catch (ChessExceptions e)
	{
		QMessageBox msgBox;
		msgBox.setText(e.what());
		msgBox.exec();
	}
}

static char PieceInfoToChar(PieceType type, PieceColor color)
{
	std::string letters = "-krbqnp";
	if (color == PieceColor::black)
		return toupper(letters[(int)type]);
	if (color == PieceColor::white)
		return tolower(letters[(int)type]);
	return letters[(int)type];
}

void ChessUIQt::OnCopyButtonClicked()
{
	QClipboard* clipboard = QApplication::clipboard();
	QString boardConfig = "{ \n";

	for (int i = 0; i < 8; i++)
	{
		boardConfig += '{';
		for (int j = 0; j < 8; j++)
		{
			boardConfig += QString('\'') + PieceInfoToChar(m_grid[i][j]->GetPieceType(), m_grid[i][j]->GetPieceColor()) + '\'';
			if (j != 7)
				boardConfig += ", ";
		}
		boardConfig += '}';
		if (i != 7)
			boardConfig += ", \n";
	}
	boardConfig += "\n }";

	clipboard->setText(boardConfig);
}

void ChessUIQt::OnHistoryClicked(QListWidgetItem* item)
{
	//int index = m_historyTableWidget->currentRow();

	//TODO ...
}

void ChessUIQt::UpdateHistory()
{
	auto status = game->GetStatus();
	auto movesPGN = status->GetMovesPGN(status->GetPGNMovesSection());

	if (movesPGN.size())
	{
		if (status->GetCurrentPlayer() != EPlayer::White)
		{
			QString itemText = QString::number(movesPGN.size() / 2 + 1);
			QListWidgetItem* item = new QListWidgetItem(itemText);
			item->setFlags(item->flags() | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			m_moveNumberList->addItem(item);

			QString moveTextW = QString::fromStdString(movesPGN[movesPGN.size() - 1]);
			QListWidgetItem* itemMoveW = new QListWidgetItem(moveTextW);
			itemMoveW->setFlags(itemMoveW->flags() | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			m_whiteMoveList->addItem(itemMoveW);

			//QString moveTextB = QString::fromStdString("  ");
			QListWidgetItem* itemMoveB = new QListWidgetItem(" ");
			itemMoveB->setFlags(itemMoveB->flags() | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			m_blackMoveList->addItem(itemMoveB);
		}
		else
		{
			
			QListWidgetItem* lastItem = m_blackMoveList->item(m_blackMoveList->count() - 1);
			if (!lastItem)
			{
				QListWidgetItem* itemMoveB = new QListWidgetItem(" ");
				itemMoveB->setFlags(itemMoveB->flags() | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
				m_blackMoveList->addItem(itemMoveB);
			}
			lastItem = m_blackMoveList->item(m_blackMoveList->count() - 1);
			lastItem->setText(QString::fromStdString(movesPGN[movesPGN.size() - 1]));
		}
	}
}

void ChessUIQt::UpdateTimers()
{
	auto status = game->GetStatus();
	auto currentPlayer = status->GetCurrentPlayer();
	auto time = status->GetTime(currentPlayer);

	int minutes = (time / 1000) / 60;
	int seconds = (time / 1000) % 60;
	int remainingMilliseconds = time % 1000;

	// Convert minutes and seconds to a nicely formatted string
	QString timeStr = QString("%1:%2:%3").arg(minutes, 2, 10, QChar('0'))
		.arg(seconds, 2, 10, QChar('0')).arg(remainingMilliseconds / 10, 2, 10, QChar('0'));

	// Update the QLabel text
	RunMethod([&, timeStr, currentPlayer]()
		{
			if ((int)currentPlayer)
				m_BlackTimer->setText(timeStr);
			else
				m_WhiteTimer->setText(timeStr);
		});
}

void ChessUIQt::UpdateBoard()
{
	auto status = game->GetStatus();
	PieceColor color;
	PieceType type;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {

			if (status->GetPieceInfo(Position(i, j)))
			{
				type = ConvertTypeEnum(status->GetPieceInfo(Position(i, j))->GetType());
				color = ConvertColorEnum(status->GetPieceInfo(Position(i, j))->GetColor());
			}
			else
			{
				type = PieceType::none;
				color = PieceColor::none;
			}

			m_grid[i][j]->setPiece({ type, color });
			m_grid[i][j]->setSelected(false);
			m_grid[i][j]->setHighlighted(false);
		}
	}
}

void ChessUIQt::HighlightPossibleMoves(const PositionList& possibleMoves)
{
	for (const auto& position : possibleMoves) {
		m_grid[position.x][position.y]->setHighlighted(true);
	}
}

void ChessUIQt::UnhighlightPossibleMoves(const PositionList& possibleMoves)
{
	for (const auto& position : possibleMoves) {
		m_grid[position.x][position.y]->setHighlighted(false);
	}
}

void ChessUIQt::StartGame()
{
	UpdateBoard();
	UpdateHistory();
}

EPieceType ConvetItemToEPieceType(QString item)
{
	if (item == "Rook")
		return EPieceType::Rook;
	if (item == "Bishop")
		return EPieceType::Bishop;
	if (item == "Queen")
		return EPieceType::Queen;
	if (item == "Knight")
		return EPieceType::Knight;
}

void ChessUIQt::SetGame(IGamePtr game)
{
	this->game = game;
	UpdateTimers();
}

void ChessUIQt::OnMoveMade(Position startPos, Position endPos, PositionList prevPossibleMoves)
{
	auto status = game->GetStatus();
	auto type = ConvertTypeEnum(status->GetPieceInfo(endPos)->GetType());
	auto color = ConvertColorEnum(status->GetPieceInfo(endPos)->GetColor());

	m_grid[endPos.x][endPos.y]->setPiece({ type, color });
	m_grid[endPos.x][endPos.y]->setSelected(false);
	m_grid[endPos.x][endPos.y]->setHighlighted(false);

	m_grid[startPos.x][startPos.y]->setPiece({ PieceType::none, PieceColor::none });
	m_grid[startPos.x][startPos.y]->setSelected(false);
	m_grid[startPos.x][startPos.y]->setHighlighted(false);

	auto bol = status->GetCurrentPlayer() == EPlayer::Black;
	m_MessageLabel->setText(status->GetCurrentPlayer() == EPlayer::Black ? "Waiting for black player" : "Waiting for white player");

	//update history
	UpdateHistory();
	UpdateBoard();
}

void ChessUIQt::OnPawnUpgrade()
{
	UpdateBoard();

	QList<QString> options;
	options.append("Rook");
	options.append("Bishop");
	options.append("Queen");
	options.append("Knight");

	while (true)
	{
		QInputDialog dialog;
		dialog.setComboBoxItems(options);
		dialog.setModal(true);
		int result = dialog.exec();

		// Process the user's selection
		if (result == QDialog::Accepted) {
			QString selectedPiece = dialog.textValue(); // Get the selected piece (e.g., "Rook", "Bishop", etc.)

			game->UpgradePawnTo(ConvetItemToEPieceType(selectedPiece));
			break;
		}
	}
	auto turn = game->GetStatus()->GetCurrentPlayer();
	m_MessageLabel->setText(turn == EPlayer::Black ? "Waiting for black player" : "Waiting for white player");
	UpdateHistory();
	UpdateBoard();
}

void ChessUIQt::OnGameOver()
{
	RunMethod([this]()
		{
			auto status = game->GetStatus();
			QMessageBox msgBox;
			msgBox.setWindowTitle("Game Over");
			QString str;
			str = ConvertEGameResultToQStr(status->GetWinner());
			msgBox.setText(str);

			QPushButton* Save = msgBox.addButton("Save", QMessageBox::AcceptRole);
			QPushButton* close = msgBox.addButton("Close", QMessageBox::AcceptRole);
			QPushButton* newGame = msgBox.addButton("New Game", QMessageBox::AcceptRole);

			QObject::connect(Save, &QPushButton::clicked, [this]() {
				OnSaveButtonClicked();
				});

			QObject::connect(close, &QPushButton::clicked, [&]() {
				});

			QObject::connect(newGame, &QPushButton::clicked, [this]() {
				OnRestartButtonClicked();
				});

			msgBox.exec();
		});
}

void ChessUIQt::OnCaptureMade(EPieceColor color, IPieceInfoPtrList capturedPieces)
{
	if (!capturedPieces.empty())
	{
		int k = 0;
		for (int i = 0; i < 8 && k < capturedPieces.size(); i++)
		{
			for (int j = 0; j < 2 && k < capturedPieces.size(); j++)
			{
				auto capturedPieceColor = ConvertColorEnum(capturedPieces[k]->GetColor());
				auto capturedPieceType = ConvertTypeEnum(capturedPieces[k]->GetType());
				m_capturedPieces[(int)color][i][j]->Update(capturedPieceType, capturedPieceColor);
				k++;
			}
		}
	}
}

void ChessUIQt::OnDraw()
{
	OnRestartButtonClicked();
}

void ChessUIQt::OnPawnUpgradePGN()
{
	UpdateHistory();
}

void ChessUIQt::OnTimerStart()
{
	UpdateTimers();
}

void ChessUIQt::OnPaused()
{
	RunMethod([this]() {
		QDialog dialog(this);
		dialog.setWindowTitle("Game Paused");
		dialog.setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
		dialog.setModal(true);

		QGraphicsBlurEffect* p_blur = new QGraphicsBlurEffect;
		p_blur->setBlurRadius(10);
		p_blur->setBlurHints(QGraphicsBlurEffect::QualityHint);
		this->setGraphicsEffect(p_blur);

		// Create a layout for the dialog
		QVBoxLayout* layout = new QVBoxLayout(&dialog);
		// Create a label for the message
		QLabel* label = new QLabel("Game Paused");
		QFont font = label->font();
		font.setPointSize(20); // Adjust the font size
		label->setFont(font);
		label->setAlignment(Qt::AlignCenter);
		layout->addWidget(label);

		// Create a "Resume" button
		QPushButton* resumeButton = new QPushButton("Resume");
		layout->addWidget(resumeButton);

		// Connect the "Resume" button's clicked signal to close the dialog
		QObject::connect(resumeButton, &QPushButton::clicked, &dialog, [&dialog, this]() {
			try
			{
				game->PlayerComand(EComand::Resume);
			}
			catch (ChessExceptions e)
			{
				QMessageBox msgBox;
				msgBox.setText(e.what());
				msgBox.exec();
			}
			dialog.accept();
			});

		int x = this->width() / 2;
		int y = this->height() / 5.1;
		dialog.move(this->mapToGlobal(QPoint(x, y)));

		dialog.resize(420, 420);
		dialog.exec();

		// Remove the blur effect from the main window
		this->setGraphicsEffect(nullptr);

		});
}

