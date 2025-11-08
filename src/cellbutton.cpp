#include "cellbutton.h"

CellButton::CellButton(const int row, const int col, QWidget* parent, const QColor color) : QPushButton(parent), _row(row), _col(col), _color(color)
{
	updateDisplay();

	this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

	qDebug() << "CB-> Cell created:\t[" << row << ";\t" << col << "]";
}

CellButton::CellButton(QWidget *parent) : QPushButton(parent)
{
	_row = 0;
	_col = 0;
	_color = QColor("white");

	updateDisplay();

	this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

qint64 CellButton::globalVisitID = 0;

bool CellButton::hoverActivated = false;

void CellButton::resetCellButton()
{
	setCellValue("");
	inConflict = false;
}

void CellButton::updateDisplay()
{
    QString qss = TM::instance().getStyle("cell");
    cornerFactor = TM::instance().getFloat("cornerFactor");
	borderFactor = TM::instance().getFloat("borderFactor");
	disableBorderFactor = TM::instance().getFloat("disableBorderFactor");

	qss.replace("%BORDER_TOP%",    QString::number(_borders[0] * width() * 0.125 * borderFactor));
	qss.replace("%BORDER_RIGHT%",  QString::number(_borders[1] * width() * 0.125 * borderFactor));
	qss.replace("%BORDER_BOTTOM%", QString::number(_borders[2] * width() * 0.125 * borderFactor));
	qss.replace("%BORDER_LEFT%",   QString::number(_borders[3] * width() * 0.125 * borderFactor));

	qss.replace("%BORDER_TOP_DISABLED%",    QString::number(_borders[0] == 1 ? 0 : _borders[0] * width() * 0.125 * disableBorderFactor));
	qss.replace("%BORDER_RIGHT_DISABLED%",  QString::number(_borders[1] == 1 ? 0 : _borders[1] * width() * 0.125 * disableBorderFactor));
	qss.replace("%BORDER_BOTTOM_DISABLED%", QString::number(_borders[2] == 1 ? 0 : _borders[2] * width() * 0.125 * disableBorderFactor));
	qss.replace("%BORDER_LEFT_DISABLED%",   QString::number(_borders[3] == 1 ? 0 : _borders[3] * width() * 0.125 * disableBorderFactor));

	qss.replace("%CORNER_RADIUS_TL%", QString::number(_corners[0] == 1 ? cornerValue * cornerFactor : 0));
	qss.replace("%CORNER_RADIUS_TR%", QString::number(_corners[1] == 1 ? cornerValue * cornerFactor : 0));
	qss.replace("%CORNER_RADIUS_BR%", QString::number(_corners[2] == 1 ? cornerValue * cornerFactor : 0));
	qss.replace("%CORNER_RADIUS_BL%", QString::number(_corners[3] == 1 ? cornerValue * cornerFactor : 0));

	qss.replace("%CELL_COLOR%", _color.name());
	qss.replace("%TEXT_COLOR%", inConflict ? QColor(255, 0, 0).name() : _color.darker(250).name());
	qss.replace("%CELL_DARK%", _color.darker(150).name());

	this->setStyleSheet(qss);
}

void CellButton::resizeEvent(QResizeEvent *event)
{
	QPushButton::resizeEvent(event);
	QFont f = font();
	f.setPointSize(event->size().height() / 3);
	setFont(f);
	cornerValue = event->size().height() * 0.49;
	updateDisplay();
}

void CellButton::paintEvent(QPaintEvent *event)
{
	QPushButton::paintEvent(event);

	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing);
	p.setPen(Qt::NoPen);

	QString source = TM::instance().getString("cornerColor");
	QColor fillColor = (source == "cell_dark") ? _color.darker(150) : TM::instance().getColor(source);

	p.setBrush(fillColor);

	float w = width();
	float radius = w * 3 * 0.125 * (this->isEnabled() ? borderFactor : disableBorderFactor);
	const array<pair<int, int>, 4> cornerPositions = {{{0, 0}, {w, 0}, {w, w}, {0, w}}};

	for (int i=0; i<4; ++i)
	{
		if (_corners[i] == 2)
		{
			QRectF cornerRect(cornerPositions[i].first-radius, cornerPositions[i].second-radius, double(radius * 2.0), double(radius * 2.0));
			if (TM::instance().getFloat("cornerFactor") != 0.0)
				p.drawPie(cornerRect, -90 * i * 16, -90 * 16);
			else
				p.drawRect(cornerRect);
		}
	}
}

void CellButton::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		emit askFilter(_row, _col);
		globalVisitID = QDateTime::currentMSecsSinceEpoch();
	}
}

void CellButton::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
		if (!hasBeenVisited())
			emit clicked(_row, _col, true);
		else
			setHoverActivated(false);
	else if (event->button() == Qt::RightButton)
		emit clicked(_row, _col, false);
	globalVisitID = 0;
}

QString CellButton::toQString()
{
	QString out = "[" + QString::number(_row) + "; " + QString::number(_col) + "]";
	return out;
}
