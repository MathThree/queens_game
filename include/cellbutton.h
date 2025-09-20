#ifndef CELLBUTTON_H
#define CELLBUTTON_H

#include <QPushButton>
#include <QString>

class CellButton : public QPushButton
{
	Q_OBJECT
public:
	CellButton(int row, int col, QWidget* parent = nullptr);
	void setCellValue(const int value);

signals:
	void clicked(const int row, const int col);

public slots:
	void handleCellClicked();

private:
	int _row;
	int _col;
};

#endif // CELLBUTTON_H