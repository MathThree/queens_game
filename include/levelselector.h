#ifndef LEVELSELECTOR_H
#define LEVELSELECTOR_H

#include "levelbutton.h"
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
#include <vector>
#include <QStyle>

using namespace std;
using TM = ThemeManager;

namespace Ui {
class LevelSelector;
}

class LevelSelector : public QWidget
{
	Q_OBJECT

public:
	explicit LevelSelector(QWidget *parent = nullptr);
	~LevelSelector();

	void updateDisplay();

signals:
    void sendGameFile(QString fileName);

public slots:
	void handleShowOverlay();

private:
	Ui::LevelSelector *ui;
    QWidget *levelWidget;

	void addLevels();
	void updateButtonSize();

protected:
	void resizeEvent(QResizeEvent *event) override;
};

#endif // LEVELSELECTOR_H
