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

	connect(ui->closeOverlay, &QPushButton::clicked, this, [this]() { hide(); });
	connect(ui->noiseButton, &QPushButton::clicked, this, [this]() { hide(); });
	connect(ui->helpDisplay, &QRadioButton::clicked, this, &SettingsOverlay::askHelp);

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
	updateButtonSize();

	ui->noiseWidget->setGeometry(rect());

	int overlayWidth = ui->overlayWidget->width();
	ui->overlayWidget->setGeometry(width() - overlayWidth, 0, overlayWidth, height());
}

void SettingsOverlay::addColors()
{
	vector<tuple<QString, QColor, QColor>> colorsList = TM::instance().getAvailableThemes();

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

	QString qss = TM::instance().getStyle("overlay");

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

void SettingsOverlay::handleShowOverlay()
{
	show();
	updateButtonSize();
}
