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

using namespace std;
using TM = ThemeManager;

namespace Ui {
class SettingsOverlay;
}

class SettingsOverlay : public QWidget
{
	Q_OBJECT

public:
	explicit SettingsOverlay(QWidget *parent = nullptr);
	~SettingsOverlay();

	void setColorTheme(vector<QColor> color) { colorTheme = color; }
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

	void addColors();
	void updateButtonSize();

protected:
	void resizeEvent(QResizeEvent *event) override;
};

#endif // SETTINGSOVERLAY_H
