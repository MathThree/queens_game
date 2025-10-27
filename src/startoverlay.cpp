#include "startoverlay.h"
#include "ui_startoverlay.h"

StartOverlay::StartOverlay(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StartOverlay)
{
    ui->setupUi(this);

	ui->overlayWidget->raise();

	setMouseTracking(true);

	connect(ui->closeOverlay, &QPushButton::clicked, this, [this]() { hide(); });
    connect(ui->noiseButton, &QPushButton::clicked, this, [this]() { hide(); });

    updateDisplay();
}

StartOverlay::~StartOverlay()
{
	delete ui;
}

void StartOverlay::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    ui->noiseWidget->setGeometry(rect());

    int size = 0.85 * min(event->size().width(), event->size().height());
    int x = (event->size().width() - size) / 2;
    int y = (event->size().height() - size) / 2;

    ui->overlayWidget->setGeometry(x, y, size, size);

    QFont font = ui->gameRules->font();
    font.setPixelSize(size / 55.0);
    ui->gameRules->setFont(font);
}

void StartOverlay::updateDisplay()
{
	QString qss = TM::instance().getStyle("overlay");

	ui->overlayWidget->setStyleSheet(qss);
	ui->overlayWidget->style()->unpolish(ui->overlayWidget);
	ui->overlayWidget->style()->polish(ui->overlayWidget);
	ui->overlayWidget->update();
}

void StartOverlay::handleShowOverlay()
{
    show();
}
