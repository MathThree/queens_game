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

using namespace std;

struct Theme {
	QString name;
	QMap<QString, QVariant> values;
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
	const Theme& getTheme() { return currentTheme; }
	QString getStyle(const QString& qssName);
	const QString& getName() { return currentTheme.name; }
	vector<tuple<QString, QColor, QColor>> getAvailableThemes();

private:
	ThemeManager() {
	}

	Theme currentTheme;
	QMap<QString, QString> styles;
	QStringList qssNames = { "cell", "central", "overlay", "centralButtons", "overlayButtons" };
};


#endif // THEMEMANAGER_H
