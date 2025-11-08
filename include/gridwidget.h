#ifndef GRIDWIDGET_H
#define GRIDWIDGET_H

#include "cellbutton.h"
#include "thememanager.h"

#include <QWidget>
#include <QMouseEvent>
#include <QDebug>
#include <QApplication>
#include <QDateTime>
#include <QPoint>
#include <QPalette>
#include <QLayout>
#include <QGridLayout>
#include <QLayoutItem>
#include <QPaintEvent>
#include <QPainter>
#include <QColor>
#include <QSize>
#include <QPainterPath>

using TM = ThemeManager;

class GridWidget : public QWidget
{
	Q_OBJECT
public:
	explicit GridWidget(QWidget *parent = nullptr);
    void setFirstCell(CellButton* cell) { _firstCell = cell; }
    void setCells(const vector<vector<CellButton*>>& cells) { _cells = &cells; }

protected:
	void resizeEvent(QResizeEvent *event) override;
	void paintEvent(QPaintEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;

private:
    CellButton *_firstCell = nullptr;
    CellButton *_startCell = nullptr;
    QPoint startPoint;
    const vector<vector<CellButton*>>* _cells;
    float cornerFactor = .7;
    float borderFactor = .03;

	TM *_themeM = &TM::instance();

    void paintCells(QPainter &painter);
    void paintCornerCells(QPainter &painter);
    void paintFrame(QPainter &painter);
};

#endif // GRIDWIDGET_H
