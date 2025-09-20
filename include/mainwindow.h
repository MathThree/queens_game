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

	void initCellGrid(const int n);
	void setCell(const int row, const int col, const QColor color);
	void setCellValue(const int row, const int col, const int value);

	void debug(QString newText, bool keep = true);

signals:
	void clicked(const int row, const int col);

public slots:
	void handleCellClicked(const int row, const int col);

private:
	QString debugText;
	Ui::MainWindow *ui;
	vector<vector<CellButton*>> cells;
	QWidget *gameWidget = nullptr;
};
#endif // MAINWINDOW_H