#include "mainwindow.h"
#include "gamemodel.h"
#include "gamepresenter.h"
#include "thememanager.h"

#include <QDebug>
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QDebug>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//QTranslator translator;

	//translator.load("../queens_game/qm/queens_game_fr_FR.qm");
	//a.installTranslator(&translator);

	ThemeManager::instance().applyTheme();

	MainWindow view;
    GameModel model;
    GamePresenter presenter(&model, &view);

	QString level = "001.txt";
    model.loadGameFile(level);
    view.initCellGrid(model.getSize());
    view.setCellGridSize(model.getSize());
    presenter.initCells();

	view.show();
    view.updateGridWidget();
	return a.exec();
}
