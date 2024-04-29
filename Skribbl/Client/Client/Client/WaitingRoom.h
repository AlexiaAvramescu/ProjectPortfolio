#pragma once

#include <QWidget>
#include <QTimer.h>
#include <QListWidgetItem>
#include <QPaintEvent>
#include <QMessageBox>

#include "Routing.h"
#include "ui_WaitingRoom.h"
#include "User.h"
#include "UserWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class WaitingRoomClass; };
QT_END_NAMESPACE

class WaitingRoom : public QWidget
{
	Q_OBJECT

public:
	WaitingRoom(QWidget *parent = nullptr, std::shared_ptr<Routing> rt = nullptr);
	~WaitingRoom();
	void SetRoomCode(QString lobbyCode);
	void AddUserToRoom(const QString& user);
	void UpdatePlayerList(const std::vector<QString>& players);
	void FetchPlayers();
	void ResetButtons();

	void paintEvent(QPaintEvent* e) override;

private slots:
	void OnStartBtnPushed();
	void AddCustomWord();

signals:
	void GoToGame();
	void ExitGame();

private:
	Ui::WaitingRoomClass *ui;
	std::shared_ptr<Routing> m_rt;

	static const uint8_t kRefreshRate{ 2 };
	uint m_refreshCount;

	QString m_admin;
	const QString kWaitingState{ "Waiting" };
};
