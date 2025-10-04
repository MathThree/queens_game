#ifndef CELLBUTTON_H
#define CELLBUTTON_H

#include <QPushButton>
#include <QString>
#include <QColor>
#include <QDebug>
#include <QResizeEvent>

using namespace std;

class CellButton : public QPushButton
{
	Q_OBJECT
public:
	CellButton(const int row, const int col, QWidget* parent = nullptr, const QColor color = QColor("white"));
	void setCellValue(const QString value);
	void setColor(const QColor color);
	void setBorders(const array<int, 4> borders);
	void updateDisplay();
	void resetCellButton(int row, int col, const QColor color = QColor("white"));
	QSize sizeHint() const override;

signals:
	void clicked(const int row, const int col);

public slots:
	void handleCellClicked();

private:
	int _row;
	int _col;
	QColor _color;
	array<int, 4> _borders;

};

#endif // CELLBUTTON_H