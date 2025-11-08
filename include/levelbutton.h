#ifndef LEVELBUTTON_H
#define LEVELBUTTON_H

#include "thememanager.h"

#include <QWidget>
#include <QPushButton>
#include <QString>
#include <QFileInfo>
#include <QDebug>
#include <QFont>
#include <QColor>
#include <vector>
#include <QStyle>

using namespace std;
using TM = ThemeManager;

class LevelButton : public QPushButton
{
	Q_OBJECT
public:
    explicit LevelButton(const QString &filePath, QWidget *parent = nullptr);

	QString getFilePath() { return _filePath; }
	void updateDisplay();

protected:
	void resizeEvent(QResizeEvent *event) override;

private:
	QString _filePath;
	int cornerRadius = 15;

	TM *_themeM = &TM::instance();
};

#endif // LEVELBUTTON_H
