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

	connect(ui->closeOverlay, &QPushButton::clicked, this, [this]() { hide(); });
	connect(ui->noiseButton, &QPushButton::clicked, this, [this]() { hide(); });

	addLevels();
	setColorTheme({QColor(76, 76, 136)});
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
	updateButtonSize();

	ui->noiseWidget->setGeometry(rect());

	int overlayWidth = ui->overlayWidget->width();
	ui->overlayWidget->setGeometry(0, 0, overlayWidth, height());
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

    QStringList files = dir.entryList(QDir::Files);
    for (const QString &file : files)
    {
        LevelButton *levelButton = new LevelButton(file, this);
		layout->addWidget(levelButton);

		connect(levelButton, &LevelButton::clicked, this, [this, levelButton]() { hide(); emit sendGameFile(levelButton->getFilePath()); } );
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

	QString qss = TM::instance().getStyle("overlay");

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

void LevelSelector::handleShowOverlay()
{
	show();
	updateButtonSize();
}