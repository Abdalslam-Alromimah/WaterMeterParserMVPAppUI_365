/**
 * @file WaterMeterParser_Model.h
 * @brief Defines the WaterMeterParserModel class, which handles the parsing of file data.
 *
 * The `WaterMeterParserModel` class provides the functionality to parse hexadecimal data for different file types
 * and processes them accordingly. The class exposes methods for parsing specific file types and storing the resulting
 * data in a `FileData` object.
 */

#ifndef FILEPARSERMODEL_H
#define FILEPARSERMODEL_H

#include <QObject>
#include <QString>
#include <QSharedPointer>
#include "filedata.h"
#include "../interfaces/basemodel.h"

/**
 * @class WaterMeterParserModel
 * @brief A model class responsible for parsing various file types and storing the parsed data.
 *
 * The `WaterMeterParserModel` class processes hexadecimal data for different types of files, ranging from
 * client cards to retrieval cards. Each file type is parsed with a dedicated method, and the parsed data is
 * stored in a `FileData` object.
 */
class WaterMeterParserModel : public BaseModel {
    Q_OBJECT
public:
    /**
     * @brief Constructs the WaterMeterParserModel.
     *
     * Initializes the `WaterMeterParserModel` object and sets up necessary resources.
     *
     * @param parent The parent object for this model, default is `nullptr`.
     */
    explicit WaterMeterParserModel(QObject* parent = nullptr);

    /**
     * @brief Parses a file based on the given file index and hexadecimal data.
     *
     * Depending on the `fileIndex`, the method calls the appropriate parser function for that file type
     * and returns the resulting parsed data as a `QSharedPointer<FileData>`.
     *
     * @param fileIndex The index representing the file type.
     * @param hexData The hexadecimal data to be parsed.
     * @return A `QSharedPointer<FileData>` containing the parsed data.
     */
    QSharedPointer<FileData> parseFile(int fileIndex, const QString& hexData);

    /**
     * @brief Returns the current file data.
     *
     * This method provides access to the last parsed `FileData`.
     *
     * @return A `QSharedPointer<FileData>` containing the current file data.
     */
    QSharedPointer<FileData> getFileData() const;

signals:
    /**
     * @brief Emitted when the file parsing is complete.
     *
     * This signal is emitted once the parsing is finished and provides the parsed data and the file index.
     *
     * @param result A `QSharedPointer<FileData>` containing the parsed data.
     * @param index The index of the file that was parsed.
     */
    void parsingComplete(QSharedPointer<FileData> result, int index);

private:
    /**
     * @brief Processes bytes for (Client Card - Inputs (From backend to meter) - Constant Main Information 1) File 001.
     *
     * This method processes the hexadecimal input data for file 001 and returns the parsed `FileData`.
     *
     * @param inputHex The hexadecimal input data as a `QString`.
     * @return A `QSharedPointer<FileData>` containing the parsed data.
     */
    QSharedPointer<FileData> processFile001Bytes(const QString& inputHex);

    /**
     * @brief Processes bytes for (Client Card - Inputs (From backend to meter) - Constant Main Information 2) File 002.
     *
     * This method processes the hexadecimal input data for file 002 and returns the parsed `FileData`.
     *
     * @param inputHex The hexadecimal input data as a `QString`.
     * @return A `QSharedPointer<FileData>` containing the parsed data.
     */
    QSharedPointer<FileData> processFile002Bytes(const QString& inputHex);

    /**
     * @brief Processes bytes for (Client Card - Inputs (From backend to meter) - Changed Main Data) File 003.
     *
     * This method processes the hexadecimal input data for file 003 and returns the parsed `FileData`.
     *
     * @param inputHex The hexadecimal input data as a `QString`.
     * @return A `QSharedPointer<FileData>` containing the parsed data.
     */
    QSharedPointer<FileData> processFile003Bytes(const QString& inputHex);

    /**
     * @brief Processes bytes for (Client Card - Inputs (From backend to meter) - Charge and Deduction Data) File 004.
     *
     * This method processes the hexadecimal input data for file 004 and returns the parsed `FileData`.
     *
     * @param inputHex The hexadecimal input data as a `QString`.
     * @return A `QSharedPointer<FileData>` containing the parsed data.
     */
    QSharedPointer<FileData> processFile004Bytes(const QString& inputHex);

    /**
     * @brief Processes bytes for (Client Card - Inputs (From backend to meter) - (Client Data + Tariff Data + Friendly Hours)) File 005.
     *
     * This method processes the hexadecimal input data for file 005 and returns the parsed `FileData`.
     *
     * @param inputHex The hexadecimal input data as a `QString`.
     * @return A `QSharedPointer<FileData>` containing the parsed data.
     */
    QSharedPointer<FileData> processFile005Bytes(const QString& inputHex);

    /**
     * @brief Processes bytes for (Maintenance Card - Input (From backend to meter) - (Changed Main Data)) File 006.
     *
     * This method processes the hexadecimal input data for file 006 and returns the parsed `FileData`.
     *
     * @param inputHex The hexadecimal input data as a `QString`.
     * @return A `QSharedPointer<FileData>` containing the parsed data.
     */
    QSharedPointer<FileData> processFile006Bytes(const QString& inputHex);

    /**
     * @brief Processes bytes for (Retrieval Card - Input (From backend to meter) - Changed Main Data) File 007.
     *
     * This method processes the hexadecimal input data for file 007 and returns the parsed `FileData`.
     *
     * @param inputHex The hexadecimal input data as a `QString`.
     * @return A `QSharedPointer<FileData>` containing the parsed data.
     */
    QSharedPointer<FileData> processFile007Bytes(const QString& inputHex);

    /**
     * @brief Processes bytes for (Client Card - Output (From meter to backend) - Changed Main Data) File 0010.
     *
     * This method processes the hexadecimal input data for file 0010 and returns the parsed `FileData`.
     *
     * @param inputHex The hexadecimal input data as a `QString`.
     * @return A `QSharedPointer<FileData>` containing the parsed data.
     */
    QSharedPointer<FileData> processFile0010Bytes(const QString& inputHex);

    /**
     * @brief Processes bytes for (Client Card - Output (From meter to backend) - Charge + Deduction + Readings) File 0011.
     *
     * This method processes the hexadecimal input data for file 0011 and returns the parsed `FileData`.
     *
     * @param inputHex The hexadecimal input data as a `QString`.
     * @return A `QSharedPointer<FileData>` containing the parsed data.
     */
    QSharedPointer<FileData> processFile0011Bytes(const QString& inputHex);

    /**
     * @brief Processes bytes for (Client Card - Output (From meter to backend) - Client Data + Tariff + Friendly Hours) File 0012.
     *
     * This method processes the hexadecimal input data for file 0012 and returns the parsed `FileData`.
     *
     * @param inputHex The hexadecimal input data as a `QString`.
     * @return A `QSharedPointer<FileData>` containing the parsed data.
     */
    QSharedPointer<FileData> processFile0012Bytes(const QString& inputHex);

    /**
     * @brief Processes bytes for (Client Card - Output (From meter to backend) - Credit Info) File 0013.
     *
     * This method processes the hexadecimal input data for file 0013 and returns the parsed `FileData`.
     *
     * @param inputHex The hexadecimal input data as a `QString`.
     * @return A `QSharedPointer<FileData>` containing the parsed data.
     */
    QSharedPointer<FileData> processFile0013Bytes(const QString& inputHex);

    /**
     * @brief Processes bytes for (Client Card - Output (From meter to backend) - Alarm Info + Tamper Info) File 0014.
     *
     * This method processes the hexadecimal input data for file 0014 and returns the parsed `FileData`.
     *
     * @param inputHex The hexadecimal input data as a `QString`.
     * @return A `QSharedPointer<FileData>` containing the parsed data.
     */
    QSharedPointer<FileData> processFile0014Bytes(const QString& inputHex);

    /**
     * @brief Processes bytes for (Configuration Card - Output (From meter to backend) - Meter Report (Multi Objects)) File 0015.
     *
     * This method processes the hexadecimal input data for file 0015 and returns the parsed `FileData`.
     *
     * @param inputHex The hexadecimal input data as a `QString`.
     * @return A `QSharedPointer<FileData>` containing the parsed data.
     */
    QSharedPointer<FileData> processFile0015Bytes(const QString& inputHex);

    /**
     * @brief Processes bytes for (Retrieval Card - Output (From meter to backend) - Retrieve Report File 1) File 0016.
     *
     * This method processes the hexadecimal input data for file 0016 and returns the parsed `FileData`.
     *
     * @param inputHex The hexadecimal input data as a `QString`.
     * @return A `QSharedPointer<FileData>` containing the parsed data.
     */
    QSharedPointer<FileData> processFile0016Bytes(const QString& inputHex);

    /** Holds the parsed file data. */
    QSharedPointer<FileData> fileData;
};

#endif // FILEPARSERMODEL_H
