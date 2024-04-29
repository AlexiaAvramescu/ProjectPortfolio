#include "WaitingRoom.h"

WaitingRoom::WaitingRoom(QWidget* parent, std::shared_ptr<Routing> rt)
	: QWidget(parent)
	, ui(new Ui::WaitingRoomClass())
	, m_refreshCount{ 0 }
	, m_rt{ rt }
	, m_admin{ "" }
{
	ui->setupUi(this);
	connect(ui->startGame, &QPushButton::clicked, this, &WaitingRoom::OnStartBtnPushed);
	connect(ui->pushButton, &QPushButton::clicked, this, &WaitingRoom::AddCustomWord);
}

void WaitingRoom::OnStartBtnPushed()
{
	auto players = m_rt->GetPlayers();
	auto nrCustomWords = m_rt->GetCustomWordsCount();
	if (players.size() <= 1)
		QMessageBox::warning(this, "Waiting Room", "Not enough players to start the game.");
	else
		if (players.size() != nrCustomWords)
			QMessageBox::warning(this, "Custom Word", "Not all players have added their custom word yet. Please wait until everyone has finished their input before starting the game.");
		else
			if (m_rt->SendStartGame())
				emit GoToGame();
}

WaitingRoom::~WaitingRoom()
{
	delete ui;
}

void WaitingRoom::SetRoomCode(QString lobbyCode)
{
	ui->label_2->setText(lobbyCode);
}

void WaitingRoom::AddUserToRoom(const QString& user)
{
	QListWidgetItem* existingItem = nullptr;
	for (int i = 0; i < ui->listWidget->count(); ++i)
	{
		QListWidgetItem* item = ui->listWidget->item(i);
		if (item->text() == user)
		{
			existingItem = item;
			break;
		}
	}
	if(!existingItem)
	{
		QListWidgetItem* item = new QListWidgetItem(user);
		ui->listWidget->addItem(item);
	}
}

void WaitingRoom::UpdatePlayerList(const std::vector<QString>& players)
{
	ui->listWidget->clear();
	m_admin = players[0];
	for (const auto& player : players)
	{
		AddUserToRoom(player);
		if (QString::fromLatin1(m_rt->GetPlayerName().data()) == m_admin)
			ui->startGame->setVisible(true);
		else ui->startGame->setVisible(false);
	}
}

void WaitingRoom::FetchPlayers()
{
	std::vector<QString> players = m_rt->GetPlayers();
	UpdatePlayerList(players);
}

void WaitingRoom::ResetButtons()
{
	ui->lineEdit->clear();
	ui->lineEdit->setDisabled(false);
	ui->pushButton->setDisabled(false);
}

void WaitingRoom::paintEvent(QPaintEvent* e)
{
	if (m_refreshCount % kRefreshRate == 0)
	{
		FetchPlayers();
		if (m_rt->GetRound() != kWaitingState)
			emit GoToGame();
	}
	m_refreshCount++;
}

void WaitingRoom::AddCustomWord()
{
	QString word = ui->lineEdit->text();
	if (!word.isEmpty())
	{
		ui->lineEdit->setDisabled(true);
		ui->pushButton->setDisabled(true);
		m_rt->SendCustomWord(word.toLatin1().data());
	}
}
