#include "cellbutton.h"

CellButton::CellButton(int row, int col, const QColor color, QWidget* parent) : QPushButton(parent), _row(row), _col(col), _color(color)
{
	connect(this, SIGNAL(clicked()), this, SLOT(handleCellClicked()));

	this->setStyleSheet(QString(
							"background-color: %1;"
							"border: 1px solid black;"
							"border-radius: 0px").arg(color.name()));

	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void CellButton::setCellValue(const int value)
{
	switch (value)
	{
	case -1:
		this->setText("•");
		break;
	case 1:
		this->setText("♛");
		break;
	default:
		this->setText("");
	}
}

void CellButton::handleCellClicked()
{
	emit clicked(_row, _col);
}