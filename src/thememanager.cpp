#include "../include/thememanager.h"

void ThemeManager::applyTheme(const QString &themeName)
{
	setTheme(themeName);
	setStyle(themeName);
}

void ThemeManager::setTheme(const QString &themeName)
{
	QString themeFile = QString(":/themes/%1/%1_theme.json").arg(themeName);

	QFile file(themeFile);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qWarning() << "TM-> File not found: " << themeFile;
		return;
	}

	currentTheme.name = themeName;

	QByteArray data = file.readAll();
	file.close();

	QJsonDocument doc = QJsonDocument::fromJson(data);
	if (!doc.isObject()) {
		qWarning() << "TM-> JSON format not valid: " << themeFile;
		return;
	}

	QJsonObject obj = doc.object();

	QJsonObject colors = obj["colors"].toObject();
	for (auto it=colors.begin(); it!=colors.end(); ++it)
	{
		QJsonArray arr = it.value().toArray();
		if (arr.size() != 3) continue;
		currentTheme.values[it.key()] = QColor(arr[0].toInt(), arr[1].toInt(), arr[2].toInt());
	}
}

void ThemeManager::setStyle(const QString &themeName)
{
	for (QString &qssName : qssNames) {
		QStringList candidates = {
			QString(":/themes/%1/%1_%2.qss").arg(themeName, qssName),
			QString(":/themes/default/default_%1.qss").arg(qssName)
		};

		QString qss;
		bool loaded = false;

		for (const QString &path : candidates)
		{
			QFile file(path);
			if (file.open(QIODevice::ReadOnly | QIODevice::Text))
			{
				qss = file.readAll();
				file.close();
				loaded = true;
				break;
			}
		}
		if (!loaded)
		{
			qWarning().noquote() << "TM-> QSS not found event in default: " << qssName << " (" << themeName << ")";
			continue;
		}

		qss.replace("%PRIMARY%", currentTheme.values["primary"].value<QColor>().name());
		qss.replace("%SECONDARY%", currentTheme.values["secondary"].value<QColor>().name());
		qss.replace("%P_HOVER%", currentTheme.values["p_hover"].value<QColor>().name());
		qss.replace("%S_HOVER%", currentTheme.values["s_hover"].value<QColor>().name());

		styles[qssName] = qss;
		qDebug() << "TM-> QSS updated: " << qssName;
	}
}

QString ThemeManager::getStyle(const QString &qssName)
{
	if (!styles.contains(qssName))
	{
		qDebug() << "TM-> Available styles:" << styles.keys();
		throw runtime_error(("TM-> Style not found: " + qssName).toStdString());
	}
	return styles[qssName];
}

vector<tuple<QString, QColor, QColor>> ThemeManager::getAvailableThemes()
{
	vector<tuple<QString, QColor, QColor>> list;

	QDir themesDir(":/themes/");
	QStringList themeFolders = themesDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

	if (themeFolders.contains("default")) {
		themeFolders.move(themeFolders.indexOf("default"), 0);
	}

	for (QString &themeName : themeFolders)
	{
		QString themeFile = QString(":/themes/%1/%1_theme.json").arg(themeName);
		QFile file(themeFile);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
			continue;

		QByteArray data = file.readAll();
		file.close();

		QJsonDocument doc = QJsonDocument::fromJson(data);
		if (!doc.isObject())
			continue;

		QJsonObject obj = doc.object();
		QJsonObject colors = obj["colors"].toObject();

		QColor primary, secondary;
		if (colors.contains("primary"))
		{
			QJsonArray arr = colors["primary"].toArray();
			if (arr.size() == 3)
			{
				primary = QColor(arr[0].toInt(), arr[1].toInt(), arr[2].toInt());
			}
		}
		if (colors.contains("secondary"))
		{
			QJsonArray arr = colors["secondary"].toArray();
			if (arr.size() == 3)
			{
				secondary = QColor(arr[0].toInt(), arr[1].toInt(), arr[2].toInt());
			}
		}
		list.emplace_back(themeName, primary, secondary);
	}
	return list;
}