/**
 * @file filedata.h
 * @brief Manages the data related to a file, including metadata and parsed values.
 *
 * The `FileData` class is responsible for storing and managing various pieces of information related to a file.
 * This includes metadata such as file ID and description, as well as data about specific fields within the file,
 * including their byte positions and parsed values.
 */

#ifndef FILEDATA_H
#define FILEDATA_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QSharedPointer>
#include <QDebug>

/**
 * @class FileData
 * @brief Stores and manages metadata and parsed field data for a file.
 *
 * The `FileData` class contains various vectors that store metadata about a file (such as its ID and description),
 * as well as details about specific fields in the file (such as field names, byte positions, and parsed values).
 * This class helps in organizing and managing file-related data efficiently.
 */
class FileData : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructs a `FileData` object.
     *
     * Initializes the `FileData` object and its data members.
     *
     * @param parent The parent `QObject`, default is `nullptr`.
     */
    explicit FileData(QObject *parent = nullptr);

    /** A vector that stores the file IDs. */
    QVector<QString> FileID;

    /** A vector that stores the descriptions associated with the file. */
    QVector<QString> Description;

    /** A vector that stores the number of bytes for each field in the file. */
    QVector<int> FieldBytesNumber;

    /** A vector that stores the field names for the file. */
    QVector<QString> FieldName;

    /** A vector that stores the byte numbers for each field in the file. */
    QVector<int> FieldByteNumber;

    /** A vector that stores the starting byte index for each field in the file. */
    QVector<int> FieldByteIndexStart;

    /** A vector that stores the ending byte index for each field in the file. */
    QVector<int> FieldByteIndexEnd;

    /** A 2D vector that stores the parsed values for each field in the file. */
    QVector<QVector<QString>> FieldParsedValue;

private:
    /**
     * @brief Initializes the file metadata.
     *
     * This method sets up the metadata vectors (`FileID`, `Description`, etc.) that hold the basic information about the file.
     */
    void initializeFileMetadata();

    /**
     * @brief Initializes the fields within the file.
     *
     * This method sets up the vectors that represent the fields in the file, including their byte positions and names.
     */
    void initializeFields();
};

#endif // FILEDATA_H
