#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <QObject>
#include <vector>
#include <utility>
#include <QStringList>

using namespace std;

class GameModel : public QObject
{
    Q_OBJECT
public:
    GameModel(QObject *parent = nullptr);

    void setSize(const int newSize);
    void setQueens(const QStringList queenList);
    void setZones(const QStringList zoneList);

    QString toQString();

private:
    struct Cell
    {
        bool hasQueen = false;
        int colorZone = 0;
        int playerValue = 0;
        int bonusValue = 0;
    };

    int n;
    vector<vector<Cell>> grid;
    vector<vector<Cell*>> zones;

signals:

};

#endif // GAMEMODEL_H
