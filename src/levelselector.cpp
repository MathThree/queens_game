#include "levelselector.h"
#include "ui_levelselector.h"

LevelSelector::LevelSelector(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LevelSelector)
{
    ui->setupUi(this);

	levelWidget = ui->levelWidget;

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
			button->setColorTheme(colorTheme);
			button->updateDisplay();
		}
	}

	QScrollArea *scrollArea = ui->scrollArea;
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	QScrollBar *vScroll = scrollArea->verticalScrollBar();
	if (vScroll)
	{
		QColor darker = colorTheme[0].darker(130);
		QString style = QString(
					"QScrollBar:vertical { background: rgba(0,0,0,50); width:10px; border-radius:5px; }"
					"QScrollBar::handle:vertical { background:%1; border-radius:5px; min-height:20px; }"
					"QScrollBar::handle:vertical:hover { background:%2; }"
					"QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height:0px; }"
					).arg(colorTheme[0].name()).arg(darker.name());

		vScroll->setStyleSheet(style);
	}
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