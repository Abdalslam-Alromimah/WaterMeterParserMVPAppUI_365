#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QSettings>
#include <QDir>
#include <QApplication>
#include <QStandardPaths>
#include <QDebug>

class Config {
public:
    static Config& getInstance() {
    static Config instance;
    return instance;
    }

    // Getters
    QString getDatabasePath() const {
        return m_settings.value("Paths/Database", QCoreApplication::applicationDirPath() +  "/resources/db/database.db").toString();
    }

    QString getSplashImagePath() const {
        return m_settings.value("Paths/SplashImage", QCoreApplication::applicationDirPath() + "/resources/images/splash.jpeg").toString();
    }

    QString getDocumentationPath() const {
        return m_settings.value("Paths/Documentation", QCoreApplication::applicationDirPath() + "/resources/doc/html/index.html").toString();
    }

    QString getDarkStylesheetPath() const {
        return m_settings.value("Paths/DarkStylesheet", QCoreApplication::applicationDirPath() + "/resources/stylesheets/modern-qt-stylesheet1.qss").toString();
    }

    QString getLightStylesheetPath() const {
        return m_settings.value("Paths/LightStylesheet", QCoreApplication::applicationDirPath() + "/resources/stylesheets/modern-qt-stylesheet.qss").toString();
    }

    QString getDataDir() const { return m_dataDir; }
    QString getConfigDir() const { return m_configDir; }
    QString getCacheDir() const { return m_cacheDir; }

    // Setters
    void setDatabasePath(const QString& path) {
        m_settings.setValue("Paths/Database", path);
    }

    void setSplashImagePath(const QString& path) {
        m_settings.setValue("Paths/SplashImage", path); // Store the path in QSettings
    }

    void setDocumentationPath(const QString& path) {
        m_settings.setValue("Paths/Documentation", path); // Store the path in QSettings
    }

    void setDarkStylesheetPath(const QString& path) {
        m_settings.setValue("Paths/DarkStylesheet", path); // Store the path in QSettings
    }

    void setLightStylesheetPath(const QString& path) {
        m_settings.setValue("Paths/LightStylesheet", path); // Store the path in QSettings
    }

private:
    Config() : m_settings(getSettingsPath(), QSettings::IniFormat) {
        initializePaths();
    }

    void initializePaths() {
        m_dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        m_configDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
        m_cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);

        // Create directories if they don't exist
        QDir().mkpath(m_dataDir);
        QDir().mkpath(m_configDir);
        QDir().mkpath(m_cacheDir);

        // Ensure default database path is set in the settings if not already done

        if (m_settings.contains("Paths/Database")) {
            m_settings.setValue("Paths/Database", QDir::currentPath()+ "/resources/db/database.db");
        } else {
            qDebug() << "No path found, setting" ;
        }
    }

    QString getSettingsPath() const {
        QString configPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
        QDir().mkpath(configPath);
        return configPath + "/settings.ini";
    }

    QSettings m_settings;
    QString m_dataDir;
    QString m_configDir;
    QString m_cacheDir;
    QString m_databasePath;
    QString m_splashImagePath;
    QString m_documentationPath;
    QString m_darkStylesheetPath;
    QString m_lightStylesheetPath;
};

extern Config& config1;

#endif // CONFIG_H
