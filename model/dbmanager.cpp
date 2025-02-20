#include "dbmanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DbManager::DbManager(const QString& path) {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);
    qDebug() << "database Path"<<path;

    if (!m_db.open()) {
        qDebug() << "Error: connection with database failed";
    }
}

DbManager::~DbManager() {
    if (m_db.isOpen()) {
        m_db.close();
    }
}

bool DbManager::isOpen() const {
    return m_db.isOpen();
}

bool DbManager::createTable() {
    QSqlQuery query;
    return query.exec("CREATE TABLE UserLogin(username TEXT PRIMARY KEY, password TEXT);");
}

bool DbManager::addUser(const QString& username, const QString& password) {
    QSqlQuery query;
    query.prepare("INSERT INTO UserLogin(username, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    return query.exec();
}

bool DbManager::removeUser(const QString& name) {
    QSqlQuery query;
    query.prepare("DELETE FROM UserLogin WHERE username = :username");
    query.bindValue(":username", name);
    return query.exec();
}

bool DbManager::userExists(const QString& name) const {
    QSqlQuery query;
    query.prepare("SELECT username FROM UserLogin WHERE username = :username");
    query.bindValue(":username", name);
    return query.exec() && query.next();
}

void DbManager::printAllUsers() const {
    QSqlQuery query("SELECT username FROM UserLogin");
    while (query.next()) {
        qDebug() << query.value(0).toString();
    }
}

bool DbManager::removeAllUsers() {
    QSqlQuery query;
    return query.exec("DELETE FROM UserLogin");
}
