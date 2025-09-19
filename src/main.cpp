#include "mainwindow.h"
#include "gamemodel.h"
#include "gamepresenter.h"

#include <QDebug>
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow view;
	GameModel model;
	GamePresenter presenter(nullptr, &model, &view);

	QString level = "001.txt";
	presenter.loadGameFile(level);
	presenter.initCells();

	view.show();
	return a.exec();
}