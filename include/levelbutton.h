#ifndef LEVELBUTTON_H
#define LEVELBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QString>
#include <QFileInfo>
#include <QDebug>
#include <QFont>
#include <QColor>
#include <vector>

using namespace std;

class LevelButton : public QPushButton
{
	Q_OBJECT
public:
    explicit LevelButton(const QString &filePath, QWidget *parent = nullptr);

	QString getFilePath() { return _filePath; }
	void setColorTheme(vector<QColor> color) { colorTheme = color; }
	void updateDisplay();

protected:
	void resizeEvent(QResizeEvent *event) override;

private:
	QString _filePath;
	vector<QColor> colorTheme = {Qt::black, Qt::white};
	int cornerRadius = 15;
};

#endif // LEVELBUTTON_H
