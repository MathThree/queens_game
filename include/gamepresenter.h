#ifndef GAMEPRESENTER_H
#define GAMEPRESENTER_H

#include <QObject>
#include <QFile>
#include <QByteArray>
#include <QTextStream>
#include <QStringList>
#include "gamemodel.h"
#include "mainwindow.h"

class GamePresenter : public QObject
{
	Q_OBJECT
public:
	GamePresenter(QObject *parent = nullptr, GameModel *model = nullptr, MainWindow *view = nullptr);

	void loadGameFile(QString gameName);
	void initCells();

public slots:
	void handleCellClicked(const int row, const int col);

private:
	void toggleCellValue(const int row, const int col);

	GameModel *_model;
	MainWindow *_view;

signals:

};

#endif // GAMEPRESENTER_H
