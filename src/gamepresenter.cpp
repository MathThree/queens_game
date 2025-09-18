#include "gamepresenter.h"

GamePresenter::GamePresenter(QObject *parent, GameModel *model, MainWindow *view) : QObject(parent)
{
    _model = model;
    _view = view;
}

void GamePresenter::LoadGameFile(QString gameName)
{
    QFile file("../queens_game/games/" + gameName);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString line;

    line = in.readLine();
    _model->setSize(line.toInt());

    line = in.readLine();
    QStringList numbers = line.split(" ");
    _model->setQueens(numbers);

    line = in.readAll();
    QStringList zone_list = line.split("\n");
    _model->setZones(zone_list);
    //_view->debug(zone_list[0]);

    _view->debug(_model->toQString());
    /*
    while (!in.atEnd())
    {
        line = in.readLine();
        _view->debug(line, true);
    }
    */


}