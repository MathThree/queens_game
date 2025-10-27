#ifndef STARTOVERLAY_H
#define STARTOVERLAY_H

#include "thememanager.h"

#include <QWidget>
#include <QDebug>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QDir>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QColor>
#include <QScrollBar>
#include <QStyle>
#include <utility>
#include <QRadioButton>
#include <QFont>
#include <QTimer>

using namespace std;
using TM = ThemeManager;

namespace Ui {
class StartOverlay;
}

class StartOverlay : public QWidget
{
	Q_OBJECT

public:
    explicit StartOverlay(QWidget *parent = nullptr);
    ~StartOverlay();

    void updateDisplay();

public slots:
	void handleShowOverlay();

private:
    Ui::StartOverlay *ui;

protected:
	void resizeEvent(QResizeEvent *event) override;
};

#endif // STARTOVERLAY_H
