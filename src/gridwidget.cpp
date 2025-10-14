#include "../include/gridwidget.h"

GridWidget::GridWidget(QWidget *parent) : QWidget(parent)
{
	setMouseTracking(true);

	/*QPalette pal = palette();
	pal.setColor(QPalette::Window, Qt::black);
	setPalette(pal);*/
}

void GridWidget::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);
	//setStyleSheet(QString("border-radius: %1px;").arg(firstCell->getCornerRadius()));
}

void GridWidget::paintEvent(QPaintEvent *event)
{
	int margin = width() * 0.025;
	layout()->setContentsMargins(margin, margin, margin, margin);
	int radius = firstCell->getCornerRadius() + margin;

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setBrush(colorTheme);
	painter.setPen(Qt::NoPen);
	painter.drawRoundedRect(rect(), radius, radius);
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
