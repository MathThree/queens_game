#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	connect(ui->chooseGameButton, &QPushButton::clicked, this, &MainWindow::chooseGameclicked);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::openGameDir(const QString dir)
{
	qDebug().noquote() << dir;
	QString filePath = QFileDialog::getOpenFileName(this, "Select a game file", dir, "Fichiers de jeu (*.txt);;Tous les fichiers (*)");
	if (!filePath.isEmpty())
	{
		QFileInfo info(filePath);
		QString fileName = info.fileName();
		emit sendGameFile(fileName);
	}
}

void MainWindow::initCellGrid(const int n)
{
	gameWidget = new QWidget(this);
	//ui->gameGrid->setParent(gameWidget);
	gameWidget->setMinimumSize(330, 330);
	gameWidget->setLayout(ui->gameGrid);
	//cells = vector<vector<CellButton*>>(n, vector<CellButton*>(n));
	setCellGridSize(n);
}

void MainWindow::setCellGridSize(const int n)
{
	int old_n = cells.size();
	if (old_n > n)
	{
		for (int i=0; i<old_n; ++i)
		{
			for (int j=0; j<old_n; ++j)
			{
				CellButton *cell = cells[i][j];
				if (i >= n || j >= n)
					cell->deleteLater();
		}
			if (i<n)
				cells[i].resize(n);
		}
		cells.resize(n);
	}
	else if (old_n < n)
	{
		cells.resize(n);
		for (int i=0; i<n; ++i)
		{
			if (i<old_n)
				cells[i].resize(n);
			else
				cells[i] = vector<CellButton*>(n);
			for (int j=0; j<n; ++j)
			{
				if (i>=old_n || j>=old_n)
					cells[i][j] = new CellButton(i, j, gameWidget);
			}
		}
	}
}

void MainWindow::setCell(const int row, const int col, const QColor color)
{
	if (row<0 || row>=cells.size()) return;
	if (col<0 || col>=cells[row].size()) return;
	qDebug() << "V: SetCell: " << row << "; " << col;

	CellButton *cell = cells[row][col];
	cell->setColor(color);
	setCellValue(row, col, 0);
	if (!ui->gameGrid->itemAtPosition(row, col))
	{
		qDebug() << "HERE: " << row << "; " << col;
		ui->gameGrid->addWidget(cell, row, col);
		connect(cell, SIGNAL(clicked(int,int)), this, SLOT(handleCellClicked(int,int)));
	}
}

void MainWindow::setCellValue(const int row, const int col, const int value)
{
	qDebug() << "V: [" << row << "; " << col << "] -> " << value;
	((CellButton *) cells[row][col])->setCellValue(symbols[value+1]);
}

void MainWindow::victory()
{
	qDebug() << "V: VICTORY!";
	gameWidget->setEnabled(false);
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
	QScrollBar *sb = ui->debugText->verticalScrollBar();
	sb->setValue(sb->maximum());
}
