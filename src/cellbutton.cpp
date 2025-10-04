#include "cellbutton.h"

CellButton::CellButton(const int row, const int col, QWidget* parent, const QColor color) : QPushButton(parent), _row(row), _col(col), _color(color)
{
	connect(this, SIGNAL(clicked()), this, SLOT(handleCellClicked()));

	setColor(_color);

	this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

	qDebug() << "CB-> Cell created:\t[" << row << ";\t" << col << "]";
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
	emit clicked(_row, _col);
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


QSize CellButton::sizeHint() const
{
	return QSize(0, 0);
}