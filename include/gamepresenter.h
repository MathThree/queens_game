#ifndef GAMEPRESENTER_H
#define GAMEPRESENTER_H

#include "gamemodel.h"
#include "mainwindow.h"

#include <QObject>
#include <QFile>
#include <QByteArray>
#include <QTextStream>
#include <QStringList>
#include <QString>

class GamePresenter : public QObject
{
	Q_OBJECT
public:
	GamePresenter(GameModel *model = nullptr, MainWindow *view = nullptr, QObject *parent = nullptr);

	void initCells();

public slots:
	void handleCellClicked(const int row, const int col);
	void handleCellUpdated(const int row, const int col, const int playerValue, const int bonusValue);
	void handleModelDebug(const QString debugText, const bool keep);

private:
	GameModel *_model;
	MainWindow *_view;

signals:

};

#endif // GAMEPRESENTER_H
