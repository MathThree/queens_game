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
	QString style = QString(
		"QPushButton {"
		" background-color: %1;"
		" border: none;"
		" border-radius: %2px;"
		" color: %3;"
		"}"
		"QPushButton:hover {"
		" color: white;"
		"}"
	).arg(colorTheme[0].name())
	 .arg(cornerRadius)
	 .arg(QColor(170, 170, 170).name());

	setStyleSheet(style);
}

void LevelButton::resizeEvent(QResizeEvent *event)
{
	QFont font = this->font();
	font.setPointSize(height()/3);
	setFont(font);
}