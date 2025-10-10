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
				qDebug() << "GW-> (0) Mouse in: " << cell->toQString();
				startCell = cell;
				startPoint = event->pos();
			} else if ((startCell != cell) || (event->pos() - startPoint).manhattanLength() > 5)
			{
				qDebug() << "GW-> (2) Mouse in: " << cell->toQString();
				cell->setHoverActivated(true);
				startCell = nullptr;
			}
		}
		if (cell->isHoverActivated() && !cell->hasBeenVisited())
		{
			qDebug() << "GW-> (3) Mouse in: " << cell->toQString();
			cell->setVisitID();
			cell->hover();
		}
	}
}