#include "filedata.h"

FileData::FileData(QObject *parent) : QObject(parent) {
    initializeFileMetadata();
    initializeFields();
}


void FileData::initializeFileMetadata() {
    FileID = {
        "0001", "0002", "0003", "0004", "0005", "0006",
        "0007", "0010", "0011", "0012", "0013", "0014", "0015"
    };

    Description = {
        "File 0001 (Constant Main Information 1)",
        "File 0002 (Constant Main Information 2)",
        "File 0003 (Changed Main Data)",
        "File 0004 (Charge and Deduction Data)",
        "File 0005 (Client Data + Tariff Data + Friendly Hours)",
        "File 0006 (Changed Main Data)",
        "File 0007 (Changed Main Data)",
        "File 0010 (Changed Main Data)",
        "File 0011 (Charge + Deduction + Readings)",
        "File 0012 (Client Data + Tariff + Friendly Hours)",
        "File 0013 (Credit Info)",
        "File 0014 (Alarm Info + Tamper Info)",
        "File 0015 (Meter Report) (Multi Objects)"
    };

    FieldBytesNumber = {
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9
    };
}

void FileData::initializeFields() {
    auto addField = [this](int index, const QString& name, int byteNum,
                           int start, int end, const QVector<QString>& parsedVal,
                           const QVector<QString>& hexVal) {


        if (FieldName.size() <= index) {
            FieldName.resize(index + 1);
            FieldByteNumber.resize(index + 1);
            FieldByteIndexStart.resize(index + 1);
            FieldByteIndexEnd.resize(index + 1);
            FieldParsedValue.resize(index + 1);
        }

        FieldName[index] = name;
        FieldByteNumber[index] = byteNum;
        FieldByteIndexStart[index] = start;
        FieldByteIndexEnd[index] = end;
        FieldParsedValue[index] = parsedVal;
    };

    // Initialize fields with sample metadata for this example
    addField(0, "Card Function", 1, 0, 1, QVector<QString>({"0"}), QVector<QString>({"0x00"}));
    addField(1, "Utility ID", 1, 1, 2, QVector<QString>({"0"}), QVector<QString>({"0x00"}));
}
