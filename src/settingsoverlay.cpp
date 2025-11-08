#include "settingsoverlay.h"
#include "ui_settingsoverlay.h"

SettingsOverlay::SettingsOverlay(QWidget *parent)
    : QWidget(parent)
	, ui(new Ui::SettingsOverlay)
{
    ui->setupUi(this);

	colorsWidget = ui->colorsWidget;
	ui->overlayWidget->raise();

	hide();
	setMouseTracking(true);

	connect(ui->closeOverlay, &QPushButton::clicked, this, [this]() { animationClicked(true); });
	connect(ui->noiseButton, &QPushButton::clicked, this, [this]() { animationClicked(true); });
	connect(ui->helpDisplay, &QRadioButton::clicked, this, &SettingsOverlay::askHelp);

	effect = new QGraphicsOpacityEffect(ui->noiseWidget);

	addColors();
	setColorTheme({QColor(76, 76, 136)});
	updateDisplay();
	updateButtonSize();
}

SettingsOverlay::~SettingsOverlay()
{
	delete ui;
}

void SettingsOverlay::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);
	updateLayout();
}

void SettingsOverlay::updateLayout()
{
	updateButtonSize();

	ui->noiseWidget->setGeometry(rect());

	effect->setOpacity(animationFactor);
	ui->noiseWidget->setGraphicsEffect(effect);

	int overlayWidth = ui->overlayWidget->width();
	int offset = 40;
	ui->overlayWidget->setGeometry(width() - (overlayWidth + offset) * animationFactor, offset, overlayWidth, height() - 2 * offset);
}

void SettingsOverlay::addColors()
{
	vector<tuple<QString, QColor, QColor>> colorsList = _themeM->getAvailableThemes();

	QVBoxLayout *layout = new QVBoxLayout(colorsWidget);
	colorsWidget->setLayout(layout);
	layout->setAlignment(Qt::AlignTop);


	for (const auto &[themeName, primary, secondary] : colorsList)
	{
		qDebug() << "SO-> Colors for " << themeName << ": " << primary.name() << "; " << secondary.name();
		ThemeButton *themeButton = new ThemeButton(themeName, primary, secondary, this);
		layout->addWidget(themeButton);

		connect(themeButton, &ThemeButton::updateThemeDisplay, this, &SettingsOverlay::updateThemeDisplay);
	}
}

void SettingsOverlay::updateDisplay()
{
	for (QObject *child : colorsWidget->children())
	{
		ThemeButton *button = qobject_cast<ThemeButton*>(child);
		if (button)
		{
			button->updateDisplay();
		}
	}

	QScrollArea *scrollArea = ui->scrollArea;
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	QString qss = _themeM->getStyle("overlay");

	ui->overlayWidget->setStyleSheet(qss);
	ui->overlayWidget->style()->unpolish(ui->overlayWidget);
	ui->overlayWidget->style()->polish(ui->overlayWidget);
	ui->overlayWidget->update();
}

void SettingsOverlay::updateButtonSize()
{
	for (QObject *child : colorsWidget->children())
	{
		ThemeButton *button = qobject_cast<ThemeButton*>(child);
		if (button)
		{
			button->setFixedSize(130, 130);
		}
	}
}

void SettingsOverlay::animationClicked(bool backward)
{
#ifdef Q_OS_WASM
	int duration = 72;
#else
	int duration = 360;
#endif
	QPropertyAnimation *animPos = new QPropertyAnimation(this, "animationFactor");
	animPos->setDuration(duration);
	animPos->setStartValue(backward ? 1.0 : 0.0);
	animPos->setEndValue(backward ? 0.0 : 1.0);
	animPos->setEasingCurve(QEasingCurve::OutCubic);

	if (backward)
		connect(animPos, &QPropertyAnimation::finished, this, [this]() { hide(); });

	animPos->start(QAbstractAnimation::DeleteWhenStopped);
}

void SettingsOverlay::handleShowOverlay()
{
	animationClicked();
	show();
}
