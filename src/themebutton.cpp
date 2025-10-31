#include "themebutton.h"

ThemeButton::ThemeButton(const QString &themeName, const QColor &primary, const QColor &secondary, QWidget *parent) : QPushButton(parent), _themeName(themeName), _primary(primary), _secondary(secondary)
{
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	setMouseTracking(true);

	connect(this, &ThemeButton::clicked, this, &ThemeButton::handleClicked);

	//updateDisplay();
}

void ThemeButton::updateDisplay()
{
	QString qss = TM::instance().getStyle("overlayButtons");

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
    int side = TM::instance().getName() == _themeName ? qMin(w, h)*(1.0-0.3*_animationProgress) : qMin(w, h);
    QRect rect((w - side)/2, (h - side)/2, side, side);

    int factor = (/*TM::instance().getName() != _themeName & */_hovered) ? 100 : 135;

	painter.setBrush(_primary.darker(factor));
	painter.drawEllipse(rect);

	QPainterPath path2;
	path2.moveTo(rect.center());
    path2.arcTo(rect, -45 + _animationProgress * 360, 180);
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
    animationClicked();
    if (TM::instance().getName() == _themeName)// return;
        TM::instance().swapThemeColors();
    else
        TM::instance().applyTheme(_themeName);
	emit updateThemeDisplay();
}

void ThemeButton::animationClicked(bool backward)
{
    QPropertyAnimation *anim = new QPropertyAnimation(this, "animationProgress");
    anim->setDuration(720);
    anim->setStartValue(backward ? 1.0 : 0.0);
    anim->setEndValue(backward ? 0.0 : 1.0);
    anim->setEasingCurve(QEasingCurve::OutCubic);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}
