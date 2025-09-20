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

void MainWindow::initCellGrid(const int n)
{
	if (!gameWidget)
	{
		gameWidget = new QWidget(this);
		ui->gameGrid->setParent(gameWidget);
		gameWidget->setMinimumSize(330, 330);
	}
	if (n<cells.size())
	{
		for (int i=n; i<cells.size(); ++i)
			for(int j=n; j<cells[i].size(); ++j)
				cells[i][j]->deleteLater();
	}
	cells = vector<vector<CellButton*>>(n, vector<CellButton*>(n));
}

void MainWindow::setCell(const int row, const int col, const QColor color)
{
	if (row<0 || row>=cells.size()) return;
	if (col<0 || col>=cells[row].size()) return;

	CellButton *cell = new CellButton(row, col, color, gameWidget);
	cells[row][col] = cell;
	ui->gameGrid->addWidget(cell, row, col);
	connect(cell, SIGNAL(clicked(int,int)), this, SLOT(handleCellClicked(int,int)));
}

void MainWindow::setCellValue(const int row, const int col, const int value)
{
	((CellButton *) cells[row][col])->setCellValue(symbols[value+1]);
}

void MainWindow::handleCellClicked(const int row, const int col)
{
	//debug(QString("Clicked (V): %1; %2\n").arg(row+1).arg(col+1));
	emit clicked(row, col);
}

void MainWindow::debug(QString newText, bool keep)
{
	debugText = !keep ? newText : debugText + newText;
	ui->debugText->setText(debugText);
}
