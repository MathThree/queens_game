#include "levelbutton.h"

LevelButton::LevelButton(const QString &filePath, QWidget *parent) : QPushButton(parent), _filePath(filePath)
{
    QString name = QFileInfo(filePath).completeBaseName();
    setText(name);

    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);
}

