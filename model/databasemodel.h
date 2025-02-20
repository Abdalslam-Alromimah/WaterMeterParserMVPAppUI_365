/**
 * @file database_model.h
 * @brief Handles database operations related to user authentication.
 *
 * The `DatabaseModel` class interacts with a database, typically an SQLite database, to perform operations such as
 * validating user credentials during login. It provides methods to open the database, validate credentials, and
 * manage user-related data.
 */

#ifndef DATABASE_MODEL_H
#define DATABASE_MODEL_H

#include "../interfaces/basemodel.h"
#include <QSqlDatabase>
#include <QDebug>

/**
 * @class DatabaseModel
 * @brief Interacts with the database to perform operations related to user authentication.
 *
 * The `DatabaseModel` class provides functionality to interact with a database for validating user credentials
 * and managing user-related data. This class extends `BaseModel` and encapsulates the logic for interacting with
 * the database, including opening the connection, validating credentials, and managing the database schema.
 */
class DatabaseModel : public BaseModel {
    Q_OBJECT

public:
    /**
     * @brief Constructs a DatabaseModel object and opens the database connection.
     *
     * This constructor initializes the database model and attempts to open the database at the given path.
     * If the connection is successful, the model can perform further database operations.
     *
     * @param path The path to the SQLite database file.
     * @param parent The parent QObject, default is nullptr.
     */
    explicit DatabaseModel(const QString& path, QObject* parent = nullptr);

    /**
     * @brief Destructor for the DatabaseModel class.
     *
     * The destructor ensures the database connection is properly closed when the model is destroyed.
     */
    ~DatabaseModel();

    /**
     * @brief Checks if the database connection is open.
     *
     * This method checks whether the database connection is successfully established and open.
     *
     * @return `true` if the database is open, `false` otherwise.
     */
    bool isOpen() const;

    /**
     * @brief Validates the user's credentials by checking against the database.
     *
     * This method connects to the SQLite database, retrieves the stored password for the provided username, and compares it
     * with the password entered by the user. If the credentials match, it returns `true`, indicating that the login is valid.
     * Otherwise, it displays an error message and returns `false`.
     *
     * @param path The path to the SQLite database file.
     * @param username The username entered by the user.
     * @param password The password entered by the user.
     * @return `true` if the credentials are valid, `false` otherwise.
     */
    bool validateCredentials(const QString &path, const QString& username, const QString& password);

private:
    /** The SQLite database object used to manage the database connection. */
    QSqlDatabase m_db;

    /**
     * @brief Creates the user login table in the database.
     *
     * This method is responsible for creating the user login table if it does not already exist in the database.
     * It defines the structure of the table, including fields like `username` and `password`.
     */
    void createUserLoginTable();

    /**
     * @brief Inserts a default user into the database.
     *
     * This method inserts a default user record into the user login table. It is typically used to initialize the database
     * with a default user during the first run or setup of the application.
     *
     * @return `true` if the default user was successfully inserted, `false` otherwise.
     */
    bool insertDefaultUser();
};

#endif // DATABASE_MODEL_H
