#include "themebutton.h"

ThemeButton::ThemeButton(const QString &themeName, const QColor &primary, const QColor &secondary, QWidget *parent) : QPushButton(parent), _themeName(themeName), _primary(primary), _secondary(secondary)
{
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	setMouseTracking(true);

	connect(this, &ThemeButton::clicked, this, &ThemeButton::handleClicked);

	if (themeName == _themeM->getName())
	{
		_rotation = 0;
		_sideFactor = 0.7;
	}
}

void ThemeButton::updateDisplay()
{
	QString qss = _themeM->getStyle("overlayButtons");

	qss.replace("%CORNER_RADIUS%", QString::number(cornerRadius));

	this->setStyleSheet(qss);
}

void ThemeButton::resizeEvent(QResizeEvent *event)
{
	QFont font = this->font();
	font.setPointSize(height()/3);
	setFont(font);
}

void ThemeButton::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);

	int w = width();
	int h = height();
	int side = _themeM->getName() == _themeName ? qMin(w, h)*_sideFactor : qMin(w, h);
    QRect rect((w - side)/2, (h - side)/2, side, side);

	int factor = (_hovered) ? 100 : 135;

	painter.setBrush(_primary.darker(factor));
	painter.drawEllipse(rect);

	QPainterPath path2;
	path2.moveTo(rect.center());
	path2.arcTo(rect, -45 + _rotation, 180);
	path2.closeSubpath();
	painter.setBrush(_secondary.darker(factor));
	painter.drawPath(path2);
}

void ThemeButton::enterEvent(QEnterEvent *event)
{
	Q_UNUSED(event);
	_hovered = true;
	update();
}

void ThemeButton::leaveEvent(QEvent *event)
{
	Q_UNUSED(event);
	_hovered = false;
	update();
}

void ThemeButton::handleClicked()
{
	if (_themeM->getName() == _themeName)
	{
		_themeM->swapThemeColors();
		_themeM->updateZoneColors(0);
		animationClicked(true);
	}
    else
	{
		_themeM->applyTheme(_themeName);
		_themeM->updateZoneColors(0, false);
		animationClicked();
	}
	emit updateThemeDisplay();
}

void ThemeButton::animationClicked(bool rotationOnly, bool backward)
{
	int duration = 720;
	QPropertyAnimation *animRotation = new QPropertyAnimation(this, "rotation");
	animRotation->setDuration(duration);
	animRotation->setStartValue(backward ? 360.0 : 0.0);
	animRotation->setEndValue(backward ? 0.0 : 360.0);
	animRotation->setEasingCurve(QEasingCurve::OutCubic);
	animRotation->start(QAbstractAnimation::DeleteWhenStopped);

	if (!rotationOnly)
	{
		QPropertyAnimation *animSideFactor = new QPropertyAnimation(this, "sideFactor");
		animSideFactor->setDuration(duration);
		animSideFactor->setStartValue(backward ? 0.7 : 1.0);
		animSideFactor->setEndValue(backward ? 1.0 : 0.7);
		animSideFactor->setEasingCurve(QEasingCurve::OutCubic);
		animSideFactor->start(QAbstractAnimation::DeleteWhenStopped);
	}
}
