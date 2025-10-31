#ifndef THEMEBUTTON_H
#define THEMEBUTTON_H

#include "thememanager.h"

#include <QWidget>
#include <QPushButton>
#include <QString>
#include <QFileInfo>
#include <QDebug>
#include <QFont>
#include <QColor>
#include <vector>
#include <QStyle>
#include <QPainter>
#include <QPainterPath>
#include <QEvent>
#include <QPropertyAnimation>

using namespace std;
using TM = ThemeManager;

class ThemeButton : public QPushButton
{
	Q_OBJECT
    Q_PROPERTY(float animationProgress READ getAnimationProgress WRITE setAnimationProgress)
public:
	explicit ThemeButton(const QString &filePath, const QColor &primary, const QColor &secondary, QWidget *parent = nullptr);

	void updateDisplay();
    float getAnimationProgress() { return _animationProgress; }
    void setAnimationProgress(float value) { _animationProgress = value; update(); }

protected:
	void resizeEvent(QResizeEvent *event) override;
	void paintEvent(QPaintEvent *event) override;
	void enterEvent(QEnterEvent *event) override;
	void leaveEvent(QEvent *event) override;

signals:
	void updateThemeDisplay();

public slots:
	void handleClicked();

private:
	QString _themeName;
	QColor _primary;
	QColor _secondary;
	int cornerRadius = 15;
	bool _hovered = false;
    float _animationProgress = 0.0f;

    void animationClicked(bool backward = false);
};

#endif // THEMEBUTTON_H
