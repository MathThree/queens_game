#ifndef GRIDWIDGET_H
#define GRIDWIDGET_H

#include <cellbutton.h>

#include <QWidget>
#include <QMouseEvent>
#include <QDebug>
#include <QApplication>
#include <QDateTime>
#include <QPoint>
#include <QPalette>
#include <QLayout>
#include <QGridLayout>
#include <QLayoutItem>
#include <QPaintEvent>
#include <QPainter>
#include <QColor>

class GridWidget : public QWidget
{
	Q_OBJECT
public:
	explicit GridWidget(QWidget *parent = nullptr);
	void setFirstCell(CellButton* cell) { firstCell = cell; }
	void setColorTheme(QColor color) { colorTheme = color; }

protected:
	void resizeEvent(QResizeEvent *event) override;
	void paintEvent(QPaintEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;

private:
	CellButton *firstCell = nullptr;
	CellButton *startCell = nullptr;
	QPoint startPoint;
	QColor colorTheme = Qt::black;
};

#endif // GRIDWIDGET_H
