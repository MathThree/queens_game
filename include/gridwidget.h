#ifndef GRIDWIDGET_H
#define GRIDWIDGET_H

#include <cellbutton.h>

#include <QWidget>
#include <QMouseEvent>
#include <QDebug>
#include <QApplication>
#include <QDateTime>
#include <QPoint>

class GridWidget : public QWidget
{
	Q_OBJECT
public:
	explicit GridWidget(QWidget *parent = nullptr);

protected:
	void mouseMoveEvent(QMouseEvent *event) override;

private:
	CellButton *startCell = nullptr;
	QPoint startPoint;
};

#endif // GRIDWIDGET_H
