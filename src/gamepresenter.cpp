#include "gamepresenter.h"

GamePresenter::GamePresenter(GameModel *model, MainWindow *view, QObject *parent) : QObject(parent), _model(model), _view(view)
{
	connect(_view, &MainWindow::chooseGameclicked,   this, &GamePresenter::handleChooseGame);
	connect(_view, &MainWindow::sendGameFile,        this, &GamePresenter::handleGetGameFile);
	connect(_view, &MainWindow::clicked,           _model, &GameModel::togglePlayerValue);
	connect(_view, &MainWindow::askFilter,         _model, &GameModel::setFilter);
	connect(_view, &MainWindow::hovered,           _model, &GameModel::tryTogglePlayerValue);
	connect(_view, &MainWindow::askHelp,           _model, &GameModel::toggleHelp);

	connect(_model, &GameModel::debug,              _view, &MainWindow::debug);
	connect(_model, &GameModel::sendGameName,       _view, &MainWindow::setGameName);
	connect(_model, &GameModel::cellUpdated,        _view, &MainWindow::setCellValue);
	connect(_model, &GameModel::victory,            _view, &MainWindow::victory);
	connect(_model, &GameModel::sendConflictValue,  _view, &MainWindow::sendConflictValue);


}

void GamePresenter::initCells()
{
	for (int i=0; i<_model->getSize(); ++i)
		for(int j=0; j<_model->getSize(); ++j)
			_view->setCell(i, j, _model->getColors(i, j), _model->getBorders(i, j), _model->getCorners(i, j));
}

void GamePresenter::handleChooseGame()
{
	_view->openGameDir(_model->getGameDir());
}

void GamePresenter::handleGetGameFile(const QString filePath)
{
    _model->loadGameFile(filePath);
	_view->setCellGridSize(_model->getSize());
	initCells();
	_view->updateGridWidget();
}
