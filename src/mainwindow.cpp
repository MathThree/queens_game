#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::initCells(const int n)
{
	QWidget *gameWidget = new QWidget(this);
	ui->gameGrid->setParent(gameWidget);
	cells = vector<vector<CellButton*>>(n, vector<CellButton*>(n));
	for (int i=0; i<n; ++i)
		for (int j=0; j<n; ++j)
		{
			CellButton *cell = new CellButton(i, j);
			ui->gameGrid->addWidget(cell, i, j);
			cells[i][j] = cell;
			connect(cell, SIGNAL(clicked(int,int)), this, SLOT(handleCellClicked(int,int)));
		}

	gameWidget->setMinimumSize(330, 330);
}

void MainWindow::setCell(const int row, const int col, const QColor color)
{
	if (row<0 || row>=cells.size()) return;
	if (col<0 || col>=cells[row].size()) return;

	CellButton *cell = cells[row][col];

	if (!cell) return;

	cell->setStyleSheet(QString(
							"background-color: %1;"
							"border: 1px solid black;"
							"border-radius: 0px").arg(color.name()));

	cell->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void MainWindow::handleCellClicked(const int row, const int col)
{
	debug(QString("Clicked (V): %1; %2\n").arg(row+1).arg(col+1));
	emit clicked(row, col);
}

void MainWindow::debug(QString newText, bool keep)
{
	debugText = !keep ? newText : debugText + newText;
	ui->debugText->setText(debugText);
}