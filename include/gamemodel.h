#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <QObject>
#include <vector>
#include <list>
#include <set>
#include <tuple>
#include <utility>
#include <QStringList>
#include <QColor>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDebug>

using namespace std;

class GameModel : public QObject
{
	Q_OBJECT
public:
	GameModel(QObject *parent = nullptr);

	void setSize(const int newSize);
	void setQueens(const QStringList queenList);
	void setZones(const QStringList zoneList);
	void setColors();

	int getSize() const { return n; }
	int getPlayerValue(const int row, const int col) const { return grid[row][col].playerValue; }
	QColor getColor(const int row, const int col) const {return colors[grid[row][col].colorZone];}

	void togglePlayerValue(const int row, const int col);

	void loadGameFile(QString gameName);

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
	vector<vector<pair<int, int>>> zones;
	list<tuple<int, int, int>> queenList;
	vector<QColor> colors;
	int offsets[2];
	bool help = false;

	void setQueenToCell(const int row, const int col, const Cell *cell);
	void setDotToCell(const int row, const int col, const Cell *cell);
	void setNoneToCell(const int row, const int col, const Cell *cell);

	bool isQueenInZone(const int zone);
	bool isQueenInRow(const int row);
	bool isQueenInColumn(const int col);
	bool isQueenInKingZone(const int row, const int col);

	set<tuple<int, int, int>> getRelatedCells(const int row, const int col, const int zone, const bool addTarget = false);
	int getValueToSend(const int playerValue, const int bonusValue) const;

signals:
	void cellUpdated(const int row, const int col, const int value);
	void debug(const QString debugText, const bool keep = true);
};

#endif // GAMEMODEL_H
