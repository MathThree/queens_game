#ifndef GRIDWIDGET_H
#define GRIDWIDGET_H

#include "cellbutton.h"
#include "thememanager.h"

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
#include <QSize>

using TM = ThemeManager;

class GridWidget : public QWidget
{
	Q_OBJECT
public:
	explicit GridWidget(QWidget *parent = nullptr);
	void setFirstCell(CellButton* cell) { firstCell = cell; }

protected:
	void resizeEvent(QResizeEvent *event) override;
	void paintEvent(QPaintEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;

private:
	CellButton *firstCell = nullptr;
	CellButton *startCell = nullptr;
	QPoint startPoint;
};

#endif // GRIDWIDGET_H
