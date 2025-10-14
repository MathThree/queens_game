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
	this->setStyleSheet(QString(
	"QPushButton {"
		"color: %11;"
		"background-color: %1;"
		"border-top: %2px solid %11;"
		"border-right: %3px solid %11;"
		"border-bottom: %4px solid %11;"
		"border-left: %5px solid %11;"
		"border-top-left-radius: %6px;"
		"border-top-right-radius: %7px;"
		"border-bottom-right-radius: %8px;"
		"border-bottom-left-radius: %9px;"
	"}"
	"QPushButton:hover {"
		"color: %11;"
		"border-top: %2px solid %11;"
		"border-right: %3px solid %11;"
		"border-bottom: %4px solid %11;"
		"border-left: %5px solid %11;"
	"}"
	"QPushButton:disabled {"
		"color: %10;"
		"border-top: 0px solid %11;"
		"border-right: 0px solid %11;"
		"border-bottom: 0px solid %11;"
		"border-left: 0px solid %11;"
	"}"
	)
	.arg(_color.name())
	.arg(_borders[0]*width()*0.03)
	.arg(_borders[1]*width()*0.03)
	.arg(_borders[2]*width()*0.03)
	.arg(_borders[3]*width()*0.03)
	.arg(_corners[0] ? cornerValue*cornerFactor : 0)
	.arg(_corners[1] ? cornerValue*cornerFactor : 0)
	.arg(_corners[2] ? cornerValue*cornerFactor : 0)
	.arg(_corners[3] ? cornerValue*cornerFactor : 0)
	.arg(colorTheme.name())
	.arg(borderColor.name())
	);
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