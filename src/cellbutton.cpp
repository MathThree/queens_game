#include "cellbutton.h"

CellButton::CellButton(const int row, const int col, QWidget* parent, const QColor color) : QPushButton(parent), _row(row), _col(col), _color(color)
{
	//connect(this, &CellButton::clicked, this, &CellButton::handleCellClicked);

	updateDisplay();

	this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

	qDebug() << "CB-> Cell created:\t[" << row << ";\t" << col << "]";
}

CellButton::CellButton(QWidget *parent) : QPushButton(parent)
{
	_row = 0;
	_col = 0;
	_color = QColor("white");
	//connect(this, &CellButton::clicked, this, &CellButton::handleCellClicked);

	updateDisplay();

	this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

void CellButton::resetCellButton(int row, int col, const QColor color)
{
	_color = color;
	setColor(_color);
	setCellValue("");
}

void CellButton::setCellValue(const QString value)
{
	this->setText(value);
}

void CellButton::handleCellClicked()
{
	emit clicked(_row, _col, true);
}

void CellButton::setColor(const QColor color)
{
	_color = color;
}

void CellButton::setBorders(const array<int, 4> borders)
{
	_borders = borders;
}

void CellButton::updateDisplay()
{
	this->setStyleSheet(QString(
		"background-color: %1;"
		"border-top: %2px solid black;"
		"border-right: %3px solid black;"
		"border-bottom: %4px solid black;"
		"border-left: %5px solid black;"
		"border-radius: 0px")
		.arg(_color.name())
		.arg(_borders[0])
		.arg(_borders[1])
		.arg(_borders[2])
		.arg(_borders[3])
	);
}

void CellButton::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
		emit clicked(_row, _col, true);
	else if (event->button() == Qt::RightButton)
		emit clicked(_row, _col, false);
}

void CellButton::resizeEvent(QResizeEvent *event)
{
	QPushButton::resizeEvent(event);
	QFont f = font();
	f.setPointSize(event->size().height() / 3);
	setFont(f);
}