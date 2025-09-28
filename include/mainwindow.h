#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "cellbutton.h"

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
	void setCell(const int row, const int col, const QColor color);
	void setCellValue(const int row, const int col, const int value);
	void setGameName(const QString gameName);
	void victory();

	void debug(QString newText, bool keep = true);

signals:
	void clicked(const int row, const int col);
	void chooseGameclicked();
	void sendGameFile(QString fileName);

public slots:
	void handleCellClicked(const int row, const int col);

private:
	QString debugText;
	Ui::MainWindow *ui;
	vector<vector<CellButton*>> cells;
	QWidget *gameWidget;
	vector<QString> symbols = {"•", "", "♛"};

	void resizeEvent(QResizeEvent *event);
};
#endif // MAINWINDOW_H