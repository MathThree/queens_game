#ifndef GAMEPRESENTER_H
#define GAMEPRESENTER_H

#include <QObject>
#include <QFile>
#include <QByteArray>
#include "gamemodel.h"
#include "mainwindow.h"

class GamePresenter : public QObject
{
    Q_OBJECT
public:
    GamePresenter(QObject *parent = nullptr, GameModel *model = nullptr, MainWindow *view = nullptr);

    void LoadGameFile(QString gameName);

private:
    GameModel *_model;
    MainWindow *_view;

signals:

};

#endif // GAMEPRESENTER_H
