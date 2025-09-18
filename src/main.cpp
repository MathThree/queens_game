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
    presenter.LoadGameFile(level);

    //view.debug("Test\n", false);
    //view.debug("Test2\n", true);

    view.show();
    return a.exec();
}