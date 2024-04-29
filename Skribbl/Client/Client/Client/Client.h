#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QStackedWidget>
#include <QMessageBox>

#include "ui_Client.h"
#include "LoginPage.h"
#include "GamePage.h"
#include "LobbyPage.h"
#include "UserInfo.h"
#include "WaitingRoom.h"
#include "User.h"

#include "Routing.h"


class Client : public QMainWindow
{
	Q_OBJECT

public:
	Client(QWidget* parent = nullptr);
	~Client() = default;

	void closeEvent(QCloseEvent* event) override;

private slots:
	void ChangeToLoginPage();
	void ChangeToGamePage();
	void ChangeToLobbyPage();
	void ChangeToWaitingRoom();
	void ShowUserInfo();

	void HandleAnswer();
	void HandleLogin();
	void HandleRegister();
	void HandleCreateLobby();
	void HandleJoinLobby();
	void HandleEndGame();
	void ExitGame();

private:
	bool ValidInput();
	User user;

private:
	Ui::ClientClass* ui;
	std::unique_ptr<QStackedWidget> m_stackedWidget;
	std::unique_ptr <LoginPage> m_loginPage;
	std::unique_ptr <GamePage> m_gamePage;
	std::unique_ptr <LobbyPage> m_lobbyPage;
	std::unique_ptr <UserInfo> m_userInfo;
	std::unique_ptr <WaitingRoom> m_waitingRoom;

	std::shared_ptr<Routing> m_rt;

signals:
	void loginButtonClicked();

};

#endif