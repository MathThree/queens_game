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
	int side = TM::instance().getName() == _themeName ? qMin(w, h)*.7 : qMin(w, h);
	QRect rect((w - side)/2, (h - side)/2, side, side);

	int factor = 135;
	if (TM::instance().getName() != _themeName & _hovered) factor = 100;

	painter.setBrush(_primary.darker(factor));
	painter.drawEllipse(rect);

	QPainterPath path2;
	path2.moveTo(rect.center());
	path2.arcTo(rect, -45, 180);
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
	if (TM::instance().getName() == _themeName) return;
	TM::instance().applyTheme(_themeName);
	emit updateThemeDisplay();
}