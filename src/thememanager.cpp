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
		currentTheme.colors[it.key()] = QColor(arr[0].toInt(), arr[1].toInt(), arr[2].toInt());
	}

    QJsonObject bools = obj["bools"].toObject();
    for (auto it = bools.begin(); it != bools.end(); ++it) {
        if (!it.value().isBool()) continue;
		currentTheme.bools[it.key()] = it.value().toBool();
	}

	QJsonObject floats = obj["floats"].toObject();
	for (auto it = floats.begin(); it != floats.end(); ++it) {
		if (!it.value().isDouble()) continue;
		currentTheme.floats[it.key()] = static_cast<float>(it.value().toDouble());
	}

	QJsonObject strings = obj["strings"].toObject();
	for (auto it = strings.begin(); it != strings.end(); ++it) {
		if (!it.value().isString()) continue;
		currentTheme.strings[it.key()] = it.value().toString();
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

        qss.replace("%PRIMARY%", getColor("primary").name());
        qss.replace("%SECONDARY%", getColor("secondary").name());
        qss.replace("%P_HOVER%", getColor("p_hover").name());
        qss.replace("%S_HOVER%", getColor("s_hover").name());
        qss.replace("%OVERLAY_RADIUS%", QString::number(30 * getFloat("cornerFactor")));

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

const bool ThemeManager::getBool(QString key)
{
	if (currentTheme.bools.contains(key))
		return currentTheme.bools[key];
	qWarning() << "TM-> Bool key not found: " << key;
	return false;
}

const float ThemeManager::getFloat(QString key)
{
	if (currentTheme.floats.contains(key))
		return currentTheme.floats[key];
	qWarning() << "TM-> Float key not found: " << key;
	return 0.0;
}

const QColor ThemeManager::getColor(QString key)
{
	if (currentTheme.colors.contains(key))
		return currentTheme.colors[key];
	qWarning() << "TM-> Color key not found: " << key;
	return Qt::cyan;
}

const QString ThemeManager::getString(QString key)
{
	if (currentTheme.strings.contains(key))
		return currentTheme.strings[key];
	qWarning() << "TM-> Source key not found: " << key;
	return QString("");
}

const QColor ThemeManager::getCellColor(const int colorZone, QString key)
{
	if (colorZone < zoneColors.size() && zoneColors[colorZone].colors.contains(key))
		return zoneColors[colorZone].colors[key];
	qWarning() << "TM-> Cell color key not found: " << key;
	return Qt::yellow;
}

void ThemeManager::updateZoneColors(int n, bool random)
{
	if (random)
		randomizeBaseColors(n);

	for (int i=0; i<zoneColors.size(); ++i)
		fillCellColors(i);

	qDebug() << "TM-> Zone colors updated: " << n;
}

void ThemeManager::randomizeBaseColors(int n)
{
	if (n == 0)
		n = zoneColors.size()-1;
	static mt19937 rng(random_device{}());
	vector<int> indexes(n);
	iota(indexes.begin(), indexes.end(), 0);
	shuffle(indexes.begin(), indexes.end(), rng);
	uniform_real_distribution<double> dist(0.0, 1.0);

	zoneColors.resize(n+1);
	double h = -dist(rng) / n;
	double sC = .62;
	double vC = .78;
	double sB = .72;
	double vB = .46;

	double offset = .5 / n;
	for (int i=0; i<n; ++i)
	{
		h += 2. * offset;
		zoneColors[indexes[i]].colors["base"] = QColor::fromHsvF(h, sC, vC);
	}
	zoneColors[n].colors["base"] = QColor(255, 255, 255);
}

void ThemeManager::fillCellColors(const int colorZone)
{
	QColor baseColor = zoneColors[colorZone].colors["base"];

	QStringList keys = {
		"text",
		"borderIn",
		"borderOut",
		"borderInDisabled",
		"borderOutDisabled",
		"cornerOut",
		"cornerOutDisabled"
	};

	for (const QString &key : keys)
		addCellColor(colorZone, key);
}

void ThemeManager::addCellColor(const int colorZone, QString key)
{
	QString colorName = getString(key);
	if (colorName == "cell")
		zoneColors[colorZone].colors[key] = zoneColors[colorZone].colors["base"];
	else if (colorName == "cell_dark")
		zoneColors[colorZone].colors[key] = zoneColors[colorZone].colors["base"].darker((int) getFloat(key));
	else
		zoneColors[colorZone].colors[key] = getColor(colorName);
}

void ThemeManager::swapThemeColors()
{
	QColor tmpPrimary   = getColor("primary");
	QColor tmpSecondary = getColor("secondary");
	QColor tmpPHover    = getColor("p_hover");
	QColor tmpSHover    = getColor("s_hover");

	currentTheme.colors["primary"]   = tmpSecondary;
	currentTheme.colors["secondary"] = tmpPrimary;
	currentTheme.colors["p_hover"]   = tmpSHover;
	currentTheme.colors["s_hover"]   = tmpPHover;

    setStyle(currentTheme.name);

    qDebug() << "TM-> Primary and secondary colors swapped for theme: " << currentTheme.name;
}
