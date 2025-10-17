#include "levelselector.h"
#include "ui_levelselector.h"

LevelSelector::LevelSelector(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LevelSelector)
{
    ui->setupUi(this);

    levelWidget = ui->levelWidget;

    qDebug() << "LS-> Created";
    hide();
    setMouseTracking(true);

    connect(ui->closeOverlay, &QPushButton::clicked, this, [this]() { hide(); });

    addLevels();
}

LevelSelector::~LevelSelector()
{
	delete ui;
}

void LevelSelector::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}

void LevelSelector::mouseMoveEvent(QMouseEvent *event)
{
    //qDebug() << "LS->HERE!";
}

void LevelSelector::addLevels()
{
    QDir dir(":/games");

    if (!dir.exists())
    {
        qDebug() << "LS-> no QRC file found";
        return;
    }

    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui->levelWidget->layout());

    QStringList files = dir.entryList(QDir::Files);
    for (const QString &file : files)
    {

        LevelButton *levelButton = new LevelButton(file, this);
        layout->addWidget(levelButton);

        connect(levelButton, &LevelButton::clicked, this, [this, levelButton]() { hide(); emit sendGameFile(levelButton->getFilePath()); } );
        qDebug() << "Ls-> Level:" << file;
    }
}
