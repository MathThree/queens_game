#include "levelbutton.h"

LevelButton::LevelButton(const QString &filePath, QWidget *parent) : QPushButton(parent), _filePath(filePath)
{
    QString name = QFileInfo(filePath).completeBaseName();
    setText(name);

	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

	updateDisplay();
}

void LevelButton::updateDisplay()
{
	QString qss = _themeM->getStyle("overlayButtons");

	qss.replace("%CORNER_RADIUS%", QString::number(cornerRadius));

	this->setStyleSheet(qss);
}

void LevelButton::resizeEvent(QResizeEvent *event)
{
	QFont font = this->font();
	font.setPointSize(height()/3);
	setFont(font);
}