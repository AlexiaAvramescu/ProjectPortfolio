#pragma once

#include <QMainWindow>
#include "ui_LeaderBoardWidget.h"

class LeaderBoardWidget : public QMainWindow
{
	Q_OBJECT

public:
	LeaderBoardWidget(QWidget *parent = nullptr, const std::vector<std::pair<QString, int16_t>>& data = {});
	~LeaderBoardWidget();

private:
	Ui::LeaderBoardWidgetClass ui;
};
