#pragma once

#include <QWidget>
#include "ui_LobbyPage.h"
#include "Routing.h"

QT_BEGIN_NAMESPACE
namespace Ui { class LobbyClass; };
QT_END_NAMESPACE

class LobbyPage : public QWidget
{
	Q_OBJECT

public:
	LobbyPage(QWidget *parent = nullptr);
	~LobbyPage();

	Ui::LobbyClass* GetUi();

private slots:
	void ChangeToLoginPage();
	void ChangeToGamePage();
	void OnJoinLobbyClicked();
	void OnCreateLobbyClicked();

signals:
	void goToLoginPage();
	void goToGamePage();
	void SendCreateLobbyToServer();  // send to server de revizuit
	void SendJoinLobbyToServer();
private:
	Ui::LobbyClass *ui;
};
