#include "gamepresenter.h"

GamePresenter::GamePresenter(GameModel *model, MainWindow *view, QObject *parent) : QObject(parent), _model(model), _view(view)
{
	connect(_view, SIGNAL(clicked(int,int)), this, SLOT(handleCellClicked(int,int)));

	connect(_model, SIGNAL(cellUpdated(int,int,int,int)), this, SLOT(handleCellUpdated(int,int,int,int)));
}

void GamePresenter::initCells()
{
	_view->initCellGrid(_model->getSize());
	for (int i=0; i<_model->getSize(); ++i)
		for(int j=0; j<_model->getSize(); ++j)
		{
			_view->setCell(i, j, _model->getColor(i, j));
		}
}

void GamePresenter::handleCellClicked(const int row, const int col)
{
	_view->debug(QString("[%1; %2] : ").arg(row+1).arg(col+1));
	_model->togglePlayerValue(row, col);
}

void GamePresenter::handleCellUpdated(const int row, const int col, const int playerValue, const int bonusValue)
{
	_view->debug(QString("[%1; %2] : ").arg(row+1).arg(col+1));
	_view->setCellValue(row, col, playerValue);
	_view->debug(QString("[-> %1]\n").arg(playerValue));
}

/*
void GamePresenter::toggleCellValue(const int row, const int col)
{
	int value = _model->togglePlayerValue(row, col);
}
*/