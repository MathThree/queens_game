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
#include <QFileInfo>
#include <QTextStream>
#include <QString>
#include <QDebug>
#include <QRegularExpression>
#include <random>

using namespace std;

/**
 * @brief GameModel manages the logic and data of the Queen Game.
 */
class GameModel : public QObject
{
	Q_OBJECT
public:
	/**
	 * @brief Constructor
	 * @param parent QObject parent (default nullptr)
	 */
	GameModel(QObject *parent = nullptr);

	/**
	 * @brief Get game directory
	 * @return Directory name (QString)
	 */
	QString getGameDir() const { return gameDir; }

	/**
	 * @brief Get grid size
	 * @return Grid size (int)
	 */
	int getSize() const { return n; }

	/**
	 * @brief Get player value of a cell
	 * @param row Row index of the cell (int)
	 * @param col Column index of the cell (int)
	 * @return Current player value (int)
	 */
	int getPlayerValue(const int row, const int col) const { return grid[row][col].playerValue; }

	/**
	 * @brief Get cell color
	 * @param row Row index of the cell (int)
	 * @param col Column index of the cell (int)
	 * @return Color associated with the cell's zone (QColor)
	 */
	pair<QColor, QColor> getColors(const int row, const int col) const { return make_pair(cellColors[grid[row][col].colorZone], borderColors[grid[row][col].colorZone]); }

	array<int, 4> getBorders(const int row, const int col) const;

	array<bool, 4> getCorners(const int row, const int col) const;

	void setFilter(const int row, const int col) { filter = getFilterValue(grid[row][col].playerValue); }

	void tryTogglePlayerValue(const int row, const int col);

	/**
	 * @brief Toggle cell value
	 * @param row Row index of the cell (int)
	 * @param col Column index of the cell (int)
	 * @param left True if it is a left click, right click otherwise (bool)
	 */
	void togglePlayerValue(const int row, const int col, const bool left);

	void toggleHelp();

	/**
	 * @brief Load game file
	 * @param gameName Name of the game file (QString)
	 */
	void loadGameFile(QString gameName);

	/**
	 * @brief String representation for debug
	 * @return Current grid state (QString)
	 */
	QString toQString();

private:
	/**
	 * @brief Data of a single cell
	 */
	struct Cell
	{
		bool hasQueen = false;       ///< True if a queen is placed by the player (bool) (default false)
		int colorZone = 0;           ///< Zone index (int) (default 0)
		int playerValue = 0;         ///< Value placed by the player (int) (default 0)
		bool couldHaveQueen = true;  ///< True if a queen could be placed by the player according to game rules (bool (default true)
	};

	QString gameDir = QString("../queens_game/games/"); ///< Game directory name
	int n;                                              ///< Grid size (n*n)
	vector<vector<Cell>> grid;                          ///< Square grid storing current state
	vector<vector<pair<int, int>>> zones;               ///< Cells grouped by zone
	list<pair<int, int>> queenList;                     ///< Positions of queens placed by the player
	vector<QColor> cellColors;                          ///< Colors per zone
	vector<QColor> borderColors;
	int offsets[2];                                     ///< Helper array {-1, 1} for diagonals
	bool help = false;                                  ///< Show help dots if true
	int filter = 0;

	/**
	 * @brief Initialize colors according to number of zones
	 */
	void setColors();

	/**
	 * @brief Set grid size
	 * @param newSize New size of the square grid (int)
	 */
	void setSize(const int newSize);

	/**
	 * @brief Set queens from input
	 * @param queenList List of strings of numbers with queen data (QStringList)
	 */
	void setQueens(const QStringList queenList);

	/**
	 * @brief Set zones from input
	 * @param zoneList List of strings of numbers with zone data (QStringList)
	 */
	void setZones(const QStringList zoneList);

	/**
	 * @brief Place a queen in a cell
	 * @param row Row index of the cell (int)
	 * @param col Column index of the cell (int)
	 */
	void setQueenToCell(const int row, const int col);

	/**
	 * @brief Place a dot in a cell
	 * @param row Row index of the cell (int)
	 * @param col Column index of the cell (int)
	 */
	void setDotToCell(const int row, const int col);

	/**
	 * @brief Clear a cell
	 * @param row Row index of the cell (int)
	 * @param col Column index of the cell (int)
	 */
	void setNoneToCell(const int row, const int col);

	/**
	 * @brief Remove a queen (if exists) and update concerned cells
	 * @param row Row index of the cell (int)
	 * @param col Column index of the cell (int)
	 */
	void removeQueen(const int row, const int col);

	/**
	 * @brief Check if a queen exists in a cell zone, ignoring the cell itself
	 * @param row Row index of the cell (int)
	 * @param col Column index of the cell (int)
	 * @return True if a queen is present (bool)
	 */
	bool isQueenInZone(const int row, const int col);

	/**
	 * @brief Check if a queen exists in a color zone of a cell, ignoring the cell itself
	 * @param row Row index of the cell (int)
	 * @param col Column index of the cell (int)
	 * @return True if a queen is present (bool)
	 */
	bool isQueenInColorZone(const int row, const int col);

	/**
	 * @brief Check if a queen exists in a row of a cell, ignoring the cell itself
	 * @param row Row index of the cell (int)
	 * @param col Column index of the cell (int)
	 * @return True if a queen is present (bool)
	 */
	bool isQueenInRow(const int row, const int col);

	/**
	 * @brief Check if a queen exists in a column of a cell, ignoring the cell itself
	 * @param row Row index of the cell (int)
	 * @param col Column index of the cell (int)
	 * @return True if a queen is present (bool)
	 */
	bool isQueenInColumn(const int row, const int col);

	/**
	 * @brief Check if a queen is in the king zone around a cell (only corners)
	 * @param row Row index of the cell (int)
	 * @param col Column index of the cell (int)
	 * @return True if a queen is present (bool)
	 */
	bool isQueenInKingZone(const int row, const int col);

	/**
	 * @brief Check if all queens are placed to the right cells according to the game rules
	 * @return True if there is a victory
	 */
	bool isVictory();

	/**
	 * @brief Get related cells (same zone/row/col/corners)
	 * @param row Row index of the cell (int)
	 * @param col Column index of the cell (int)
	 * @param addTarget Include the target cell if true (bool, default false)
	 * @return Related cells (set<pair<int,int>>)
	 */
	set<pair<int, int>> getRelatedCells(const int row, const int col, const bool addTarget = false);

	/**
	 * @brief Compute value sent to the view
	 * @param cell Cell pointer (Cell*)
	 * @return Computed value (int)
	 */
	int getValueToSend(const Cell *cell) const;

	int getBorder(int row, int col, const Cell *cell) const;

	bool getCorner(const array<int, 3>& rows, const array<int, 3>& cols, const Cell *cell) const;

	int getFilterValue(int playerValue);

	void updateGrid(const int row, const int col);

signals:
	void sendGameName(const QString row);

	/**
	 * @brief Emitted when a cell is updated
	 * @param row Row index of the cell (int)
	 * @param col Colum index of the cell (int)
	 * @param value New cell value (int)
	 */
	void cellUpdated(const int row, const int col, const int value);

	void victory();

	/**
	 * @brief Debug signal
	 * @param debugText Debug message (QString)
	 * @param keep Append if true, replace if false (bool, default true)
	 */
	void debug(const QString debugText, const bool keep = true);
};

#endif // GAMEMODEL_H
