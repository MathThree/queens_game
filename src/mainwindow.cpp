#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QTranslator>

MainWindow::MainWindow(QWidget *parent)	: QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	connect(ui->chooseGameButton, &QPushButton::clicked, this, &MainWindow::chooseGameclicked);
	connect(ui->settingsButton, &QPushButton::clicked, this, &MainWindow::askHelp);

	QShortcut *toggleDebug = new QShortcut(QKeySequence(Qt::Key_F3), this);
	connect(toggleDebug, &QShortcut::activated, this, [this]() {
		ui->debugText->setVisible(!ui->debugText->isVisible());
	});

	ui->debugText->setVisible(false);

	gameWidget = ui->gameWidget;
	gridWidget = ui->gridWidget;

	gridWidget->setFirstCell(ui->firstCell);

	setColorTheme(QColor(55, 55, 98));

	setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(Qt::FramelessWindowHint);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::openGameDir(const QString dir)
{
	QString filePath = QFileDialog::getOpenFileName(this, "Select a game file", dir, "Game files (*.txt);;All files (*)");
	if (!filePath.isEmpty())
	{
		QFileInfo info(filePath);
		QString fileName = info.fileName();
		emit sendGameFile(fileName);
	}
}

void MainWindow::initCellGrid(const int n)
{
	cells.resize(1);
	cells[0].resize(1);
	cells[0][0] = ui->firstCell;
	connectCell(ui->firstCell);
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
	gameWidget->setEnabled(true);
	gameWidget->update();
	updateColorTheme();
}

void MainWindow::setCell(const int row, const int col, const pair<QColor, QColor> colors, const array<int, 4>& borders, const array<bool, 4>& corners)
{
	if (row<0 || row>=cells.size()) return;
	if (col<0 || col>=cells[row].size()) return;

	CellButton *cell = cells[row][col];
	cell->setColors(colors);
	cell->setBorders(borders);
	cell->setCorners(corners);
	cell->updateDisplay();
	setCellValue(row, col, 0);
	if (!ui->gameGrid->itemAtPosition(row, col))
	{
		ui->gameGrid->addWidget(cell, row, col);
		connectCell(cell);
	}
}

void MainWindow::setCellValue(const int row, const int col, const int value)
{
	qDebug() << "V -> Cell value:\t\t[" << row << "; " << col << "] -> " << value;
	((CellButton *) cells[row][col])->setCellValue(symbols[value]);
}

void MainWindow::setGameName(const QString gameName)
{
	ui->gameName->setText(gameName);
}

void MainWindow::updateColorTheme()
{
	gridWidget->setColorTheme(colorTheme);
	for (auto &row : cells)
	{
		for (auto &cell : row)
		{
			cell->setColorTheme(colorTheme);
			cell->updateDisplay();
		}
	}
}

void MainWindow::connectCell(const CellButton *cell)
{
	connect(cell, &CellButton::clicked, this, &MainWindow::clicked);
	connect(cell, &CellButton::askFilter, this, &MainWindow::askFilter);
	connect(cell, &CellButton::hovered, this, &MainWindow::hovered);
}

void MainWindow::victory()
{
	qDebug() << "V -> VICTORY!";
	gameWidget->setEnabled(false);
}

void MainWindow::debug(QString newText, bool keep)
{
	debugText = !keep ? newText : debugText + newText;
	ui->debugText->setText(debugText);
	QScrollBar *sb = ui->debugText->verticalScrollBar();
	sb->setValue(sb->maximum());
}
