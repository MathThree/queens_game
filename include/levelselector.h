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
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

using namespace std;
using TM = ThemeManager;

namespace Ui {
class LevelSelector;
}

class LevelSelector : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(float animationFactor READ getAnimationFactor WRITE setAnimationFactor NOTIFY update)

public:
	explicit LevelSelector(QWidget *parent = nullptr);
	~LevelSelector();

	float getAnimationFactor() { return animationFactor; }
	void setAnimationFactor(float value) { animationFactor = value; updateLayout(); }
	void updateDisplay();

signals:
    void sendGameFile(QString fileName);

public slots:
	void handleShowOverlay();

private:
	Ui::LevelSelector *ui;
    QWidget *levelWidget;
	float animationFactor = 0.0f;
	QGraphicsOpacityEffect *effect;

	void addLevels();
	void updateButtonSize();
	void updateLayout();
	void animationClicked(bool backward = false);

protected:
	void resizeEvent(QResizeEvent *event) override;
};

#endif // LEVELSELECTOR_H
