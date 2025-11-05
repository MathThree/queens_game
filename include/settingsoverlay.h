#ifndef SETTINGSOVERLAY_H
#define SETTINGSOVERLAY_H

#include "themebutton.h"
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
#include <utility>
#include <QStyle>
#include <QRadioButton>
#include <QGraphicsOpacityEffect>

using namespace std;
using TM = ThemeManager;

namespace Ui {
class SettingsOverlay;
}

class SettingsOverlay : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(float animationFactor READ getAnimationFactor WRITE setAnimationFactor NOTIFY update)

public:
	explicit SettingsOverlay(QWidget *parent = nullptr);
	~SettingsOverlay();

	void setColorTheme(vector<QColor> color) { colorTheme = color; }
	float getAnimationFactor() { return animationFactor; }
	void setAnimationFactor(float value) { animationFactor = value; updateLayout(); }
	void updateDisplay();

signals:
	void sendGameFile(QString fileName);
	void updateThemeDisplay();
	void askHelp();

public slots:
	void handleShowOverlay();

private:
	Ui::SettingsOverlay *ui;
	QWidget *colorsWidget;
	vector<QColor> colorTheme;
	float animationFactor = 0.0f;
	QGraphicsOpacityEffect *effect;

	void addColors();
	void updateButtonSize();
	void updateLayout();
	void animationClicked(bool backward = false);

protected:
	void resizeEvent(QResizeEvent *event) override;
};

#endif // SETTINGSOVERLAY_H
