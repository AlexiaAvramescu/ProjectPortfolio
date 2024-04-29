#include "LeaderBoardWidget.h"
#include <QTableWidget>
#include <QHeaderView>

LeaderBoardWidget::LeaderBoardWidget(QWidget *parent, const std::vector<std::pair<QString, int16_t>>& data)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	ui.leaderBoard->clearContents();
	ui.leaderBoard->setRowCount(0);
	ui.leaderBoard->horizontalHeader()->setStretchLastSection(true);
	ui.leaderBoard->setSelectionMode(QAbstractItemView::NoSelection);

	int row;
	for (const auto& pair : data) {
		int row = ui.leaderBoard->rowCount();
		ui.leaderBoard->setRowCount(row + 1);

		QTableWidgetItem* playerNameItem = new QTableWidgetItem(pair.first);
		QTableWidgetItem* scoreItem = new QTableWidgetItem(QString::number(pair.second));

		playerNameItem->setTextAlignment(Qt::AlignCenter);
		scoreItem->setTextAlignment(Qt::AlignCenter);

		playerNameItem->setFlags(playerNameItem->flags() & ~Qt::ItemIsEditable);
		scoreItem->setFlags(scoreItem->flags() & ~Qt::ItemIsEditable);

		ui.leaderBoard->setItem(row, 0, playerNameItem);
		ui.leaderBoard->setItem(row, 1, scoreItem);
	}
	
}

LeaderBoardWidget::~LeaderBoardWidget()
{
}
