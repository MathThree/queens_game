#include "gamemodel.h"

GameModel::GameModel(QObject *parent) : QObject(parent), offsets{-1, 1}
{
}

void GameModel::loadGameFile(QString gameName)
{
	QFile file("../queens_game/games/" + gameName);

	if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QFileInfo info(file);
	emit sendGameName(info.completeBaseName());

	QTextStream in(&file);
	QString line;

	line = in.readLine();
	setSize(line.toInt());

	line = in.readLine();
	QStringList numbers = line.split(QRegularExpression("\\s+"), QString::SkipEmptyParts);
	setQueens(numbers);

	line = in.readAll();
	QStringList zone_list = line.split("\n");
	setZones(zone_list);

	qDebug() << "M -> Game loaded:\t" << gameName;
}

void GameModel::setSize(const int newSize)
{
	n = newSize;
	grid = vector<vector<Cell>>(n, vector<Cell>(n));
	zones = vector<vector<pair<int, int>>>(n, vector<pair<int, int>>(0));
	queenList.clear();
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
		QStringList line = zoneList.at(i).split(QRegularExpression("\\s+"), QString::SkipEmptyParts);
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

	static mt19937 rng(random_device{}());
	shuffle(colors.begin(), colors.end(), rng);
}

void GameModel::togglePlayerValue(const int row, const int col)
{
	Cell *cell = (Cell*) &grid[row][col];
	cell->playerValue = (cell->playerValue == 0 && !cell->couldHaveQueen && help) ? 1 : (cell->playerValue + 3) % 3 - 1;
	qDebug() << "M -> Cell clicked:\t[" << row << "; " << col << "] -> " << cell->playerValue ;

	switch (cell->playerValue)
	{
	case 1:
		setQueenToCell(row, col);
		break;
	case -1:
		setDotToCell(row, col);
		break;
	default:
		setNoneToCell(row, col);
	}

	if (isVictory())
	{
		qDebug() << "M -> ##### VICTORY! #####";
		debug("M -> ##### VICTORY! #####");
		emit victory();
	}
}

void GameModel::setQueenToCell(const int row, const int col)
{
	queenList.push_back(make_pair(row, col));
	emit cellUpdated(row, col, getValueToSend(&grid[row][col]));
	for (const pair<int, int>& p : getRelatedCells(row, col))
	{
		Cell *c = &grid[p.first][p.second];
		if (c->couldHaveQueen)
		{
			c->couldHaveQueen = false;
			if (help)
				emit cellUpdated(p.first, p.second, getValueToSend(&grid[p.first][p.second]));
		}
	}
	debug(toQString());
}

void GameModel::setDotToCell(const int row, const int col)
{
	emit cellUpdated(row, col, getValueToSend(&grid[row][col]));
	debug(toQString());
}

void GameModel::setNoneToCell(const int row, const int col)
{
	auto it = find(queenList.begin(), queenList.end(), make_pair(row, col));
	if (it != queenList.end())
		queenList.erase(it);
	emit cellUpdated(row, col, getValueToSend(&grid[row][col]));
	for (const pair<int, int>& p : getRelatedCells(row, col))
	{
		Cell *c = &grid[p.first][p.second];
		bool b = isQueenInZone(p.first, p.second);
		if (!b)
		{
			c->couldHaveQueen = !b;
			if (help)
				emit cellUpdated(p.first, p.second, getValueToSend(&grid[p.first][p.second]));
		}
	}
	debug(toQString());
}

bool GameModel::isQueenInZone(const int row, const int col)
{
	bool b1 = isQueenInColorZone(row, col);
	bool b2 = isQueenInRow(row, col);
	bool b3 = isQueenInColumn(row, col);
	bool b4 = isQueenInKingZone(row, col);
	bool b = b1 || b2 || b3 || b4;
	qDebug() << (QString("N -> Queen in zone:\t%1 - %2  %3 || %4 || %5 || %6 = %7").arg(row+1).arg(col+1).arg(b1).arg(b2).arg(b3).arg(b4).arg(b));
	return (b);
}

bool GameModel::isQueenInColorZone(const int row, const int col)
{
	int colorZone = grid[row][col].colorZone;
	for (const pair<int, int>& p : queenList)
		if (p.first != row && p.second != col && grid[p.first][p.second].colorZone == colorZone)
			return true;
	return false;
}

bool GameModel::isQueenInRow(const int row, const int col)
{
	for (const pair<int, int>& p : queenList)
		if (p.second != col && p.first == row)
			return true;
	return false;
}

bool GameModel::isQueenInColumn(const int row, const int col)
{
	for (const pair<int, int>& p : queenList)
		if (p.first != row && p.second == col)
			return true;
	return false;
}

bool GameModel::isQueenInKingZone(const int row, const int col)
{
	for (const pair<int, int>& p : queenList)
		if (max(abs(row - p.first), abs(col - p.second)) == 1)
			return true;
	return false;
}

bool GameModel::isVictory()
{
	if (queenList.size() != n)
		return false;
	for (const auto &p : queenList)
	{
		if (!grid[p.first][p.second].couldHaveQueen)
			return false;
	}
	return true;
}

set<pair<int, int>> GameModel::getRelatedCells(const int row, const int col, const bool addTarget)
{
	set<pair<int, int>> relatedCells;
	for (pair<int, int> p : zones[grid[row][col].colorZone])
	{
		relatedCells.insert(make_pair(p.first, p.second));
	}
	for (int i=0; i<n; ++i)
	{
		relatedCells.insert(make_pair(i, col));
		relatedCells.insert(make_pair(row, i));
	}
	for (int dx : offsets)
		for (int dy : offsets)
		{
			int x = row + dx;
			int y = col + dy;
			if (0<=x && x<n && 0<=y && y<n)
				relatedCells.insert(make_pair(x, y));
		}
	if (!addTarget)
	{
		relatedCells.erase(make_pair(row, col));
	}
	return relatedCells;
}

int GameModel::getValueToSend(const Cell *cell) const
{
	if (cell->playerValue != 0)
		return cell->playerValue;
	if (help && cell->playerValue == 0)
		return cell->couldHaveQueen ? 0 : -1 ;
	return cell->playerValue;
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
			QString couldHaveQueen = cell.couldHaveQueen ? "_" : ".";
			out += " [" + QString::number(cell.colorZone) + "|" + hasQueen + playerValue + couldHaveQueen + "]  ";
		}
		out += "\n";
	}
	out += "\n";
	return out;
}
