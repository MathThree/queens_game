#include "../include/gridcontainer.h"

GridContainer::GridContainer(QWidget *parent) : QWidget(parent)
{
}

void GridContainer::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);

	child = findChild<QWidget*>("gridWidget");
	if (!child) return;

	int side = qMin(width(), height());

	int x = (width() - side) / 2;
	int y = (height() - side) / 2;

	child->setGeometry(x, y, side, side);
}