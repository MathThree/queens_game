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

void CellButton::resetCellButton(int row, int col, const QColor color)
{
	//_color = color;
	setCellValue("");
}

void CellButton::updateDisplay()
{
	QString qss = TM::instance().getStyle("cell");

	qss.replace("%BORDER_TOP%", QString::number(_borders[0] * width() * 0.03));
	qss.replace("%BORDER_RIGHT%", QString::number(_borders[1] * width() * 0.03));
	qss.replace("%BORDER_BOTTOM%", QString::number(_borders[2] * width() * 0.03));
	qss.replace("%BORDER_LEFT%", QString::number(_borders[3] * width() * 0.03));

	qss.replace("%CORNER_RADIUS_TL%", QString::number(_corners[0] ? cornerValue * cornerFactor : 0));
	qss.replace("%CORNER_RADIUS_TR%", QString::number(_corners[1] ? cornerValue * cornerFactor : 0));
	qss.replace("%CORNER_RADIUS_BR%", QString::number(_corners[2] ? cornerValue * cornerFactor : 0));
	qss.replace("%CORNER_RADIUS_BL%", QString::number(_corners[3] ? cornerValue * cornerFactor : 0));

	qss.replace("%CELL_COLOR%", _color.name());
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