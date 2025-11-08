#include "levelselector.h"
#include "ui_levelselector.h"

LevelSelector::LevelSelector(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LevelSelector)
{
    ui->setupUi(this);

	levelWidget = ui->levelWidget;
	ui->overlayWidget->raise();

    hide();
    setMouseTracking(true);

	connect(ui->closeOverlay, &QPushButton::clicked, this, [this]() { animationClicked(true); });
	connect(ui->noiseButton, &QPushButton::clicked, this, [this]() { animationClicked(true); });

	effect = new QGraphicsOpacityEffect(ui->noiseWidget);

    addLevels();
	updateDisplay();
	updateButtonSize();
}

LevelSelector::~LevelSelector()
{
	delete ui;
}

void LevelSelector::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);
	updateLayout();
}

void LevelSelector::updateLayout()
{
	updateButtonSize();

	ui->noiseWidget->setGeometry(rect());

	effect->setOpacity(animationFactor);
	ui->noiseWidget->setGraphicsEffect(effect);

	int overlayWidth = ui->overlayWidget->width();
	int offset = 40;
	ui->overlayWidget->setGeometry(offset * animationFactor - overlayWidth * (1.0 - animationFactor), offset, overlayWidth, height() - 2 * offset);
}

void LevelSelector::addLevels()
{
    QDir dir(":/games");

    if (!dir.exists())
    {
        qDebug() << "LS-> no QRC file found";
        return;
    }

	QVBoxLayout *layout = new QVBoxLayout(levelWidget);
	levelWidget->setLayout(layout);
	layout->setAlignment(Qt::AlignTop);

	const QStringList files = dir.entryList(QDir::Files);
	for (const QString &file : files)
	{
		if (file == "000.txt") continue;
        LevelButton *levelButton = new LevelButton(file, this);
		layout->addWidget(levelButton);

		connect(levelButton, &LevelButton::clicked, this, [this, levelButton]() { animationClicked(true); emit sendGameFile(levelButton->getFilePath()); } );
    }
}

void LevelSelector::updateDisplay()
{
	for (QObject *child : levelWidget->children())
	{
		LevelButton *button = qobject_cast<LevelButton*>(child);
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

void LevelSelector::updateButtonSize()
{
	for (QObject *child : levelWidget->children())
	{
		LevelButton *button = qobject_cast<LevelButton*>(child);
		if (button)
		{
			button->setFixedSize(130, 130);
		}
	}
}

void LevelSelector::animationClicked(bool backward)
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

void LevelSelector::handleShowOverlay()
{
	animationClicked();
	show();
}
