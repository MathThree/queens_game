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
	QColor primary = _themeM->getColor("primary");

    int margin = width() * 0.025;
    int cellRadius = _firstCell->getCornerRadius();
    int radius = cellRadius == 0 ? 0 : cellRadius + margin;

    QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(primary);
    painter.setPen(Qt::NoPen);
	painter.drawRoundedRect(rect(), radius, radius);

	paintCellBackground(painter);
    QWidget::paintEvent(event);
}

void GridWidget::paintCellBackground(QPainter &painter)
{
	painter.setRenderHint(QPainter::Antialiasing, false);

	int n = _cells->size();

	for (int i=0; i<n; ++i)
		for (int j=0; j<n; ++j)
		{
			CellButton* cell = (*_cells)[i][j];

			for (int c=0; c<4; ++c)
			{
				int c_corner = cell->getCorners(c);
				//int n_border = cell->getBorders(c);
				//int p_border = cell->getBorders((c+3)%4);

				if (c_corner == 0 || c_corner == 2)
				{
					pair cDir = cornerDirections[c];
					int halfWidth = (int) round(cell->width() * 0.5f);
					int halfHeight = (int) round(cell->height() * 0.5f);

					/*painter.fillRect(QRect(round(cell->x()) + cDir.second * halfWidth,
										   round(cell->y()) + cDir.first * halfHeight,
										   halfWidth,
										   halfHeight),
									 _themeM->getCellColor(cell->getColorZone(), "base"));*/
				}
				if (c_corner == 1 && _themeM->getBool("fillCorners"))
				{
					pair d = directions[2*c];
					pair cDir = cornerDirections[c];
					int halfWidth = (int) round(cell->width() * 0.5f);
					int halfHeight = (int) round(cell->height() * 0.5f);
					pair<QColor, bool> pairColor = getCornerColor(i, j, c, i+d.first, j+d.second);

					if (pairColor.second)
						painter.fillRect(QRect(round(cell->x()) + cDir.second * halfWidth,
											   round(cell->y()) + cDir.first * halfHeight,
											   halfWidth,
											   halfHeight),
										 pairColor.first);
				}
			}
		}
}

pair<QColor, bool> GridWidget::getCornerColor(int row, int col, int cornerIndex, int otherRow, int otherCol)
{
	int n = _cells->size();
	int r = 2*row + cornerDirections[cornerIndex].first;
	int c = 2*col + cornerDirections[cornerIndex].second;
	bool isREdge = (r == 0 || r == (2*n-1));
	bool isCEdge = (c == 0 || c == (2*n-1));

	if (isREdge && isCEdge) // grid corners
		return make_pair(_themeM->getColor("primary"), false);
	if (isREdge || isCEdge) // grid borders
	{
		if (_themeM->getBool("fillCornersOut"))
			return make_pair(_themeM->getCellColor((*_cells)[row][col]->getColorZone(), isEnabled() ? "cornerOut" : "cornerOutDisabled"), true);
		return make_pair(_themeM->getColor("primary"), false);
	}
	if (_themeM->getBool("fillCornersOwnColor"))
		return make_pair(_themeM->getCellColor((*_cells)[row][col]->getColorZone(), isEnabled() ? "cornerOut" : "cornerOutDisabled"), true);

	CellButton * otherCell = (*_cells)[otherRow][otherCol];

	if (otherCell->getCorners((cornerIndex+2)%4) == 2)
		return make_pair(_themeM->getCellColor(otherCell->getColorZone(), isEnabled() ? "cornerOut" : "cornerOutDisabled"), true);
	return make_pair(_themeM->getColor("primary"), false);
}
