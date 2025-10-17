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

namespace Ui {
class LevelSelector;
}

class LevelSelector : public QWidget
{
	Q_OBJECT

public:
	explicit LevelSelector(QWidget *parent = nullptr);
	~LevelSelector();

signals:
    void sendGameFile(QString fileName);

private:
	Ui::LevelSelector *ui;
    QWidget *levelWidget;

    void addLevels();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // LEVELSELECTOR_H
