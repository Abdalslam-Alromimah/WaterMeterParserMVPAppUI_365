#include "databasemodel.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include<QMessageBox>
#include "../model/dbmanager.h"
#include "config.h"

DatabaseModel::DatabaseModel(const QString& path,QObject* parent)
    : BaseModel(parent) {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);
    qDebug() << "DatabaseModel path"<<path;

    if (!m_db.open()) {
           qDebug() << "Error: connection with database failed";
       } else {
        qDebug() << "Create table or login";

           createUserLoginTable();
       }
}

DatabaseModel::~DatabaseModel() {
    if (m_db.isOpen()) {
        m_db.close();
    }
}

bool DatabaseModel::isOpen() const {
    return m_db.isOpen();
}

/**
 * @brief Creates the UserLogin table if it does not already exist.
 *
 * This method checks if the UserLogin table exists in the database, and if not,
 * it creates the table.
 */
void DatabaseModel::createUserLoginTable() {
    QSqlQuery query;
    // SQL to check if the table exists
    query.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='UserLogin'");

    if (!query.next()) {
        // If the table does not exist, create it
        qDebug() << "UserLogin table does not exist, creating it.";
        query.prepare("CREATE TABLE UserLogin ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "username TEXT NOT NULL, "
                      "password TEXT NOT NULL"
                      ")");
        if (!query.exec()) {
            qDebug() << "Error: Unable to create UserLogin table: " << query.lastError().text();
        } else {
            qDebug() << "UserLogin table created successfully.";

           insertDefaultUser();
        }
    } else {
        qDebug() << "database.db table already exists.";
    }
}

bool DatabaseModel::insertDefaultUser() {
    QSqlQuery query;
    query.prepare("INSERT INTO UserLogin (username, password) VALUES (:username, :password)");
    query.bindValue(":username", "Ahmed");
    query.bindValue(":password", "123");

    if (!query.exec()) {
        qDebug() << "Error: Unable to insert user: " << query.lastError().text();
        return false;
    } else {
        qDebug() << "Default user inserted successfully.";
        return true;
    }
}

/**
 * @brief Validates the user's credentials by checking against the database.
 *
 * This method connects to the SQLite database, retrieves the stored password for the provided username, and compares it
 * with the password entered by the user. If the credentials match, it returns `true`, indicating that the login is valid.
 * Otherwise, it displays an error message and returns `false`.
 *
 * @param username The username entered by the user.
 * @param password The password entered by the user.
 * @return `true` if the credentials are valid, `false` otherwise.
 */
bool DatabaseModel::validateCredentials(const QString &path,const QString &username, const QString &password) {

    DbManager db(path);

    if (!db.isOpen()) {
        QMessageBox::critical(nullptr, "Error", "Database is not open!");
        return false;
    }
    qDebug() << "Query Running.";

    QSqlQuery query;
    query.prepare("SELECT password FROM UserLogin WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Error", "Database query failed: " + query.lastError().text());
        return false;
    }

    if (query.next()) {
        QString storedPassword = query.value(0).toString();
        if (storedPassword == password) {
            return true;
        }
    }

    QMessageBox::critical(nullptr, "Error", "Invalid username or password.");
    return false;
}


