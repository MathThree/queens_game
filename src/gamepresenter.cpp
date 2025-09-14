#include "gamepresenter.h"

GamePresenter::GamePresenter(QObject *parent, GameModel *model, MainWindow *view) : QObject(parent)
{
    _model = model;
    _view = view;
}

void GamePresenter::LoadGameFile(QString gameName)
{
    QFile file("../queens_game/games/" + gameName);

    _view->debug(file.open(QIODevice::ReadOnly | QIODevice::Text) ? "TRUE\n" : "FALSE\n", true);

    while (!file.atEnd())
    {
        QByteArray line = file.readLine();
        _view->debug(line, true);
    }
}