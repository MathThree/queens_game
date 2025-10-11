#include "../include/gridwidget.h"

GridWidget::GridWidget(QWidget *parent) : QWidget(parent)
{
	setMouseTracking(true);
}

void GridWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton)
	{
		QWidget *w = QApplication::widgetAt(event->globalPos());
		CellButton *cell = qobject_cast<CellButton*>(w);
		if (cell == nullptr) return;

		if (!cell->isHoverActivated())
		{
			if (startCell == nullptr)
			{
				startCell = cell;
				startPoint = event->pos();
			} else if ((startCell != cell) || (event->pos() - startPoint).manhattanLength() > 5)
			{
				cell->setHoverActivated(true);
				startCell = nullptr;
			}
		}
		if (cell->isHoverActivated() && !cell->hasBeenVisited())
		{
			cell->setVisitID();
			cell->hover();
		}
	}
}