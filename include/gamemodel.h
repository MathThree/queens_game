#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <QObject>
#include <vector>
#include <utility>
#include <QStringList>
#include <QColor>

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
	void setPlayerValue(const int row, const int col);

	int getSize() const { return n; }
	QColor getColor(const int row, const int col) const {return colors[grid[row][col].colorZone];}

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
	vector<QColor> colors;

signals:

};

#endif // GAMEMODEL_H
