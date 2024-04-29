#include "Client.h"

#include <QPushButton>
#include <QCryptographicHash>

Client::Client(QWidget* parent)
	: QMainWindow{ parent },
	ui{ new Ui::ClientClass }
{
	ui->setupUi(this);

	this->setFixedSize(1200, 700);

	m_stackedWidget = std::make_unique<QStackedWidget>(ui->centralWidget->findChild<QStackedWidget*>("stackedWidget"));
	ui->menuBar->hide();

	m_rt = std::make_shared<Routing>();
	m_loginPage = std::make_unique<LoginPage>(this);
	m_gamePage = std::make_unique<GamePage>(this, m_rt);
	m_lobbyPage = std::make_unique<LobbyPage>(this);
	m_waitingRoom = std::make_unique<WaitingRoom>(this, m_rt);
	m_userInfo = std::make_unique<UserInfo>(this);

	m_stackedWidget->addWidget(m_loginPage.get());
	m_stackedWidget->addWidget(m_waitingRoom.get());
	m_stackedWidget->addWidget(m_lobbyPage.get());
	m_stackedWidget->addWidget(m_gamePage.get());

	setCentralWidget(m_stackedWidget.get());

	connect(m_loginPage.get(), &LoginPage::loginSuccessful, this, &Client::ChangeToLobbyPage);
	connect(m_loginPage.get(), &LoginPage::SendLoginToServer, this, &Client::HandleLogin);
	connect(m_loginPage.get(), &LoginPage::SendRegisterToServer, this, &Client::HandleRegister);

	connect(m_waitingRoom.get(), &WaitingRoom::GoToGame, this, &Client::ChangeToGamePage);

	connect(m_lobbyPage.get(), &LobbyPage::goToLoginPage, this, &Client::ChangeToLoginPage);
	connect(m_lobbyPage.get(), &LobbyPage::SendCreateLobbyToServer, this, &Client::HandleCreateLobby);
	connect(m_lobbyPage.get(), &LobbyPage::SendJoinLobbyToServer, this, &Client::HandleJoinLobby);

	connect(m_gamePage.get(), &GamePage::ExitGame, this, &Client::ExitGame);
	connect(m_gamePage.get(), &GamePage::HandleEndGame, this, &Client::HandleEndGame);

	QAction* menuAction = new QAction("Game history", this);
	QMenu* menuUsername = ui->menuUsername;
	menuUsername->addAction(menuAction);
	connect(menuAction, &QAction::triggered, this, &Client::ShowUserInfo);

	ChangeToLoginPage();
}

void Client::closeEvent(QCloseEvent* event)
{
	if(!m_rt->GetLobbyCode().empty())
		m_rt->ExitGame();
}

void Client::ChangeToLoginPage() {
	m_stackedWidget->setCurrentWidget(m_loginPage.get());
}

void Client::ChangeToGamePage() {
	m_stackedWidget->setCurrentWidget(m_gamePage.get());
}

void Client::ChangeToLobbyPage() {

	m_stackedWidget->setCurrentWidget(m_lobbyPage.get());
}

void Client::ChangeToWaitingRoom()
{
	auto optionalLobbyCode = user.GetLobbyCode();
	if (!optionalLobbyCode.has_value()) {
		QMessageBox::information(nullptr, "Error", "Lobby Code is not set.");
		return;
	}

	QString lobbyCode = optionalLobbyCode.value();
	m_waitingRoom->ResetButtons();
	m_waitingRoom->SetRoomCode(lobbyCode);
	m_stackedWidget->setCurrentWidget(m_waitingRoom.get());
}

void Client::ShowUserInfo()
{
	m_userInfo->setWindowTitle(user.GetUsername() + "'s game history");
	m_userInfo->DisplayHistory(m_rt->GetGamesHistory());
	m_userInfo->show();
}

void Client::HandleAnswer()
{
	auto gameUi = m_gamePage->GetUi();
	std::string answer = gameUi->chatInput->text().toUtf8().constData();
	gameUi->chatInput->clear();

	

}

void Client::ExitGame()
{
	if (!m_rt->ExitGame())
		QMessageBox::information(nullptr, "Server Conection Problem", "Player did not exit game.");
	else
		ChangeToLobbyPage();
}

bool Client::ValidInput() {
	return true;
}

void Client::HandleLogin() {

	auto loginUi = m_loginPage->GetUi();

	std::string username = loginUi->inputUsername->text().toUtf8().constData();
	std::string password = std::string(QCryptographicHash::hash(loginUi->inputPassword->text().toUtf8(), QCryptographicHash::Sha256));
	

	if (m_rt->SendLogin(username, password) == true) {
		user.SetUsername(username);
		QMessageBox::information(nullptr, "Title", "Hello " + user.GetUsername());
		emit loginButtonClicked();
		ui->menuUsername->setTitle(user.GetUsername());
		ui->menuBar->show();
		m_userInfo->DisplayHistory(user.GetGameHistory());
		ChangeToLobbyPage();
	}
	else {
		QMessageBox::information(nullptr, "Title", "Wrong credentials.");
	}
}

void Client::HandleRegister() {

	auto loginUi = m_loginPage->GetUi();

	std::string username = loginUi->inputUsername->text().toUtf8().constData();
	std::string password = std::string(QCryptographicHash::hash(loginUi->inputPassword->text().toUtf8(), QCryptographicHash::Sha256));

	if (m_rt->SendRegister(username, password) == true) {
		user.SetUsername(username);

		QMessageBox::information(nullptr, " Title", "Your account has been registered successfully, " + user.GetUsername());
		m_loginPage->ChangeToLoginPage();
		ui->menuUsername->setTitle(user.GetUsername());
		ui->menuBar->show();
		ChangeToLobbyPage();
	}
	else {
		QMessageBox::information(nullptr, "Title", "An account with this username already exist");
	}
}

void Client::HandleCreateLobby()
{
	auto lobbyUiUser = m_loginPage->GetUi();
	user.SetAdmin();

	std::string lobbyName = lobbyUiUser->inputUsername->text().toUtf8().constData();

	bool islobbyCreated = m_rt->SendCreateLobby(lobbyName);
	if (!islobbyCreated) {
		QMessageBox::information(nullptr, "Server Conection Problem", "Lobby not created."); //de revizuit
	}
	else {
		user.SetLobbyCode(m_rt->GetLobbyCode());
		ChangeToWaitingRoom();
	}
}

void Client::HandleJoinLobby()
{
	auto lobbyUi = m_lobbyPage->GetUi();
	std::string lobbyName = lobbyUi->lobbyCode->text().toUtf8().constData();
	user.SetLobbyCode(lobbyName);

	if (!m_rt->SendJoinLobby(lobbyName)) {
		QMessageBox::information(nullptr, "Server Conection Problem", "Lobby not joined.");
		return;
	}
	else {
		ChangeToWaitingRoom();
	}
}

void Client::HandleEndGame()
{
	m_rt->ResetGame();
	ChangeToWaitingRoom();
}

