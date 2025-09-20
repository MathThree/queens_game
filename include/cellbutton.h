#ifndef CELLBUTTON_H
#define CELLBUTTON_H

#include <QPushButton>
#include <QString>
#include <QColor>

class CellButton : public QPushButton
{
	Q_OBJECT
public:
	CellButton(int row, int col, const QColor color = QColor("white"), QWidget* parent = nullptr);
	void setCellValue(const QString value);
	void setColor(const QColor color);

signals:
	void clicked(const int row, const int col);

public slots:
	void handleCellClicked();

private:
	int _row;
	int _col;
	QColor _color;
};

#endif // CELLBUTTON_H