#include "gamemodel.h"

GameModel::GameModel(QObject *parent) : QObject(parent)
{

}

void GameModel::setSize(const int newSize)
{
    n = newSize;
    grid = vector<vector<Cell>>(n, vector<Cell>(n));
    zones = vector<vector<Cell *>>(n, vector<Cell *>(0));
}

void GameModel::setQueens(const QStringList queenList)
{
    for (int j=0; j<queenList.size(); ++j)
    {
        int i = queenList.at(j).toInt() - 1;
        grid[i][j].hasQueen = true;
    }
}

void GameModel::setZones(const QStringList zoneList)
{
    for (int i=0; i<zoneList.size(); ++i)
    {
        QStringList line = zoneList.at(i).split(" ");
        for (int j=0; j<line.size(); ++j)
        {
            int colorZone = line.at(j).toInt();
            grid[i][j].colorZone = colorZone;
            zones[colorZone-1].push_back(&grid[i][j]);
        }
    }
}

QString GameModel::toQString()
{
    QString out = "Size: " + QString::number(n) + "\n";

    for (const auto &row : grid)
    {
        for (const auto &cell : row)
        {
            QString hasQueen = cell.hasQueen ? "X" : "  ";
            out += " [" + QString::number(cell.colorZone) + "|" + hasQueen + "] ";
        }
        out += "\n";
    }
    out += "\n";
    for (const auto &zone : zones)
    {
        for (const auto &ptr : zone)
        {
            out += "|";
        }
        out += "\n";
    }

    return out;
}