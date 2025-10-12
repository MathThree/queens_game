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
	static mt19937 rng(random_device{}());

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

	shuffle(colors.begin(), colors.end(), rng);
}

void GameModel::tryTogglePlayerValue(const int row, const int col)
{
	if (getFilterValue(grid[row][col].playerValue) == filter)
	{
		Cell *cell = (Cell*) &grid[row][col];
		cell->playerValue = 1 - filter;
		qDebug() << "M -> Cell clicked:\t[" << row << "; " << col << "] -> " << cell->playerValue ;

		updateGrid(row, col);
	}
}

void GameModel::togglePlayerValue(const int row, const int col, const bool left)
{
	Cell *cell = (Cell*) &grid[row][col];
	cell->playerValue = (cell->playerValue + (left ? 1 : 2)) % 3;
	qDebug() << "M -> Cell clicked:\t[" << row << "; " << col << "] -> " << cell->playerValue ;

	updateGrid(row, col);
}

void GameModel::toggleHelp()
{
	help = !help;
	for (int i=0; i<n; ++i)
		for (int j=0; j<n; ++j)
		{
			Cell *c = &grid[i][j];
			if (c->playerValue == 0)
				emit cellUpdated(i, j, getValueToSend(c));
		}
}

void GameModel::updateGrid(const int row, const int col)
{
	switch (grid[row][col].playerValue)
	{
	case 2:
		setQueenToCell(row, col);
		break;
	case 1:
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
			{
				qDebug() << "M -> Value to send:\t" << getValueToSend(&grid[p.first][p.second]);
				emit cellUpdated(p.first, p.second, getValueToSend(&grid[p.first][p.second]));
			}
		}
		if (c->playerValue == 2)
		{
			qDebug() << "QUEEN CONFLICT: [" << row << "; " << col << "] <> [" << p.first << "; " << p.second << "]";
		}
	}
	debug(toQString());
}

void GameModel::setDotToCell(const int row, const int col)
{
	removeQueen(row, col);
	emit cellUpdated(row, col, getValueToSend(&grid[row][col]));
	debug(toQString());
}

void GameModel::setNoneToCell(const int row, const int col)
{
	removeQueen(row, col);
	emit cellUpdated(row, col, getValueToSend(&grid[row][col]));
	debug(toQString());
}

void GameModel::removeQueen(const int row, const int col)
{
	auto it = find(queenList.begin(), queenList.end(), make_pair(row, col));
	if (it != queenList.end())
	{
		queenList.erase(it);
		//if (!grid[row][col].couldHaveQueen)
		qDebug() << "M -> QUEEN CLEAR: [" << row << "; " << col << "] <> [" << "?" << "; " << "?" << "]";

		for (const pair<int, int>& p : getRelatedCells(row, col))
		{
			Cell *c = &grid[p.first][p.second];
			bool b = isQueenInZone(p.first, p.second);
			if (!b)
			{
				c->couldHaveQueen = true;
				if (help)
					emit cellUpdated(p.first, p.second, getValueToSend(&grid[p.first][p.second]));
			}
		}
	}
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
	if (help)
		return cell->couldHaveQueen ? 0 : 3;
	return 0;
}

array<int, 4> GameModel::getBorders(const int row, const int col) const
{
	array<int, 4> borders = {0, 0, 0, 0};
	const Cell *cell = &grid[row][col];
	const array<pair<int, int>, 4> directions = {{{-1, 0}, {0, 1}, {1, 0}, {0, -1}}};
	for (int k=0; k<4; ++k)
	{
		int r = row + directions[k].first;
		int c = col + directions[k].second;
		borders[k] = getBorder(r, c, cell);
	}
	return borders;
}

int GameModel::getBorder(int row, int col, const Cell *cell) const
{
	if (row < 0 || row >= n || col < 0 || col >= n)
		return 4;
	const Cell *other_cell = &grid[row][col];
	if (cell->colorZone != other_cell->colorZone)
		return 2;
	return 1;
}

array<bool, 4> GameModel::getCorners(const int row, const int col) const
{
	array<bool, 4> corners;
	const Cell *cell = &grid[row][col];
	const array<pair<int, int>, 8> directions = {{{-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}}};
	for (int k=0; k<4; ++k)
	{
		array<int, 3> rows;
		array<int, 3> cols;
		for (int l=0; l<3; ++l)
		{
			int index = (2*k + l + 7) % 8;
			rows[l] = row + directions[index].first;
			cols[l] = col + directions[index].second;
		}
		corners[k] = getCorner(rows, cols, cell);
	}
	return corners;
}

bool GameModel::getCorner(const array<int, 3>& rows, const array<int, 3>& cols, const Cell *cell) const
{
	if ((rows[1] != -1 && rows[1] != n) || (cols[1] != -1 && cols[1] != n))
		for (int k=0; k<3; ++k)
		{
			if (rows[k] < 0 || rows[k] >= n || cols[k] < 0 || cols[k] >= n)
				return false;
			if (grid[rows[k]][cols[k]].colorZone == cell->colorZone)
				return false;
		}
	return true;
}

int GameModel::getFilterValue(int playerValue)
{
	return (playerValue == 2) ? 1 : playerValue;
}

QString GameModel::toQString()
{
	QString out = "\n┌────────────\n│   " + QString::number(n) + "\n";

	for (const auto &row : grid)
	{
		for (const auto &cell : row)
		{
			QString hasQueen = cell.hasQueen ? "X" : " ";
			QString playerValue = cell.playerValue == 2 ? "X" : cell.playerValue == 0 ? "_" : ".";
			QString couldHaveQueen = cell.couldHaveQueen ? "_" : ".";
			out += "│   [" + QString::number(cell.colorZone) + "|" + hasQueen + playerValue + couldHaveQueen + "]  ";
		}
		out += "\n";
	}
	out += "└────────────\n";
	return out;
}
