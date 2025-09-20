#include "cellbutton.h"

CellButton::CellButton(int row, int col, QWidget* parent) : QPushButton(parent), _row(row), _col(col)
{
	connect(this, SIGNAL(clicked()), this, SLOT(handleCellClicked()));
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