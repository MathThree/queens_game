#include "../include/gridwidget.h"

GridWidget::GridWidget(QWidget *parent) : QWidget(parent)
{
	setMouseTracking(true);
}

void GridWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() && Qt::LeftButton)
	{
		QWidget *w = QApplication::widgetAt(event->globalPos());
		CellButton *cell = qobject_cast<CellButton*>(w);
		if (cell == nullptr) return;

		if (!cell->isHoverActivated())
		{
            if (_startCell == nullptr)
			{
                _startCell = cell;
				startPoint = event->pos();
            } else if ((_startCell != cell) || (event->pos() - startPoint).manhattanLength() > 5)
			{
				cell->setHoverActivated(true);
                _startCell = nullptr;
			}
		}
		if (cell->isHoverActivated() && !cell->hasBeenVisited())
		{
			cell->setVisitID();
			cell->hover();
		}
	}
}

void GridWidget::resizeEvent(QResizeEvent *event)
{
    int margin = event->size().width() * 0.025;
    layout()->setContentsMargins(margin, margin, margin, margin);
    QWidget::resizeEvent(event);
}

void GridWidget::paintEvent(QPaintEvent *event)
{
    QColor primary = TM::instance().getColor("primary");

    int margin = width() * 0.025;
    int cellRadius = _firstCell->getCornerRadius();
    int radius = cellRadius == 0 ? 0 : cellRadius + margin;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(primary);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), radius, radius);

    if (_cells && !_cells->empty() && (this->isEnabled() || TM::instance().getBool("fillCellsInDisabled")))
        if (TM::instance().getBool("fillCellsIn"))
        {
            paintCells(painter);
            paintCornerCells(painter);
            if (!TM::instance().getBool("fillCellsOut"))
                paintFrame(painter);
        }

    QWidget::paintEvent(event);
}

void GridWidget::paintCells(QPainter &painter)
{
    int n = _cells->size();

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            CellButton* cell = (*_cells)[i][j];
            if (!cell) continue;
            if ((i == 0 || i == n-1) && (j == 0 || j == n-1)) continue;

            QRectF rect = cell->geometry();
            QColor cellColor = this->isEnabled() ? cell->getColor().darker(150) : cell->getColor();

            painter.fillRect(rect, cellColor);
        }
    }
}

void GridWidget::paintCornerCells(QPainter &painter)
{
    int n = _cells->size();
    if (n == 1) return;
    float cellSize = (*_cells)[0][0]->geometry().width();
    float half = cellSize * 0.5f;

    for (int i=0; i<n; i+=n-1)
    {
        for (int j=0; j<n; j+=n-1)
        {
            CellButton* cell = (*_cells)[i][j];
            QRectF base = cell->geometry();
            QColor cellColor = this->isEnabled() ? cell->getColor().darker(150) : cell->getColor();

            int x = base.x() + (j == 0 ? half : 0);
            int y = base.y() + (i == 0 ? half : 0);

            painter.fillRect(QRectF(x, base.y(), half, cellSize), cellColor);
            painter.fillRect(QRectF(base.x(), y, cellSize, half), cellColor);
        }
    }
}

void GridWidget::paintFrame(QPainter &painter)
{
    int n = _cells->size();
    CellButton* firstCell = (*_cells)[0][0];
    CellButton* lastCell = (*_cells)[n-1][n-1];
    if (!firstCell || !lastCell) return;

    QRectF outerRect = firstCell->geometry().united(lastCell->geometry());

    float cellSize = firstCell->geometry().width();
    float halfCell = cellSize / 2.0f;

    QRectF innerRect = outerRect.adjusted(halfCell / 2.0f, halfCell / 2.0f, -halfCell / 2.0f, -halfCell / 2.0f);

    QPen pen(TM::instance().getColor("primary"));
    pen.setWidthF(halfCell);
    pen.setJoinStyle(Qt::MiterJoin);

    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(innerRect);
}
