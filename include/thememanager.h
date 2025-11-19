#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QColor>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <QMap>
#include <stdexcept>
#include <QVariant>
#include <vector>
#include <utility>
#include <random>

using namespace std;

struct Theme {
	QString name;
	QMap<QString, QColor> colors;
	QMap<QString, bool> bools;
	QMap<QString, float> floats;
	QMap<QString, QString> strings;
};

struct CellColors {
	QMap<QString, QColor> colors;
};

class ThemeManager {
public:
	static ThemeManager& instance() {
		static ThemeManager instance;
		return instance;
	}

	void applyTheme(const QString &themeName = "default");
	void setTheme(const QString &themeName = "default");
    void setStyle(const QString &themeName = "default");
	QString getStyle(const QString& qssName);
	const QString& getName() { return currentTheme.name; }
    vector<tuple<QString, QColor, QColor>> getAvailableThemes();
    const bool getBool(QString key);
	const float getFloat(QString key);
	const QColor getColor(QString key);
	const QString getString(QString key);
	const QColor getCellColor(const int colorZone, QString key);
	void updateZoneColors(int n, bool random = true);
    void swapThemeColors();

private:
	ThemeManager() {
	}

	Theme currentTheme;
	QMap<QString, QString> styles;
	QStringList qssNames = { "cell", "central", "overlay", "centralButtons", "overlayButtons" };
	vector<CellColors> zoneColors;

	void fillCellColors(const int colorZone);
	void randomizeBaseColors(int n);
	void addCellColor(const int colorZone, QString key);
};


#endif // THEMEMANAGER_H
