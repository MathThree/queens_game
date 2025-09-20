#include "cellbutton.h"

CellButton::CellButton(int row, int col, const QColor color, QWidget* parent) : QPushButton(parent), _row(row), _col(col), _color(color)
{
	connect(this, SIGNAL(clicked()), this, SLOT(handleCellClicked()));

	setColor(_color);

	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
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
	this->setStyleSheet(QString(
							"background-color: %1;"
							"border: 1px solid black;"
							"border-radius: 0px").arg(color.name()));
}