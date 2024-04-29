#include "UserInfo.h"

UserInfo::UserInfo(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::UserInfoClass())
{
	ui->setupUi(this);
}

UserInfo::~UserInfo()
{
	delete ui;
}

void UserInfo::DisplayHistory(const std::vector<GameHistory>& gameHistory)
{

	ui->tableWidget->setRowCount(gameHistory.size());

	int rowIndex = 0;
	for (const auto& history : gameHistory) {
		 
		const auto& [points, date] = history;

		QTableWidgetItem* pointsCell = new QTableWidgetItem(QString::fromUtf8(points.c_str()));
		QTableWidgetItem* dateCell = new QTableWidgetItem(QString::fromUtf8(date.c_str()));

		pointsCell->setFlags(pointsCell->flags() & ~Qt::ItemIsEditable);
		dateCell->setFlags(dateCell->flags() & ~Qt::ItemIsEditable);

		ui->tableWidget->setItem(rowIndex, 0, dateCell);
		ui->tableWidget->setItem(rowIndex, 1, pointsCell);
		rowIndex++;
	}
}