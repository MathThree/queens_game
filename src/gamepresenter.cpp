#include "gamepresenter.h"

GamePresenter::GamePresenter(GameModel *model, MainWindow *view, QObject *parent) : QObject(parent), _model(model), _view(view)
{
	connect(_view, &MainWindow::clicked, this, &GamePresenter::handleCellClicked);
	connect(_view, &MainWindow::chooseGameclicked, this, &GamePresenter::handleChooseGame);
	connect(_view, &MainWindow::sendGameFile, this, &GamePresenter::handleGetGameFile);
	connect(_view, &MainWindow::askFilter, this, &GamePresenter::handleAskFilter);
	connect(_view, &MainWindow::hovered, this, &GamePresenter::handleCellHovered);
	connect(_view, &MainWindow::askHelp, this, &GamePresenter::handleAskHelp);

	connect(_model, &GameModel::debug, this, &GamePresenter::handleModelDebug);
	connect(_model, &GameModel::sendGameName, this, &GamePresenter::handleGetGameName);
	connect(_model, &GameModel::cellUpdated, this, &GamePresenter::handleCellUpdated);
	connect(_model, &GameModel::victory, this, &GamePresenter::handleVictory);
}

void GamePresenter::initCells()
{
	for (int i=0; i<_model->getSize(); ++i)
		for(int j=0; j<_model->getSize(); ++j)
			_view->setCell(i, j, _model->getColors(i, j), _model->getBorders(i, j), _model->getCorners(i, j));
}

void GamePresenter::handleCellClicked(const int row, const int col, const bool left)
{
	_model->togglePlayerValue(row, col, left);
}

void GamePresenter::handleModelDebug(const QString debugText, const bool keep)
{
	_view->debug(debugText, keep);
}

void GamePresenter::handleCellUpdated(const int row, const int col, const int value)
{
	_view->setCellValue(row, col, value);
	_view->debug(QString("P -> Cell updated: [%1; %2] -> %3\n").arg(row).arg(col).arg(value));
}

void GamePresenter::handleVictory()
{
	_view->victory();
}

void GamePresenter::handleChooseGame()
{
	_view->openGameDir(_model->getGameDir());
}

void GamePresenter::handleGetGameFile(const QString fileName)
{
	_model->loadGameFile(fileName);
	_view->setCellGridSize(_model->getSize());
	initCells();
}

void GamePresenter::handleGetGameName(const QString gameName)
{
	_view->setGameName(gameName);
}

void GamePresenter::handleAskFilter(const int row, const int col)
{
	_model->setFilter(row, col);
}

void GamePresenter::handleCellHovered(const int row, const int col)
{
	_model->tryTogglePlayerValue(row, col);
}

void GamePresenter::handleAskHelp()
{
	_model->toggleHelp();
}