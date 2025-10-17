#ifndef LEVELBUTTON_H
#define LEVELBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QString>
#include <QFileInfo>

class LevelButton : public QPushButton
{
	Q_OBJECT
public:
    explicit LevelButton(const QString &filePath, QWidget *parent = nullptr);

    QString getFilePath() { qDebug() << "LS-> return: " << _filePath; return _filePath; }

private:
    QString _filePath;
};

#endif // LEVELBUTTON_H
