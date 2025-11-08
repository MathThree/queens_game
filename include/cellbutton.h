#ifndef CELLBUTTON_H
#define CELLBUTTON_H

#include "thememanager.h"

#include <QPushButton>
#include <QString>
#include <QColor>
#include <QDebug>
#include <QResizeEvent>
#include <QFont>
#include <QApplication>
#include <QDateTime>
#include <QPaintEvent>
#include <QPainter>
#include <QColor>
#include <QStyle>
#include <QRegularExpression>

using namespace std;
using TM = ThemeManager;

class CellButton : public QPushButton
{
	Q_OBJECT
public:
	explicit CellButton(QWidget *parent = nullptr);
	CellButton(const int row, const int col, QWidget* parent = nullptr, const QColor color = QColor("white"));
	void setCellValue(const QString value) { currentValue = value; updateValueDisplay(); }
	void setColors(const pair<QColor, QColor> colors) { _color = colors.first; borderColor = colors.second; }
	void setBorders(const array<int, 4>& borders) { _borders = borders; }
	void setCorners(const array<int, 4>& corners) { _corners = corners; }
	void setVisitID() { lastVisitID = globalVisitID; }
	void setHoverActivated(bool hoverValue) { hoverActivated = hoverValue; }
	void setInConflict(const bool conflict) { inConflict = conflict; }
	float getCornerRadius() { return cornerValue * cornerFactor; }
    const QColor getColor() { return _color; }
    const int getBorders(int index) { return _borders[index]; }
	const int getCorners(int index) { return _corners[index]; }
	bool isHoverActivated() { return hoverActivated; }
	void updateDisplay();
	void updateValueDisplay() { setText(TM::instance().getString(currentValue)); }
	void resetCellButton();
	void click() { emit clicked(_row, _col, true); };
	void hover() { emit hovered(_row, _col); }
	bool hasBeenVisited() { return (globalVisitID == lastVisitID); }
	QString toQString();

signals:
	void askFilter(const int row, const int col);
	void clicked(const int row, const int col, const bool left=true);
	void hovered(const int row, const int col);

public slots:

private:
	int _row;
	int _col;
	QString currentValue;
	QColor _color;
	QColor borderColor;
	array<int, 4> _borders;
	array<int, 4> _corners;
	qint64 lastVisitID = 0;
	static qint64 globalVisitID;
	static bool hoverActivated;
	int cornerValue;
	float cornerFactor;
	float borderFactor;
	float disableBorderFactor;
	bool inConflict = false;

	TM *_themeM = &TM::instance();

protected:
	void resizeEvent(QResizeEvent *event) override;
	void paintEvent(QPaintEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

};

#endif // CELLBUTTON_H
