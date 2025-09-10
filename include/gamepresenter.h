#ifndef GAMEPRESENTER_H
#define GAMEPRESENTER_H

#include <QObject>

class GamePresenter : public QObject
{
    Q_OBJECT
public:
    GamePresenter(QObject *parent = nullptr);
signals:

};

#endif // GAMEPRESENTER_H
