#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "cellbutton.h"
#include "gridcontainer.h"
#include "gridwidget.h"

#include <QMainWindow>
#include <QString>
#include <QWidget>
#include <QGridLayout>
#include <vector>
#include <utility>
#include <QColor>
#include <QScrollBar>
#include <QDebug>
#include <QFileDialog>
#include <QLayoutItem>
#include <QResizeEvent>
#include <QLabel>
#include <QRect>
#include <QShortcut>
#include <QKeySequence>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	void openGameDir(const QString dir);
	void initCellGrid(const int n);
	void setCellGridSize(const int n);
	void setCell(const int row, const int col, const pair<QColor, QColor> colors, const array<int, 4>& borders, const array<bool, 4>& corners);
	void setCellValue(const int row, const int col, const int value);
	void setGameName(const QString gameName);
	void setColorTheme(QColor color) { colorTheme = color; }
	void updateColorTheme();
	void victory();

	void debug(QString newText, bool keep = true);

signals:
	void askFilter(const int row, const int col);
	void clicked(const int row, const int col, const bool left);
	void hovered(const int row, const int col);
	void chooseGameclicked();
	void sendGameFile(QString fileName);
	void askHelp();

public slots:

private:
	QString debugText;
	Ui::MainWindow *ui;
	vector<vector<CellButton*>> cells;
	GridContainer *gameWidget;
	GridWidget *gridWidget;
	vector<QString> symbols = {"", "•", "♛", "·"};
	QColor colorTheme = Qt::black;

	void connectCell(const CellButton *cell);
};
#endif // MAINWINDOW_H