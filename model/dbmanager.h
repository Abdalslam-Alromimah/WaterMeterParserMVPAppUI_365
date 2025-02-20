/**
 * @file dbmanager.h
 * @brief Manages the database connection and user-related operations.
 *
 * The `DbManager` class handles database interactions such as creating tables, adding or removing users,
 * checking if a user exists, and printing all users. It provides an interface for managing user data in
 * the database.
 */

#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>

/**
 * @class DbManager
 * @brief Manages database operations such as creating tables, adding/removing users, and querying user data.
 *
 * The `DbManager` class encapsulates the logic for managing users in the database. It provides methods to
 * create tables, add or remove users, check for user existence, and retrieve or delete all user records.
 */
class DbManager {
public:
    /**
     * @brief Constructs a DbManager object and opens the database connection.
     *
     * This constructor initializes the `DbManager` object by opening a database connection using the provided path.
     *
     * @param path The path to the SQLite database file.
     */
    DbManager(const QString& path);

    /**
     * @brief Destructor for the DbManager class.
     *
     * The destructor ensures the database connection is properly closed when the object is destroyed.
     */
    ~DbManager();

    /**
     * @brief Checks if the database connection is open.
     *
     * This method checks whether the database connection is successfully established and open.
     *
     * @return `true` if the database is open, `false` otherwise.
     */
    bool isOpen() const;

    /**
     * @brief Creates a table for storing user information.
     *
     * This method creates the user table in the database. If the table already exists, this method does nothing.
     *
     * @return `true` if the table was successfully created or already exists, `false` otherwise.
     */
    bool createTable();

    /**
     * @brief Adds a new user to the database.
     *
     * This method inserts a new user into the database with the provided username and password.
     *
     * @param username The username of the user to add.
     * @param password The password of the user to add.
     * @return `true` if the user was successfully added, `false` otherwise.
     */
    bool addUser(const QString& username, const QString& password);

    /**
     * @brief Removes a user from the database.
     *
     * This method removes a user from the database using the provided username.
     *
     * @param name The username of the user to remove.
     * @return `true` if the user was successfully removed, `false` otherwise.
     */
    bool removeUser(const QString& name);

    /**
     * @brief Checks if a user exists in the database.
     *
     * This method checks if the user with the provided username exists in the database.
     *
     * @param name The username to check for existence.
     * @return `true` if the user exists, `false` otherwise.
     */
    bool userExists(const QString& name) const;

    /**
     * @brief Prints all users stored in the database.
     *
     * This method retrieves and prints all the users from the database, displaying their usernames.
     */
    void printAllUsers() const;

    /**
     * @brief Removes all users from the database.
     *
     * This method deletes all user records from the database.
     *
     * @return `true` if all users were successfully removed, `false` otherwise.
     */
    bool removeAllUsers();

private:
    /** The SQLite database object used to manage the database connection. */
    QSqlDatabase m_db;
};

#endif // DBMANAGER_H
