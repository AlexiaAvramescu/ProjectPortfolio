#include "ChessUIQt.h"
#include <QtWidgets/QApplication>

#include <QDialog>
#include <QInputDialog>

#include <vector>

EGameType ConvertToGameType(QString item)
{
	if (item == "Rapid")
		return EGameType::Rapid;
	if (item == "Blitz")
		return EGameType::Blitz;
	if (item == "Bullet")
		return EGameType::Bullet;
	if (item == "No timer")
		return EGameType::NoTimer;
}

EGameType GetGameType()
{
	QList<QString> options;
	options.append("Rapid");
	options.append("Blitz");
	options.append("Bullet");
	options.append("No timer");

	while (true)
	{
		QInputDialog dialog;
		dialog.setComboBoxItems(options);
		dialog.setModal(true);
		int result = dialog.exec();

		// Process the user's selection
		if (result == QDialog::Accepted)
		{
			QString selectedGameType = dialog.textValue(); // Get the selected piece (e.g., "Rook", "Bishop", etc.)

			
			return ConvertToGameType(selectedGameType);
		}
	}
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    auto ui = std::make_shared<ChessUIQt>();

    // game timer type

    ui->show();
	auto gameType = GetGameType();
    auto game = IGame::Produce(gameType);

    game->AddListener(ui);

    ui->SetGame(game);

    ui->StartGame();

    QObject::connect(ui.get(), &ChessUIQt::Exit, &a, &QApplication::quit);

    auto result = a.exec();
    game->RemoveListener(ui.get());
    return result;
}
