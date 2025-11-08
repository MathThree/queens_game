#ifndef GAMEPRESENTER_H
#define GAMEPRESENTER_H

#include "gamemodel.h"
#include "mainwindow.h"

#include <QObject>
#include <QString>

/**
 * @brief The GamePresenter class connects the View and Model, so they can work without knowing each other (MVP architecture).
 */
class GamePresenter : public QObject
{
	Q_OBJECT
public:
	/**
	 * @brief Constructor
	 * @param model Pointer to the GameModel (default nullptr)
	 * @param view Pointer to the MainWindow (default nullptr)
	 * @param parent QObject parent (default nullptr)
	 */
    GamePresenter(GameModel *model = nullptr, MainWindow *view = nullptr, QObject *parent = nullptr);

	/**
	 * @brief Initialize all cells in the View
	 */
	void initCells();

public slots:
	/**
	 * @brief Handle the request to choose a game
	 */
	void handleChooseGame();

	/**
	 * @brief Handle the request to load a selected game file
	 * @param filePath Path to the selected game file (QString)
	 */
	void handleGetGameFile(const QString filePath);

private:
	GameModel *_model;  ///< Pointer to the game logic (Model)
	MainWindow *_view;  ///< Pointer to the UI (View)
};

#endif // GAMEPRESENTER_H
