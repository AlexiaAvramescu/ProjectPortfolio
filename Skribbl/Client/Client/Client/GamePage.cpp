#include "GamePage.h"
#include <QKeyEvent>
#include <QTableWidgetItem>
#include <QMessageBox>

GamePage::GamePage(QWidget* parent, std::shared_ptr<Routing> rt)
	: QWidget{ parent }
	, ui{ new Ui::GamePageClass() }
	, m_refreshCount{ 0 }
	, m_rt{ rt }
{
	ui->setupUi(this);
	ui->labelWord->setStyleSheet("font-size: 40px;");
	m_drawingArea = ui->drawingArea;

	connect(ui->sendAnswerBtn, &QPushButton::clicked, this, &GamePage::OnSendAnswerBtnClicked);
	connect(ui->undoLastLineBtn, &QPushButton::clicked, this, &GamePage::OnUndoBtnClicked);
	connect(ui->exitGameBtn, &QPushButton::clicked, this, &GamePage::OnExitGameBtnClicked);
	connect(ui->resetDrawingBtn, &QPushButton::clicked, this, &GamePage::OnResetDrawingBtnClicked);
}

GamePage::~GamePage() {
	delete ui;
}

Ui::GamePageClass* GamePage::GetUi()
{
	return ui;
}

void GamePage::paintEvent(QPaintEvent* e)
{
	if (m_refreshCount == 0)
		UpdateEndTurn();
	if (m_refreshCount % kRefreshRate == 0)
	{
		QString time = m_rt->GetTime();
		UpdateChat(std::move(m_rt->GetAnswers()));

		ui->lableSeconds->setText(time);

		auto word = m_rt->GetWord();

		if (word != m_word) {
			m_word = word;
			UpdateEndTurn();
		}

		QString round = m_rt->GetRound();
		ui->labelRound->setText(round);

		if ((round == kGameOverState || round == kWaitingState))
			ShowLeaderBoard();

		if (m_rt->GetIsDrawing())
			m_rt->SendDrawing(m_drawingArea->GetDrawing());
		else {
			if (m_rt->GetTime().toInt() <= kFirstHintTime) {
				UpdateHint(m_rt->GetHint());
			}

			m_drawingArea->SetDrawing(m_rt->GetDrawing());
		}

		m_refreshCount = 1;
	}

	m_refreshCount++;
}

void GamePage::keyPressEvent(QKeyEvent* event) {
	if (event->key() == Qt::Key_Return)
		OnSendAnswerBtnClicked();

	if (event->key() == Qt::Key_Z && event->modifiers() & Qt::ControlModifier)
		OnUndoBtnClicked();
}

void GamePage::OnSendAnswerBtnClicked() {
	QString answer = ui->chatInput->text();
	if (answer.isEmpty())
		return;

	if (!m_rt->SendAnswer(answer.toUtf8().constData()))
		QMessageBox::information(nullptr, "Server Conection Problem", "Answert not sent.");
	ui->chatInput->clear();
}

void GamePage::UpdateLeaderBoard()
{
	std::vector<std::pair<QString, int16_t>> leaderBoard = m_rt->GetLeaderBoard();

	ui->leaderBoard->clearContents();
	ui->leaderBoard->setRowCount(leaderBoard.size());

	int rowIndex = 0;
	for (const auto& playerInfo : leaderBoard) {

		const auto& [playerName, score] = playerInfo;

		QTableWidgetItem* playerNameItem = new QTableWidgetItem(playerName);
		QTableWidgetItem* scoreItem = new QTableWidgetItem(QString::number((score)));

		playerNameItem->setTextAlignment(Qt::AlignCenter);
		scoreItem->setTextAlignment(Qt::AlignCenter);

		playerNameItem->setFlags(playerNameItem->flags() & ~Qt::ItemIsEditable);
		scoreItem->setFlags(scoreItem->flags() & ~Qt::ItemIsEditable);

		ui->leaderBoard->setItem(rowIndex, 0, playerNameItem);
		ui->leaderBoard->setItem(rowIndex, 1, scoreItem);
		rowIndex++;
	}
}

void GamePage::UpdateChat(const std::vector<QString>& answers) {

	if (answers.size() <= ui->answerList->count())
		return;
	size_t i = ui->answerList->count();
	for (; i < answers.size(); i++)
	{
		QListWidgetItem* answerItem = new QListWidgetItem(answers[i]);
		answerItem->setFlags(answerItem->flags() & ~Qt::ItemIsSelectable);
		ui->answerList->addItem(answerItem);
	}
	ui->answerList->scrollToBottom();
}

void GamePage::UpdateEndTurn()
{
	m_drawingArea->SetIsPlayerDrawing(m_rt->IsDrawingPlayer());
	m_drawingArea->ResetDrawing();

	UpdateLeaderBoard();

	m_word = m_rt->GetWord();

	if (m_rt->GetIsDrawing()) {
		ui->labelWord->setText(m_word);
		ui->chatInput->setDisabled(true);
	}
	else {
		QString hiddenWord;
		for (size_t i = 0; i < m_word.size(); i++)
			hiddenWord += "_ ";

		ui->labelWord->setText(hiddenWord);
		ui->chatInput->setDisabled(false);
	}
}

void GamePage::OnUndoBtnClicked() {
	m_drawingArea->UndoLastLine();
}

void GamePage::OnResetDrawingBtnClicked() {
	m_drawingArea->ResetDrawing();
}

void GamePage::OnExitGameBtnClicked() {

	ui->answerList->clear();
	m_drawingArea->ResetDrawing();

	emit ExitGame();
}

void GamePage::UpdateHint(std::vector<uint8_t> hintIndexs)
{
	QString hiddenWord;

	for (int i = 0; i < m_word.size(); i++)
		hiddenWord += "_ ";

	for (const auto& index : hintIndexs) {
		hiddenWord[index*2] = m_word[index];
	}
	ui->labelWord->setText(hiddenWord);
}

void GamePage::ShowLeaderBoard() {
	std::vector<std::pair<QString, int16_t>> playersAndScores = m_rt->GetLeaderBoard();

	m_leaderBoard = new LeaderBoardWidget(this, playersAndScores);
	m_leaderBoard->show();
	emit HandleEndGame();
}