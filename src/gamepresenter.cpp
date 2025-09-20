#include "gamepresenter.h"

GamePresenter::GamePresenter(QObject *parent, GameModel *model, MainWindow *view) : QObject(parent)
{
    _model = model;
    _view = view;
}

void GamePresenter::loadGameFile(QString gameName)
{
	QFile file("../queens_game/games/" + gameName);

	if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QTextStream in(&file);
	QString line;

	line = in.readLine();
	_model->setSize(line.toInt());

	line = in.readLine();
	QStringList numbers = line.split(" ");
	_model->setQueens(numbers);

	line = in.readAll();
	QStringList zone_list = line.split("\n");
	_model->setZones(zone_list);

	//_view->debug(_model->toQString());
}

void GamePresenter::initCells()
{
	connect(_view, SIGNAL(clicked(int,int)), this, SLOT(handleCellClicked(int,int)));
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
	toggleCellValue(row, col);
}

void GamePresenter::toggleCellValue(const int row, const int col)
{
	_model->togglePlayerValue(row, col);
	_view->setCellValue(row, col, _model->getPlayerValue(row, col));
	_view->debug(QString("[-> %1]\n").arg(_model->getPlayerValue(row, col)));
}