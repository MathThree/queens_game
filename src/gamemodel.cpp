#include "gamemodel.h"

GameModel::GameModel(QObject *parent) : QObject(parent), offsets{-1, 1}
{
}

void GameModel::loadGameFile(QString gameName)
{
	QFile file("../queens_game/games/" + gameName);

	if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QTextStream in(&file);
	QString line;

	line = in.readLine();
	setSize(line.toInt());

	line = in.readLine();
	QStringList numbers = line.split(" ");
	setQueens(numbers);

	line = in.readAll();
	QStringList zone_list = line.split("\n");
	setZones(zone_list);
}

void GameModel::setSize(const int newSize)
{
	n = newSize;
	grid = vector<vector<Cell>>(n, vector<Cell>(n));
	zones = vector<vector<pair<int, int>>>(n, vector<pair<int, int>>(0));
	setColors();
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
			grid[i][j].colorZone = colorZone-1;
			zones[colorZone-1].push_back(pair<int, int>(i, j));
		}
	}
}

void GameModel::setColors()
{
	colors = vector<QColor>(n, QColor());
	double h = -.5 / n;
	double s = .62;
	double v = .78;
	double offset = .5 / n;
	for (int i=0; i<colors.size(); ++i)
	{
		h += 2. * offset;
		colors[i] = QColor::fromHsvF(h, s, v);
	}
}

void GameModel::togglePlayerValue(const int row, const int col)
{
	Cell *cell = (Cell*) &grid[row][col];
	cell->playerValue = (cell->playerValue == 0 && cell->bonusValue == -1 && help) ? 1 : (cell->playerValue + 3) % 3 - 1;
	qDebug() << "M: [" << row << "; " << col << "]";
	emit cellUpdated(row, col, getValueToSend(cell->playerValue, cell->bonusValue));

	switch (cell->playerValue)
	{
	case 1:
		setQueenToCell(row, col, cell);
		break;
	case -1:
		setDotToCell(row, col, cell);
		break;
	default:
		setNoneToCell(row, col, cell);
	}
}

void GameModel::setQueenToCell(const int row, const int col, const Cell *cell)
{
	queenList.push_back(make_tuple(row, col, cell->colorZone));
	for (const tuple<int, int, int>& t : getRelatedCells(row, col, cell->colorZone))
	{
		Cell *c = &grid[get<0>(t)][get<1>(t)];
		if (c->bonusValue != -1)
		{
			c->bonusValue = -1;
			if (help)
				emit cellUpdated(get<0>(t), get<1>(t), getValueToSend(c->playerValue, c->bonusValue));
		}
	}
	debug(toQString());
}

void GameModel::setDotToCell(const int row, const int col, const Cell *cell)
{
	debug(toQString());
}

void GameModel::setNoneToCell(const int row, const int col, const Cell *cell)
{
	auto it = find(queenList.begin(), queenList.end(), make_tuple(row, col, cell->colorZone));
	if (it != queenList.end())
		queenList.erase(it);
	for (const tuple<int, int, int>& t : getRelatedCells(row, col, cell->colorZone))
	{
		Cell *c = &grid[get<0>(t)][get<1>(t)];
		bool b1 = isQueenInZone(c->colorZone);
		bool b2 = isQueenInRow(get<0>(t));
		bool b3 = isQueenInColumn(get<1>(t));
		bool b4 = isQueenInKingZone(get<0>(t), get<1>(t));
		debug(QString("N :\t%1 - %2  %3 %4 %5 %6\n").arg(get<0>(t)+1).arg(get<1>(t)+1).arg(b1).arg(b2).arg(b3).arg(b4));
		if (!b1 && !b2 && !b3 && !b4)
		{
			c->bonusValue = 0;
			if (help)
				emit cellUpdated(get<0>(t), get<1>(t), getValueToSend(c->playerValue, c->bonusValue));
		}
	}
	debug(toQString());
}

bool GameModel::isQueenInZone(const int zone)
{
	for (const tuple<int, int, int>& t : queenList)
		if (get<2>(t) == zone)
			return true;
	return false;
}

bool GameModel::isQueenInRow(const int row)
{
	for (const tuple<int, int, int>& t : queenList)
		if (get<0>(t) == row)
			return true;
	return false;
}

bool GameModel::isQueenInColumn(const int col)
{
	for (const tuple<int, int, int>& t : queenList)
		if (get<1>(t) == col)
			return true;
	return false;
}

bool GameModel::isQueenInKingZone(const int row, const int col)
{
	for (const tuple<int, int, int>& t : queenList)
		if (max(abs(row - get<0>(t)), abs(col - get<1>(t))) == 1)
			return true;
	return false;
}

set<tuple<int, int, int>> GameModel::getRelatedCells(const int row, const int col, const int zone, const bool addTarget)
{
	set<tuple<int, int, int>> relatedCells;
	for (pair<int, int> p : zones[zone])
	{
		relatedCells.insert(make_tuple(p.first, p.second, grid[p.first][p.second].colorZone));
	}
	for (int i=0; i<n; ++i)
	{
		relatedCells.insert(make_tuple(i, col, grid[i][col].colorZone));
		relatedCells.insert(make_tuple(row, i, grid[row][i].colorZone));
	}
	for (int dx : offsets)
		for (int dy : offsets)
		{
			int x = row + dx;
			int y = col + dy;
			if (0<=x && x<n && 0<=y && y<n)
				relatedCells.insert(make_tuple(x, y, grid[x][y].colorZone));
		}
	if (!addTarget)
	{
		relatedCells.erase(make_tuple(row, col, zone));
	}
	return relatedCells;
}

int GameModel::getValueToSend(const int playerValue, const int bonusValue) const
{
	if (playerValue != 0)
		return playerValue;
	if (help && playerValue == 0)
		return bonusValue;
	return playerValue;
}

QString GameModel::toQString()
{
	QString out = " " + QString::number(n) + "\n";

	for (const auto &row : grid)
	{
		for (const auto &cell : row)
		{
			QString hasQueen = cell.hasQueen ? "X" : " ";
			QString playerValue = cell.playerValue == 1 ? "X" : cell.playerValue == 0 ? "_" : ".";
			QString bonusValue = cell.bonusValue == 1 ? "X" : cell.bonusValue == 0 ? "_" : ".";
			out += " [" + QString::number(cell.colorZone) + "|" + hasQueen + playerValue + bonusValue + "]  ";
		}
		out += "\n";
	}
	out += "\n";
	/*for (const auto &zone : zones)
	{
		for (const auto &ptr : zone)
		{
			out += "|";
		}
		out += "\n";
	}*/

	return out;
}
