#include "mainwindow.h"
#include "gamepresenter.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow gameView;
    GamePresenter gamePresenter;



    gameView.show();
    return a.exec();
}
