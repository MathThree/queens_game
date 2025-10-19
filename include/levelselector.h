#ifndef LEVELSELECTOR_H
#define LEVELSELECTOR_H

#include "levelbutton.h"

#include <QWidget>
#include <QDebug>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QDir>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QColor>
#include <QScrollBar>
#include <vector>

using namespace std;

namespace Ui {
class LevelSelector;
}

class LevelSelector : public QWidget
{
	Q_OBJECT

public:
	explicit LevelSelector(QWidget *parent = nullptr);
	~LevelSelector();

	void setColorTheme(vector<QColor> color) { colorTheme = color; }
	void updateDisplay();

signals:
    void sendGameFile(QString fileName);

public slots:
	void handleShowOverlay();

private:
	Ui::LevelSelector *ui;
	QWidget *levelWidget;
	vector<QColor> colorTheme;

	void addLevels();
	void updateButtonSize();

protected:
	void resizeEvent(QResizeEvent *event) override;
};

#endif // LEVELSELECTOR_H
