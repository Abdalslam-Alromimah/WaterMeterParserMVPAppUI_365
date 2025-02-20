#include "filedata.h"
#include "WaterMeterParser_Model.h"
#include <QString>
#include <QtEndian>
#include <QtDebug>
#include <QByteArray>
#include <QDate>
#include <QDateTime>
#include <array>

WaterMeterParserModel::WaterMeterParserModel(QObject* parent)
    : BaseModel(parent), fileData(new FileData()) {}

/**
 * @brief Parses a file based on the provided index and hexadecimal data.
 * @param fileIndex The index of the file type.
 * @param hexData The hexadecimal data of the file.
 * @return A shared pointer to the parsed FileData object.
 */

QSharedPointer<FileData> WaterMeterParserModel::parseFile(int fileIndex, const QString &hexData) {
    QString hexString = hexData;

    // Use fileParser to process input
    QString result;
    QSharedPointer<FileData> fileData = nullptr;

    switch (fileIndex) {
        case 1:
             fileData = processFile001Bytes(hexString);
            break;
        case 2:
            fileData = processFile002Bytes(hexString);
            break;
        case 3:
             fileData = processFile003Bytes(hexString);
            break;
        case 4:
             fileData = processFile004Bytes(hexString);
            break;
        case 5:
             fileData = processFile005Bytes(hexString);
            break;
        case 6:
             fileData = processFile006Bytes(hexString);
            break;
        case 7:
             fileData = processFile007Bytes(hexString);
            break;
        case 8:
             fileData = processFile0010Bytes(hexString);
            break;
        case 9:
             fileData = processFile0011Bytes(hexString);
            break;
        case 10:
            fileData = processFile0012Bytes(hexString);
            break;
        case 11:
             fileData = processFile0013Bytes(hexString);
            break;
        case 12:
            fileData = processFile0014Bytes(hexString);
            break;
        case 13:
             fileData = processFile0015Bytes(hexString);
            break;
       case 14:
         fileData = processFile0016Bytes(hexString);
        break;
        default:
            result = "Invalid file selected.";
            break;
    }
    qDebug() << result;
    emit parsingComplete(fileData, fileIndex);

    return fileData;
}

/**
         * @brief Processes bytes for (Client Card- Inputs (From backend to meter)- Constant Main Information 1) File 001.
         *
         *
         * @param inputHex The hexadecimal input data as a QString.
         * @return A QSharedPointer to the parsed FileData object.
         */
QSharedPointer<FileData> WaterMeterParserModel::processFile001Bytes(const QString &inputHex) {
    QString cleanedHex = inputHex;
    cleanedHex.replace(" ", "");
    QByteArray bytehex = QByteArray::fromHex(cleanedHex.toUtf8());
    // Clear existing data
    fileData->FieldParsedValue.clear();


    fileData->FileID.append("0001");

    //output

       quint8 cardFunction = qFromLittleEndian<quint8>(bytehex.mid(0,1).data());
       fileData->FieldParsedValue.append( {QString::number(cardFunction)});

       // Process Utility ID (byte 1)
       quint8 utilityId = qFromLittleEndian<quint8>(bytehex.mid(1,1).data());
       fileData->FieldParsedValue.append( {QString::number(utilityId)});




       return fileData;
}
/**
         * @brief Processes bytes for  (Client Card- Inputs (From backend to meter)- Constant Main Information 2) File 002.
         *
         * @param inputHex The hexadecimal input data as a QString.
         * @return A QSharedPointer to the parsed FileData object.
         */
QSharedPointer<FileData> WaterMeterParserModel::processFile002Bytes(const QString &inputHex) {
    QString cleanedHex = inputHex;
    cleanedHex.replace(" ", "");
    QByteArray hexBytes002 = QByteArray::fromHex(cleanedHex.toUtf8());

    // Clear existing data
    fileData->FieldParsedValue.clear();


    fileData->FileID.append("0002");

    // Process Bytes 1-4: Meter serial
    quint32 meterSerial = qFromLittleEndian<quint32>(hexBytes002.mid(0, 4).data());
    fileData->FieldParsedValue.append({QString::number(meterSerial)});


    // Process Bytes 5-8: Meter Initial Reading
    quint32 initialReading = qFromLittleEndian<quint32>(hexBytes002.mid(4, 4).data());
    fileData->FieldParsedValue.append({QString::number(initialReading)});


    // Process Byte 9: Diameter ID
    quint8 diameterID = qFromLittleEndian<quint8>(hexBytes002.mid(8, 1).data());
    fileData->FieldParsedValue.append({QString::number(diameterID * 0.5)});


    // Process Byte 10: Year of production
    quint8 yearOfProduction = qFromLittleEndian<quint8>(hexBytes002.mid(9, 1).data());
    fileData->FieldParsedValue.append({QString::number(yearOfProduction)});


    // Process Byte 11: Meter supplier ID
    quint8 supplierID = qFromLittleEndian<quint8>(hexBytes002.mid(10, 1).data());
    fileData->FieldParsedValue.append({QString::number(supplierID)});


    // Process Bytes 12-19: Subscription Number
    quint64 subscriptionNumber = qFromLittleEndian<quint64>(hexBytes002.mid(11, 8).data());
    fileData->FieldParsedValue.append({QString::number(subscriptionNumber)});


    return fileData;
}
/**
         * @brief Processes bytes for (Client Card- Inputs (From backend to meter)- Changed Main Data) File 003.
         *
         * @param inputHex The hexadecimal input data as a QString.
         * @return A QSharedPointer to the parsed FileData object.
         */

QSharedPointer<FileData> WaterMeterParserModel::processFile003Bytes(const QString &inputHex) {
    QString cleanedHex = inputHex;
    cleanedHex.replace(" ", "");
    QByteArray hexBytes003 = QByteArray::fromHex(cleanedHex.toUtf8());

    // Clear existing data
    fileData->FieldParsedValue.clear();


    fileData->FileID.append("0003");

    // Process Byte 1: Last server transaction date (Year)
    quint8 year = qFromLittleEndian<quint8>(hexBytes003.mid(0, 1).data());
    fileData->FieldParsedValue.append({QString::number(year)});


    // Process Byte 2: Last server transaction date (Month)
    quint8 month = qFromLittleEndian<quint8>(hexBytes003.mid(1, 1).data());
    fileData->FieldParsedValue.append({QString::number(month)});


    // Process Bytes 3-4: Last server transaction date (Minute)
    quint16 minute = qFromLittleEndian<quint16>(hexBytes003.mid(2, 2).data());
    fileData->FieldParsedValue.append({QString::number(minute)});


    // Process Byte 5: Card Status
    quint8 cardStatus = qFromLittleEndian<quint8>(hexBytes003.mid(4, 1).data());
    fileData->FieldParsedValue.append({QString::number(cardStatus)});


    // Process Byte 6: Meter Actions
    quint8 meterActions = qFromLittleEndian<quint8>(hexBytes003.mid(5, 1).data());
    fileData->FieldParsedValue.append({QString::number(meterActions)});


    // Process Byte 7: Valve maintenance cycle (Day of Month)
    quint8 valveDay = qFromLittleEndian<quint8>(hexBytes003.mid(6, 1).data());
    fileData->FieldParsedValue.append({QString::number(valveDay)});


    // Process Bytes 8-9: Valve maintenance cycle (Minute)
    quint16 valveMinute = qFromLittleEndian<quint16>(hexBytes003.mid(7, 2).data());
    fileData->FieldParsedValue.append({QString::number(valveMinute)});


    return fileData;
}
/**
         * @brief Processes bytes for (Client Card- Inputs (From backend to meter)- Charge and Deduction Data) File 004.
         *
         * @param inputHex The hexadecimal input data as a QString.
         * @return A QSharedPointer to the parsed FileData object.
         */
QSharedPointer<FileData> WaterMeterParserModel::processFile004Bytes(const QString &inputHex) {

    QString cleanedHex = inputHex;
    cleanedHex.replace(" ", "");
    QByteArray hexBytes004 = QByteArray::fromHex(cleanedHex.toUtf8());
    QString result;

    // Clear existing data
    fileData->FieldParsedValue.clear();


    fileData->FileID.append("0004");
    // Process Bytes 1-4: Charge ID

   quint32 chargeID = qFromLittleEndian<quint8>(hexBytes004.mid(0, 4).data());
    fileData->FieldParsedValue.append({QString::number(chargeID)});

    // Process Bytes 5-12:  Charge Amount

   quint64 chargeAmount = qFromLittleEndian<quint64>(hexBytes004.mid(4, 8).data());
   fileData->FieldParsedValue.append({QString::number(chargeAmount)});

    //Process Byte 13-16 Replacement Charge ID

     quint32 ReplacementChargeID = qFromLittleEndian<quint32>(hexBytes004.mid(12, 4).data());
     fileData->FieldParsedValue.append({QString::number(ReplacementChargeID)});


    //Process Byte 17-24: Replacement Charge Amount

     quint64 ReplacementChargeAmount=qFromBigEndian<quint64>(hexBytes004.mid(16,8).data());
     fileData->FieldParsedValue.append({QString::number(ReplacementChargeAmount)});


    // Process Byte 25-32: Low Credit Limit Value
     quint64 LowCreditLimitValue = qFromLittleEndian<quint64>(hexBytes004.mid(24, 8).data());
     fileData->FieldParsedValue.append({QString::number(LowCreditLimitValue)});



    //Process Byte 33: Low credit percentage value 5

     quint8 Lowcreditpercentagevalue=qFromBigEndian<quint8>(hexBytes004.mid(32,1).data());
     fileData->FieldParsedValue.append({QString::number(Lowcreditpercentagevalue)});



    // Process Byte 34: Low credit limit option 6

      quint8 Lowcreditlimitoption=qFromBigEndian<quint8>(hexBytes004.mid(33,1).data());
      fileData->FieldParsedValue.append({QString::number(Lowcreditlimitoption)});



    //Process Bytes 35-38: Deduction ID 7

     quint32 DeductionID=qFromLittleEndian<quint32>(hexBytes004.mid(34,4).data());
     fileData->FieldParsedValue.append({QString::number(DeductionID)});


    //Process Byte 39: Number of cycles 8

     quint8 Numberofcycles=qFromLittleEndian<quint8>(hexBytes004.mid(38,1).data());
     fileData->FieldParsedValue.append({QString::number(Numberofcycles)});


    //Process Bytes 40-47: Deducted value 9


     quint64 Deductedvalue = qFromLittleEndian<quint64>(hexBytes004.mid(39, 8).data());
     fileData->FieldParsedValue.append({QString::number(Deductedvalue)});



    //Process Byte 48: Deduction start cycle (year) 10

     quint8 Deductionstartcycleyear=qFromLittleEndian<quint8>(hexBytes004.mid(47,1).data());
     fileData->FieldParsedValue.append({QString::number(Deductionstartcycleyear)});



    //Process Byte 49: Deduction start cycle (month) 11

     quint8 Deductionstartcyclemonth=qFromLittleEndian<quint8>(hexBytes004.mid(48,1).data());
     fileData->FieldParsedValue.append({QString::number(Deductionstartcyclemonth)});




    return fileData;
}
/**
         * @brief Processes bytes for (Client Card- Inputs (From backend to meter)- (Client Data + Tariff Data + Friendly Hours)) File 005.
         *
         * @param inputHex The hexadecimal input data as a QString.
         * @return A QSharedPointer to the parsed FileData object.
         */
QSharedPointer<FileData> WaterMeterParserModel::processFile005Bytes(const QString &inputHex) {
    QString cleanedHex = inputHex;
    cleanedHex.replace(" ", "");
    QByteArray hexBytes005 = QByteArray::fromHex(cleanedHex.toUtf8());
    if(!fileData){
        fileData=QSharedPointer<FileData>(new FileData);
    }
    // Clear existing data
    fileData->FieldParsedValue.clear();


    fileData->FileID.append("0005");

    // Process Byte 1: Service point category ID
    quint8 servicePointCategoryID = qFromLittleEndian<quint8>(hexBytes005.mid(0, 1).data());
    fileData->FieldParsedValue.append({QString::number(servicePointCategoryID)});


    // Process Byte 2: Service point activity ID
    quint8 servicePointActivityID = qFromLittleEndian<quint8>(hexBytes005.mid(1, 1).data());
    fileData->FieldParsedValue.append({QString::number(servicePointActivityID)});


    // Process Byte 3: Service cut flag
    quint8 serviceCutFlag = qFromLittleEndian<quint8>(hexBytes005.mid(2, 1).data());
    fileData->FieldParsedValue.append({QString::number(serviceCutFlag)});


    // Process Bytes 4-5: Number of units
    quint16 numberOfUnits = qFromLittleEndian<quint16>(hexBytes005.mid(3, 2).data());
    fileData->FieldParsedValue.append({QString::number(numberOfUnits)});


    // Process Byte 6: Sewage service flag
    quint8 sewageServiceFlag = qFromLittleEndian<quint8>(hexBytes005.mid(5, 1).data());
    fileData->FieldParsedValue.append({QString::number(sewageServiceFlag)});


    // Process Bytes 7-10: tariffId field
    quint32 tariffId = qFromLittleEndian<quint32>(hexBytes005.mid(6, 4).data());
    fileData->FieldParsedValue.append({QString::number(tariffId)});


    // Process Bytes 11-12: Monthly fixed fees
    quint16 monthlyFixedFees = qFromLittleEndian<quint16>(hexBytes005.mid(10, 2).data());
    fileData->FieldParsedValue.append({QString::number(monthlyFixedFees)});


    // Process Bytes 13-14: Monthly fixed fees per unit
    quint16 monthlyFixedFeesPerUnit = qFromLittleEndian<quint16>(hexBytes005.mid(12, 2).data());
    fileData->FieldParsedValue.append({QString::number(monthlyFixedFeesPerUnit)});


    // Process Bytes 15-46: Monthly fixed step fees
    QVector<QString> monthlyFixedStepFees;
    for (int i = 0; i < 16; ++i) {
        quint16 fee = qFromLittleEndian<quint16>(hexBytes005.mid(14 + i * 2, 2).data());
        monthlyFixedStepFees.append(QString::number(fee));
    }
    fileData->FieldParsedValue.append(monthlyFixedStepFees);


    // Process Bytes 47-48: Monthly fixed fees per cubic meter
    quint16 monthlyFixedFeesPerCubicMeter = qFromLittleEndian<quint16>(hexBytes005.mid(46, 2).data());
    fileData->FieldParsedValue.append({QString::number(monthlyFixedFeesPerCubicMeter)});


    // Process Byte 49: Monthly fixed fees options
    quint8 monthlyFixedFeesOptions = qFromLittleEndian<quint8>(hexBytes005.mid(48, 1).data());
    fileData->FieldParsedValue.append({QString::number(monthlyFixedFeesOptions)});



    // Process Bytes 50-51: Accumulated tier flag
    quint16 accumulatedTierFlag = qFromLittleEndian<quint16>(hexBytes005.mid(49, 2).data());
    fileData->FieldParsedValue.append({QString::number(accumulatedTierFlag)});


    // Process Bytes 52-83: Consumption price
    QVector<QString> consumptionPrice;
    for (int i = 0; i < 16; ++i) {
        quint16 fee = qFromLittleEndian<quint16>(hexBytes005.mid(51 + i * 2, 2).data());
        consumptionPrice.append(QString::number(fee));
    }
    fileData->FieldParsedValue.append(consumptionPrice);


    // Process Bytes 84-115: Max consumption per tier
    QVector<QString> maxConsumptionPerTier;
    for (int i = 0; i < 16; ++i) {
        quint16 fee = qFromLittleEndian<quint16>(hexBytes005.mid(83 + i * 2, 2).data());
        maxConsumptionPerTier.append(QString::number(fee));
    }
    fileData->FieldParsedValue.append(maxConsumptionPerTier);


    // Process Bytes 116-117: Sewage percentage
    quint16 sewagePercentage = qFromLittleEndian<quint16>(hexBytes005.mid(115, 2).data());
    fileData->FieldParsedValue.append({QString::number(sewagePercentage)});


    // Process Bytes 118-119: Sewage price
    quint16 sewagePrice = qFromLittleEndian<quint16>(hexBytes005.mid(117, 2).data());
    fileData->FieldParsedValue.append({QString::number(sewagePrice)});


    // Process Byte 120: Start date (Year)
    quint8 startDateYear = qFromLittleEndian<quint8>(hexBytes005.mid(119, 1).data());
    fileData->FieldParsedValue.append({QString::number( startDateYear)});


    // Process Byte 121: Start date (Month)
    quint8 startDateMonth = qFromLittleEndian<quint8>(hexBytes005.mid(120, 1).data());
    fileData->FieldParsedValue.append({QString::number(startDateMonth), QDate::longMonthName(startDateMonth)});


    // Process Bytes 122-123: Start date (Min)
    quint16 startDateMin = qFromLittleEndian<quint16>(hexBytes005.mid(121, 2).data());
    fileData->FieldParsedValue.append({QString::number(startDateMin)});


    // Process Byte 124: Units tiers table flag
    quint8 unitsTiersTableFlag = qFromLittleEndian<quint8>(hexBytes005.mid(123, 1).data());
    fileData->FieldParsedValue.append({QString::number(unitsTiersTableFlag), unitsTiersTableFlag == 1 ? "ON" : "OFF"});




    // Process Byte 129: Workdays
    quint8 workdaysFlag = qFromLittleEndian<quint8>(hexBytes005.mid(128, 1).data());
    QStringList activeDays;
    quint8 activeDaysint = 0;

    QStringList daysOfWeek = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri","Sat","NoN"};
    for (int i = 0; i<8; ++i) {
        if (workdaysFlag & (1 << 7-i)) {
            activeDays.append(daysOfWeek[7-i]);
        }


    }
    fileData->FieldParsedValue.append({activeDays.join(", ")});



    // Process Byte 130: Start Working Hour
    quint8 startWorkingHour = qFromLittleEndian<quint8>(hexBytes005.mid(129, 1).data());
    QTime startTime(startWorkingHour, 0);
    fileData->FieldParsedValue.append({startTime.toString("h:mm AP")});


    // Process Byte 131: End Working Hour
    quint8 endWorkingHour = qFromLittleEndian<quint8>(hexBytes005.mid(130, 1).data());
    QTime endTime(endWorkingHour, 0);
    fileData->FieldParsedValue.append({endTime.toString("h:mm AP")});


    // Process Byte 132: Overdraft amount of time
    quint8 overdraftAmountOfTime = qFromLittleEndian<quint8>(hexBytes005.mid(131, 1).data());
    fileData->FieldParsedValue.append({QString::number(overdraftAmountOfTime)});


    // Process Bytes 133-140: Overdraft amount of money
    quint64 overdraftAmountOfMoney = qFromLittleEndian<quint64>(hexBytes005.mid(132, 8).data());
    fileData->FieldParsedValue.append({QString::number(overdraftAmountOfMoney)});


    // Process Bytes 141-165: Holiday month
    QVector<QString> holidayMonths;
    for (int i = 0; i < 25; ++i) {
        quint8 holidayMonth = qFromLittleEndian<quint8>(hexBytes005.mid(140 + i, 1).data());
        holidayMonths.append(QString::number(holidayMonth));
    }
    fileData->FieldParsedValue.append(holidayMonths);


    // Process Bytes 166-190: Holiday day
    QVector<QString> holidayDays;
    for (int i = 0; i < 25; ++i) {
        quint8 holidayDay = qFromLittleEndian<quint8>(hexBytes005.mid(165 + i, 1).data());
        holidayDays.append(QString::number(holidayDay));
    }
    fileData->FieldParsedValue.append(holidayDays);


    // process Bytes 125-128: Friendly Hours ID = 0x00000002 = 2

    quint16 FriendlyHoursID = qFromLittleEndian<quint16>(hexBytes005.mid(124, 4).data());
    fileData->FieldParsedValue.append({QString::number(FriendlyHoursID)});


    return fileData;
}
/**
         * @brief Processes bytes for (Maintenance Card -Input (From backend to meter) - (Changed Main Data))  File 006.
         *
         * @param inputHex The hexadecimal input data as a QString.
         * @return A QSharedPointer to the parsed FileData object.
         */
QSharedPointer<FileData> WaterMeterParserModel::processFile006Bytes(const QString &inputHex) {
    QString cleanedHex = inputHex;
    cleanedHex.replace(" ", "");
    QByteArray hexBytes006 = QByteArray::fromHex(cleanedHex.toUtf8());

    // Clear existing data
    fileData->FieldParsedValue.clear();


    fileData->FileID.append("0006");

    // Process Byte 1: Last server transaction date (Year)
    quint8 year = qFromLittleEndian<quint8>(hexBytes006.mid(0, 1).data());
    fileData->FieldParsedValue.append({QString::number(year)});


    // Process Byte 2: Last server transaction date (Month)
    quint8 month = qFromLittleEndian<quint8>(hexBytes006.mid(1, 1).data());
    fileData->FieldParsedValue.append({QString::number(month)});

    // Process Bytes 3-4: Last server transaction date (Minutes)
    quint16 minutes = qFromLittleEndian<quint16>(hexBytes006.mid(2, 2).data());
    fileData->FieldParsedValue.append({QString::number(minutes)});


    // Process Byte 5: Card Status
    quint8 cardStatus = qFromLittleEndian<quint8>(hexBytes006.mid(4, 1).data());
    fileData->FieldParsedValue.append({QString::number(cardStatus)});
   /* fileData->FieldUnit.append({
        [cardStatus]() -> QString {
            switch (cardStatus) {
                case 0: return "New";
                case 1: return "Active";
                case 2: return "Initialized";
                case 3: return "Replacement";
                case 4: return "Inactive";
                default: return "Invalid value";
            }
        }()
    });*/

    // Process Byte 6: Meter Actions
    quint8 meterActions = qFromLittleEndian<quint8>(hexBytes006.mid(5, 1).data());
    fileData->FieldParsedValue.append({QString::number(meterActions)});

    // Process Byte 7: Meter Date (Year)
    quint8 meterYear = qFromLittleEndian<quint8>(hexBytes006.mid(6, 1).data());
    fileData->FieldParsedValue.append({QString::number(meterYear)});


    // Process Byte 8: Meter Date (Month)
    quint8 meterMonth = qFromLittleEndian<quint8>(hexBytes006.mid(7, 1).data());
    fileData->FieldParsedValue.append({QString::number(meterMonth)});


    // Process Bytes 9-10: Meter Date (Minutes)
    quint16 meterMinutes = qFromLittleEndian<quint16>(hexBytes006.mid(8, 2).data());
    fileData->FieldParsedValue.append({QString::number(meterMinutes)});


    return fileData;
}
/**
         * @brief Processes bytes for (Retrieval Card -Input (From backend to meter)-7 (Changed Main Data)) File 007.
         *
         * @param inputHex The hexadecimal input data as a QString.
         * @return A QSharedPointer to the parsed FileData object.
         */
QSharedPointer<FileData> WaterMeterParserModel::processFile007Bytes(const QString &inputHex){

    QString cleanedHex = inputHex;
    cleanedHex.replace(" ", "");
    QByteArray hexBytes007 = QByteArray::fromHex(cleanedHex.toUtf8());
    // Clear existing data
    fileData->FieldParsedValue.clear();


    fileData->FileID.append("0007");
    // Process Byte 1: Last server transaction date (Year) 0
    quint8 year = qFromLittleEndian<quint8>(hexBytes007.mid(0, 1).data());
    fileData->FieldParsedValue.append({QString::number(year)});


    // Process Byte 2: Last server transaction date (Month) 1
    quint8 month = qFromLittleEndian<quint8>(hexBytes007.mid(1, 1).data());
    fileData->FieldParsedValue.append({QString::number(month)});


    // Process Bytes 3-4: Last server transaction date (Minute) 2
    quint16 minute = qFromLittleEndian<quint16>(hexBytes007.mid(2, 2).data());
    fileData->FieldParsedValue.append({QString::number(minute)});


    // Process Byte 5: Card Status 3
    quint8 cardStatus = qFromLittleEndian<quint8>(hexBytes007.mid(4, 1).data());
    fileData->FieldParsedValue.append({QString::number(cardStatus)});


    // Process Byte 6: Meter Actions 4
    quint8 meterActions = qFromLittleEndian<quint8>(hexBytes007.mid(5, 1).data());
    fileData->FieldParsedValue.append({QString::number(meterActions)});



    return fileData;


}
/**
         * @brief Processes bytes for (Client Card-(Output (From meter to backend)-(Changed Main Data)) File 0010.
         *
         * @param inputHex The hexadecimal input data as a QString.
         * @return A QSharedPointer to the parsed FileData object.
         */
QSharedPointer<FileData> WaterMeterParserModel::processFile0010Bytes(const QString &inputHex){

    QString cleanedHex = inputHex;
    cleanedHex.replace(" ", "");
    QByteArray hexBytes0010 = QByteArray::fromHex(cleanedHex.toUtf8());
    // Clear existing data
    fileData->FieldParsedValue.clear();


    fileData->FileID.append("0010");
    // Process Byte 1: Last server transaction date (Year) 0
    quint8 year = qFromLittleEndian<quint8>(hexBytes0010.mid(0, 1).data());
    fileData->FieldParsedValue.append({QString::number(year)});


    // Process Byte 2: Last server transaction date (Month) 1
    quint8 month = qFromLittleEndian<quint8>(hexBytes0010.mid(1, 1).data());
    fileData->FieldParsedValue.append({QString::number(month)});


    // Process Bytes 3-4: Last server transaction date (Minute) 2
    quint16 minute = qFromLittleEndian<quint16>(hexBytes0010.mid(2, 2).data());
    fileData->FieldParsedValue.append({QString::number(minute)});


    // Process Byte 5: Card Status 3
    quint8 cardStatus = qFromLittleEndian<quint8>(hexBytes0010.mid(4, 1).data());
    fileData->FieldParsedValue.append({QString::number(cardStatus)});


    // Process Byte 6: Meter Actions 4
    quint8 meterActions = qFromLittleEndian<quint8>(hexBytes0010.mid(5, 1).data());
    fileData->FieldParsedValue.append({QString::number(meterActions)});



    return fileData;


}
/**
         * @brief Processes bytes for (Client Card-(Output (From meter to backend)- (Charge + Deduction + Readings)) File 0011.
         *
         * @param inputHex The hexadecimal input data as a QString.
         * @return A QSharedPointer to the parsed FileData object.
         */
QSharedPointer<FileData> WaterMeterParserModel::processFile0011Bytes(const QString &inputHex){
    QString cleanedHex = inputHex;
    cleanedHex.replace(" ", "");
    QByteArray hexBytes0011 = QByteArray::fromHex(cleanedHex.toUtf8());
    QString result;

    // Clear existing data
    fileData->FieldParsedValue.clear();


    fileData->FileID.append("0011");
    // Process Bytes 1-4: Charge ID
   quint32 chargeID = qFromLittleEndian<quint8>(hexBytes0011.mid(0, 4).data());
    fileData->FieldParsedValue.append({QString::number(chargeID)});


    // Process Bytes 5-12:  Charge Amount

       quint64 chargeAmount = qFromLittleEndian<quint64>(hexBytes0011.mid(4, 8).data());
       fileData->FieldParsedValue.append({QString::number(chargeAmount)});


       //Process Byte 13-16 Replacement Charge ID

            quint32 ReplacementChargeID = qFromLittleEndian<quint32>(hexBytes0011.mid(12, 4).data());
            fileData->FieldParsedValue.append({QString::number(ReplacementChargeID)});



        //Process Byte 17-24: Replacement Charge Amount

             quint64 ReplacementChargeAmount=qFromBigEndian<quint64>(hexBytes0011.mid(16,8).data());
             fileData->FieldParsedValue.append({QString::number(ReplacementChargeAmount)});


         // Process Byte 25-32: Low Credit Limit Value
              quint64 LowCreditLimitValue = qFromLittleEndian<quint64>(hexBytes0011.mid(24, 8).data());
              fileData->FieldParsedValue.append({QString::number(LowCreditLimitValue)});


          //Process Byte 33: Low credit percentage value 5

               quint8 Lowcreditpercentagevalue=qFromBigEndian<quint8>(hexBytes0011.mid(32,1).data());
               fileData->FieldParsedValue.append({QString::number(Lowcreditpercentagevalue)});



          // Process Byte 34: Low credit limit option 6

                quint8 Lowcreditlimitoption=qFromBigEndian<quint8>(hexBytes0011.mid(33,1).data());
                fileData->FieldParsedValue.append({QString::number(Lowcreditlimitoption)});


        //Process Bytes 35-38: Deduction ID 7

             quint32 DeductionID=qFromLittleEndian<quint32>(hexBytes0011.mid(34,4).data());
             fileData->FieldParsedValue.append({QString::number(DeductionID)});

         //Process Byte 39: Number of cycles 8

              quint8 Numberofcycles=qFromLittleEndian<quint8>(hexBytes0011.mid(38,1).data());
              fileData->FieldParsedValue.append({QString::number(Numberofcycles)});


        //Process Bytes 40-47: Deducted value 9


              quint64 Deductedvalue = qFromLittleEndian<quint64>(hexBytes0011.mid(39, 8).data());
              fileData->FieldParsedValue.append({QString::number(Deductedvalue)});

       //Process Byte 48: Deduction start cycle (year) 10

                   quint8 Deductionstartcycleyear=qFromLittleEndian<quint8>(hexBytes0011.mid(47,1).data());
                   fileData->FieldParsedValue.append({QString::number(Deductionstartcycleyear)});



      //Process Byte 49: Deduction start cycle (month) 11

                   quint8 Deductionstartcyclemonth=qFromLittleEndian<quint8>(hexBytes0011.mid(48,1).data());
                   fileData->FieldParsedValue.append({QString::number(Deductionstartcyclemonth)});

     //Process Bytes 50-57: Reading 12
                   quint64 Reading=qFromLittleEndian<quint64>(hexBytes0011.mid(49,8).data());
                   fileData->FieldParsedValue.append({QString::number(Reading)});


    //Process Byte 58-65:Current Month Reading 13

                    quint64 CurrentMonthReading=qFromLittleEndian<quint64>(hexBytes0011.mid(57,8).data());
                    fileData->FieldParsedValue.append({QString::number(CurrentMonthReading)});


    //Process Bytes 66-161: Monthly consumption 14


                    QVector<QString> Monthlyconsumption;
                        for (int i = 0; i < 12; ++i) {
                            quint64 Monthlyconsum = qFromLittleEndian<quint64>(hexBytes0011.mid(65 + i * 8, 8).data());
                            Monthlyconsumption.append(QString::number(Monthlyconsum));
                        }
                        fileData->FieldParsedValue.append(Monthlyconsumption);




  // Process Bytes 162-169: Overdraft consumption 15

                      quint64 Overdraftconsumption=qFromLittleEndian<quint64>(hexBytes0011.mid(162,8).data());
                      fileData->FieldParsedValue.append({QString::number(Overdraftconsumption)});



  //Process Bytes 170-177: Leakage Consumption 16

                      quint64 LeakageConsumption=qFromLittleEndian<quint64>(hexBytes0011.mid(169,8).data());
                      fileData->FieldParsedValue.append({QString::number(LeakageConsumption)});





    return fileData;
}
/**
         * @brief Processes bytes for (Client Card-(Output (From meter to backend)- (Client Data + Tariff + Friendly Hours) File 0012.
         *
         * @param inputHex The hexadecimal input data as a QString.
         * @return A QSharedPointer to the parsed FileData object.
         */
QSharedPointer<FileData> WaterMeterParserModel::processFile0012Bytes(const QString &inputHex) {
    QString cleanedHex = inputHex;
    cleanedHex.replace(" ", "");
    QByteArray hexBytes0012 = QByteArray::fromHex(cleanedHex.toUtf8());
    if(!fileData){
        fileData=QSharedPointer<FileData>(new FileData);
    }
    // Clear existing data
    fileData->FieldParsedValue.clear();


    fileData->FileID.append("0012");

    // Process Byte 1: Service point category ID
    quint8 servicePointCategoryID = qFromLittleEndian<quint8>(hexBytes0012.mid(0, 1).data());
    fileData->FieldParsedValue.append({QString::number(servicePointCategoryID)});


    // Process Byte 2: Service point activity ID
    quint8 servicePointActivityID = qFromLittleEndian<quint8>(hexBytes0012.mid(1, 1).data());
    fileData->FieldParsedValue.append({QString::number(servicePointActivityID)});


    // Process Byte 3: Service cut flag
    quint8 serviceCutFlag = qFromLittleEndian<quint8>(hexBytes0012.mid(2, 1).data());
    fileData->FieldParsedValue.append({QString::number(serviceCutFlag)});


    // Process Bytes 4-5: Number of units
    quint16 numberOfUnits = qFromLittleEndian<quint16>(hexBytes0012.mid(3, 2).data());
    fileData->FieldParsedValue.append({QString::number(numberOfUnits)});


    // Process Byte 6: Sewage service flag
    quint8 sewageServiceFlag = qFromLittleEndian<quint8>(hexBytes0012.mid(5, 1).data());
    fileData->FieldParsedValue.append({QString::number(sewageServiceFlag)});


    // Process Bytes 7-10: tariffId field
    quint32 tariffId = qFromLittleEndian<quint32>(hexBytes0012.mid(6, 4).data());
    fileData->FieldParsedValue.append({QString::number(tariffId)});


    // Process Bytes 11-12: Monthly fixed fees
    quint16 monthlyFixedFees = qFromLittleEndian<quint16>(hexBytes0012.mid(10, 2).data());
    fileData->FieldParsedValue.append({QString::number(monthlyFixedFees)});


    // Process Bytes 13-14: Monthly fixed fees per unit
    quint16 monthlyFixedFeesPerUnit = qFromLittleEndian<quint16>(hexBytes0012.mid(12, 2).data());
    fileData->FieldParsedValue.append({QString::number(monthlyFixedFeesPerUnit)});


    // Process Bytes 15-46: Monthly fixed step fees
    QVector<QString> monthlyFixedStepFees;
    for (int i = 0; i < 16; ++i) {
        quint16 fee = qFromLittleEndian<quint16>(hexBytes0012.mid(14 + i * 2, 2).data());
        monthlyFixedStepFees.append(QString::number(fee));
    }
    fileData->FieldParsedValue.append(monthlyFixedStepFees);


    // Process Bytes 47-48: Monthly fixed fees per cubic meter
    quint16 monthlyFixedFeesPerCubicMeter = qFromLittleEndian<quint16>(hexBytes0012.mid(46, 2).data());
    fileData->FieldParsedValue.append({QString::number(monthlyFixedFeesPerCubicMeter)});


    // Process Byte 49: Monthly fixed fees options
    quint8 monthlyFixedFeesOptions = qFromLittleEndian<quint8>(hexBytes0012.mid(48, 1).data());
    fileData->FieldParsedValue.append({QString::number(monthlyFixedFeesOptions)});



    // Process Bytes 50-51: Accumulated tier flag
    quint16 accumulatedTierFlag = qFromLittleEndian<quint16>(hexBytes0012.mid(49, 2).data());
    fileData->FieldParsedValue.append({QString::number(accumulatedTierFlag)});


    // Process Bytes 52-83: Consumption price
    QVector<QString> consumptionPrice;
    for (int i = 0; i < 16; ++i) {
        quint16 fee = qFromLittleEndian<quint16>(hexBytes0012.mid(51 + i * 2, 2).data());
        consumptionPrice.append(QString::number(fee));
    }
    fileData->FieldParsedValue.append(consumptionPrice);


    // Process Bytes 84-115: Max consumption per tier
    QVector<QString> maxConsumptionPerTier;
    for (int i = 0; i < 16; ++i) {
        quint16 fee = qFromLittleEndian<quint16>(hexBytes0012.mid(83 + i * 2, 2).data());
        maxConsumptionPerTier.append(QString::number(fee));
    }
    fileData->FieldParsedValue.append(maxConsumptionPerTier);


    // Process Bytes 116-117: Sewage percentage
    quint16 sewagePercentage = qFromLittleEndian<quint16>(hexBytes0012.mid(115, 2).data());
    fileData->FieldParsedValue.append({QString::number(sewagePercentage)});


    // Process Bytes 118-119: Sewage price
    quint16 sewagePrice = qFromLittleEndian<quint16>(hexBytes0012.mid(117, 2).data());
    fileData->FieldParsedValue.append({QString::number(sewagePrice)});


    // Process Byte 120: Start date (Year)
    quint8 startDateYear = qFromLittleEndian<quint8>(hexBytes0012.mid(119, 1).data());
    fileData->FieldParsedValue.append({QString::number(startDateYear)});


    // Process Byte 121: Start date (Month)
    quint8 startDateMonth = qFromLittleEndian<quint8>(hexBytes0012.mid(120, 1).data());
    fileData->FieldParsedValue.append({QString::number(startDateMonth), QDate::longMonthName(startDateMonth)});


    // Process Bytes 122-123: Start date (Min)
    quint16 startDateMin = qFromLittleEndian<quint16>(hexBytes0012.mid(121, 2).data());
    fileData->FieldParsedValue.append({QString::number(startDateMin)});


    // Process Byte 124: Units tiers table flag
    quint8 unitsTiersTableFlag = qFromLittleEndian<quint8>(hexBytes0012.mid(123, 1).data());
    fileData->FieldParsedValue.append({QString::number(unitsTiersTableFlag), unitsTiersTableFlag == 1 ? "ON" : "OFF"});




    // Process Byte 129: Workdays
    quint8 workdaysFlag = qFromLittleEndian<quint8>(hexBytes0012.mid(128, 1).data());
    QStringList activeDays;
    quint8 activeDaysint = 0;

    QStringList daysOfWeek = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri","Sat","NoN"};
    for (int i = 0; i<8; ++i) {
        if (workdaysFlag & (1 << 7-i)) {
            activeDays.append(daysOfWeek[7-i]);
        }


    }
    fileData->FieldParsedValue.append({activeDays.join(", ")});



    // Process Byte 130: Start Working Hour
    quint8 startWorkingHour = qFromLittleEndian<quint8>(hexBytes0012.mid(129, 1).data());
    QTime startTime(startWorkingHour, 0);
    fileData->FieldParsedValue.append({startTime.toString("h:mm AP")});


    // Process Byte 131: End Working Hour
    quint8 endWorkingHour = qFromLittleEndian<quint8>(hexBytes0012.mid(130, 1).data());
    QTime endTime(endWorkingHour, 0);
    fileData->FieldParsedValue.append({endTime.toString("h:mm AP")});


    // Process Byte 132: Overdraft amount of time
    quint8 overdraftAmountOfTime = qFromLittleEndian<quint8>(hexBytes0012.mid(131, 1).data());
    fileData->FieldParsedValue.append({QString::number(overdraftAmountOfTime)});


    // Process Bytes 133-140: Overdraft amount of money
    quint64 overdraftAmountOfMoney = qFromLittleEndian<quint64>(hexBytes0012.mid(132, 8).data());
    fileData->FieldParsedValue.append({QString::number(overdraftAmountOfMoney)});


    // Process Bytes 141-165: Holiday month
    QVector<QString> holidayMonths;
    for (int i = 0; i < 25; ++i) {
        quint8 holidayMonth = qFromLittleEndian<quint8>(hexBytes0012.mid(140 + i, 1).data());
        holidayMonths.append(QString::number(holidayMonth));
    }
    fileData->FieldParsedValue.append(holidayMonths);


    // Process Bytes 166-190: Holiday day
    QVector<QString> holidayDays;
    for (int i = 0; i < 25; ++i) {
        quint8 holidayDay = qFromLittleEndian<quint8>(hexBytes0012.mid(165 + i, 1).data());
        holidayDays.append(QString::number(holidayDay));
    }
    fileData->FieldParsedValue.append(holidayDays);


    // process Bytes 125-128: Friendly Hours ID = 0x00000002 = 2

    quint16 FriendlyHoursID = qFromLittleEndian<quint16>(hexBytes0012.mid(124, 4).data());
    fileData->FieldParsedValue.append({QString::number(FriendlyHoursID)});


    return fileData;
}
/**
         * @brief Processes bytes for (Client Card-(Output (From meter to backend)- (Credit Info)) File 0013.
         *
         * @param inputHex The hexadecimal input data as a QString.
         * @return A QSharedPointer to the parsed FileData object.
         */
QSharedPointer<FileData> WaterMeterParserModel::processFile0013Bytes(const QString &inputHex) {
    QString cleanedHex = inputHex;
    cleanedHex.replace(" ", "");
    QByteArray hexBytes013 = QByteArray::fromHex(cleanedHex.toUtf8());
    QString result;

    // Clear existing data
    fileData->FieldParsedValue.clear();


     fileData->FileID.append("0013");

    // Process Bytes 1-8: Credit Balance
    quint64 creditBalance = qFromLittleEndian<quint64>(hexBytes013.mid(0, 8).data());
     fileData->FieldParsedValue.append({QString::number(creditBalance)});


    // Process Bytes 9-16: Overdraft Credit
    quint64 overdraftCredit = qFromLittleEndian<quint64>(hexBytes013.mid(8, 8).data());
     fileData->FieldParsedValue.append({QString::number(overdraftCredit)});


    // Process Bytes 17-24: Consumed Credit
    quint64 consumedCredit = qFromLittleEndian<quint64>(hexBytes013.mid(16, 8).data());
    fileData->FieldParsedValue.append({QString::number(consumedCredit)});


    // Process Bytes 25-120: Consumed Credit Monthly (Array of 12 values)
    QVector<QString> consumedCreditMonthly;
    for (int i = 0; i < 12; ++i) {
         quint64 monthlyCredit = qFromLittleEndian<quint64>(hexBytes013.mid(24 + i * 8, 8).data());
        consumedCreditMonthly.append(QString::number(monthlyCredit));
    }
    fileData->FieldParsedValue.append(consumedCreditMonthly);


    // Process Bytes 121-128: Cumulative Charge
    quint64 cumulativeCharge = qFromLittleEndian<quint64>(hexBytes013.mid(120, 8).data());
    fileData->FieldParsedValue.append({QString::number(cumulativeCharge)});


    // Process Bytes 129-132: Active Tariff ID
    quint32 activeTariffID = qFromLittleEndian<quint32>(hexBytes013.mid(128, 4).data());
    fileData->FieldParsedValue.append({QString::number(activeTariffID)});


    // Process Byte 133: Active Tariff Start Date (Year)
    quint8 activeTariffYear = qFromLittleEndian<quint8>(hexBytes013.mid(132, 1).data());
     fileData->FieldParsedValue.append({QString::number(activeTariffYear)});


    // Process Byte 134: Active Tariff Start Date (Month)
    quint8 activeTariffMonth = qFromLittleEndian<quint8>(hexBytes013.mid(133, 1).data());
     fileData->FieldParsedValue.append({QString::number(activeTariffMonth)});


    // Process Bytes 135-136: Active Tariff Start Date (Min)
    quint16 activeTariffMin = qFromLittleEndian<quint16>(hexBytes013.mid(134, 2).data());
    fileData->FieldParsedValue.append({QString::number(activeTariffMin)});


    // Process Bytes 137-140: Other Tariff ID
    quint32 otherTariffID = qFromLittleEndian<quint32>(hexBytes013.mid(136, 4).data());
    fileData->FieldParsedValue.append({QString::number(otherTariffID)});


    return fileData;
}
/**
         * @brief Processes bytes for (Client Card-(Output (From meter to backend)- (Alarm Info + Tamper Info)) File 0014.
         *
         * @param inputHex The hexadecimal input data as a QString.
         * @return A QSharedPointer to the parsed FileData object.
         */

QSharedPointer<FileData> WaterMeterParserModel::processFile0014Bytes(const QString &inputHex) {
    QString cleanedHex = inputHex;
    cleanedHex.replace(" ", "");
    QByteArray hexBytes014 = QByteArray::fromHex(cleanedHex.toUtf8());
    QString result;

    // Clear existing data
    fileData->FieldParsedValue.clear();


    fileData->FileID.append("0014");

    // Process Bytes 1-28: Last occurrence date = Array(7 DateTime "y-mon-min")
    std::array<int, 7> offsets = {0, 4, 8, 12, 16, 20, 24};
    QStringList formattedDates;

    std::for_each(offsets.begin(), offsets.end(), [&formattedDates, &hexBytes014](int baseOffset) {
       quint8 year = static_cast<quint8>(hexBytes014[baseOffset]);
       quint8 month = static_cast<quint8>( hexBytes014[baseOffset + 1]);
        quint16 minutes = qFromLittleEndian<quint16>(hexBytes014.mid(baseOffset + 2, 2).data());
        QString dateStr = QString("0x%1-0x%2-0x%3")
                .arg(year, 2, 16, QChar('0'))
                .arg(month, 2, 16, QChar('0'))
                .arg(minutes, 4, 16, QChar('0'));

        formattedDates.append(dateStr);
    });

    fileData->FieldParsedValue.append(formattedDates.toVector());


    // Process Bytes 29-56: Last clearance date = Array(7 DateTime "y-mon-min")
    std::array<int, 7> offsets1 = {28, 32, 36, 40, 44, 48, 52};
    QStringList formattedDates1;

    std::for_each(offsets1.begin(), offsets1.end(), [&formattedDates1, &hexBytes014](int baseOffset) {
        quint8 year = static_cast<quint8>(hexBytes014[baseOffset]);
        quint8 month = static_cast<quint8>(hexBytes014[baseOffset + 1]);
        quint16 minutes = qFromLittleEndian<quint16>(hexBytes014.mid(baseOffset + 2, 2).data());
        QString dateStr = QString("0x%1-0x%2-0x%3")
                .arg(year, 2, 16, QChar('0'))
                .arg(month, 2, 16, QChar('0'))
                .arg(minutes, 4, 16, QChar('0'));

        formattedDates1.append(dateStr);
    });

    fileData->FieldParsedValue.append(formattedDates1.toVector());


    // Process Bytes 57-62: Tamper Count = Array(6)
    QVector<QString> tamperCountsHex;
    QVector<QString> tamperCountsDecimal;

    for (int i = 0; i < 6; ++i) {
        quint8 tamperCount = qFromLittleEndian<quint8>(hexBytes014.mid(56 + i, 1).data());
        tamperCountsHex.append(QString("0x%1").arg(tamperCount, 2, 16, QChar('0')));
        tamperCountsDecimal.append(QString::number(tamperCount));
    }

    fileData->FieldParsedValue.append(tamperCountsDecimal);


    // Process Bytes 63-86: Tamper Last occurrence date = Array(6 DateTime "y-mon-min")
    std::array<int, 6> offsets2 = {0, 4, 8, 12, 16, 20};
    QStringList formattedDates2;

    std::for_each(offsets2.begin(), offsets2.end(), [&formattedDates2, &hexBytes014](int baseOffset) {
        int actualOffset = 62 + baseOffset;
        if (actualOffset + 3 < hexBytes014.size()) {
            quint8 year = static_cast<quint8>(hexBytes014[actualOffset]);
            quint8 month = static_cast<quint8>(hexBytes014[actualOffset + 1]);
            quint16 minutes = qFromLittleEndian<quint16>(hexBytes014.mid(actualOffset + 2, 2).data());
            QString dateStr = QString("0x%1-0x%2-0x%3")
                    .arg(year, 2, 16, QChar('0'))
                    .arg(month, 2, 16, QChar('0'))
                    .arg(minutes, 4, 16, QChar('0')).toUpper();
            formattedDates2.append(dateStr);
        } else {
            formattedDates2.append("0x00-0x00-0x0000");
        }
    });

    fileData->FieldParsedValue.append(formattedDates2.toVector());


    // Process Byte 87: Valve History (Valve Status)
    quint8 valveStatus = qFromLittleEndian<quint8>(hexBytes014.mid(86, 1).data());
    QString valveStatusStr;
    switch (valveStatus) {
        case 0: valveStatusStr = "Open"; break;
        case 1: valveStatusStr = "Close"; break;
        case 2: valveStatusStr = "Error"; break;
    }

    fileData->FieldParsedValue.append({QString::number(valveStatus)});


    // Process Bytes 88-103: Array(4 DateTime "y-mon-min")
    std::array<int, 4> offsets3 = {0, 4, 8, 12};
    QStringList formattedDates3;

    std::for_each(offsets3.begin(), offsets3.end(), [&formattedDates3, &hexBytes014](int baseOffset) {
        int actualOffset = 87 + baseOffset;
        if (actualOffset + 3 < hexBytes014.size()) {
            quint8 year = static_cast<quint8>(hexBytes014[actualOffset]);
            quint8 month = static_cast<quint8>(hexBytes014[actualOffset + 1]);
            quint16 minutes = qFromLittleEndian<quint16>(hexBytes014.mid(actualOffset + 2, 2).data());
            QString dateStr = QString("0x%1-0x%2-0x%3")
                    .arg(year, 2, 16, QChar('0'))
                    .arg(month, 2, 16, QChar('0'))
                    .arg(minutes, 4, 16, QChar('0')).toUpper();
            formattedDates3.append(dateStr);
        } else {
            formattedDates3.append("0x00-0x00-0x0000");
        }
    });

    fileData->FieldParsedValue.append(formattedDates3.toVector());


    return fileData;
}
/**
         * @brief Processes bytes for (Configuration Card- Output (From meter to backend)-  (Meter Report) (Multi Objects)) File 0015.
         *
         * @param inputHex The hexadecimal input data as a QString.
         * @return A QSharedPointer to the parsed FileData object.
         */
QSharedPointer<FileData> WaterMeterParserModel::processFile0015Bytes(const QString &inputHex){

          //0015 (Meter Report) (Multi Objects)
                QString cleanedHex = inputHex;
                cleanedHex.replace(" ", "");
                QByteArray hexBytes015 = QByteArray::fromHex(cleanedHex.toUtf8());
                QString result;

                // Clear existing data
                fileData->FieldParsedValue.clear();


                fileData->FileID.append("0015");


            // process Byte 1 Last Meter date (Year) 0
            quint8 LastMeterTransactionDateYear = qFromLittleEndian<quint8>(hexBytes015.mid(0, 1).data());
             fileData->FieldParsedValue.append({QString::number(LastMeterTransactionDateYear)});

            //Process Byte 2  Last Meter date (Month) 1
            quint8 LastMeterTransactionDateMonth = qFromLittleEndian<quint8>(hexBytes015.mid(1, 1).data());
             fileData->FieldParsedValue.append({QString::number(LastMeterTransactionDateMonth)});


            //Process Byte 3-4 Last Meter date (Min) 2
            quint16 LastMeterTransactionDateMin = qFromLittleEndian<quint16>(hexBytes015.mid(2, 2).data());
            fileData->FieldParsedValue.append({QString::number(LastMeterTransactionDateMin)});



            // Process Byte 5:  Returned Meter Actions 3
            quint8 ReturnedMeterActions = qFromLittleEndian<quint8>(hexBytes015.mid(4, 1).data());
            fileData->FieldParsedValue.append({QString::number(ReturnedMeterActions)});


            //Process Byte 6-7: 0x0001 = Success  Error Code 4
            quint16 SuccessErrorCode = qFromLittleEndian<quint16>(hexBytes015.mid(5, 2).data());
            fileData->FieldParsedValue.append({QString::number(SuccessErrorCode)});


            //Process Byte 8:Not used 5
            quint8 Notused = qFromLittleEndian<quint8>(hexBytes015.mid(7, 1).data());
            fileData->FieldParsedValue.append({QString::number(Notused)});



        return fileData;

}
/**
         * @brief Processes bytes for(Retrieval Card -Output (From meter to backend)- 6 (Retrieve Report File 1) (Multi Objects)) File 0016.
         *
         * @param inputHex The hexadecimal input data as a QString.
         * @return A QSharedPointer to the parsed FileData object.
         */
QSharedPointer<FileData> WaterMeterParserModel::processFile0016Bytes(const QString &inputHex) {
    QString cleanedHex = inputHex;
    cleanedHex.replace(" ", "");
    QByteArray hexBytes016 = QByteArray::fromHex(cleanedHex.toUtf8());

    //Process Clear existing data
    fileData->FieldParsedValue.clear();

    fileData->FileID.append("0016");

    //Process Bytes 1-4: Last Transaction Date 0
    quint8 lastTransYear = qFromLittleEndian<quint8>(hexBytes016.mid(0, 1).data());
    quint8 lastTransMonth = qFromLittleEndian<quint8>(hexBytes016.mid(1, 1).data());
    quint16 lastTransMin = qFromLittleEndian<quint16>(hexBytes016.mid(2, 2).data());

    fileData->FieldParsedValue.append({QString::number(lastTransYear)});
    fileData->FieldParsedValue.append({QString::number(lastTransMonth)});
    fileData->FieldParsedValue.append({QString::number(lastTransMin)});
    //Process Byte 5: Returned Meter Actions 3
    quint8 returnedMeterActions = qFromLittleEndian<quint8>(hexBytes016.mid(4, 1).data());
    fileData->FieldParsedValue.append({QString::number(returnedMeterActions)});


    // Bytes 6-13: Subscription Number 4
    quint64 subscriptionNumber = qFromLittleEndian<quint64>(hexBytes016.mid(5, 8).data());
    fileData->FieldParsedValue.append({QString::number(subscriptionNumber)});


    // Service Point Data from
    //Process Bytes 14:  ServicePoint CategoryID 5
    quint8 servicePointCategoryID = qFromLittleEndian<quint8>(hexBytes016.mid(13, 1).data());
    //Process Bytes 15:  ServicePoint Sub CategoryID 6
    quint8 servicePointSubCategoryID = qFromLittleEndian<quint8>(hexBytes016.mid(14, 1).data());
    //Process Bytes 16:  Service Cut Flag 7
    quint8 serviceCutFlag = qFromLittleEndian<quint8>(hexBytes016.mid(15, 1).data());
    //Process Bytes 17: Number Of Units 8
    quint16 numberOfUnits = qFromLittleEndian<quint16>(hexBytes016.mid(16, 2).data());
    //Process Bytes 18: Sewage Service Flag 9
    quint8 sewageServiceFlag = qFromLittleEndian<quint8>(hexBytes016.mid(18, 1).data());

    fileData->FieldParsedValue.append({QString::number(servicePointCategoryID)});
    fileData->FieldParsedValue.append({QString::number(servicePointSubCategoryID)});
    fileData->FieldParsedValue.append({QString::number(serviceCutFlag)});
    fileData->FieldParsedValue.append({QString::number(numberOfUnits)});
    fileData->FieldParsedValue.append({QString::number(sewageServiceFlag)});







    // Charge Data (from File 11)
    //Process Bytes 20-23: charge ID 10
    quint32 chargeID = qFromLittleEndian<quint32>(hexBytes016.mid(19, 4).data());
    //Process Bytes 24-31: charge Amount 11
    quint64 chargeAmount = qFromLittleEndian<quint64>(hexBytes016.mid(23, 8).data());
    //Process Bytes 32-35: replacement Charge ID 12
    quint32 replacementChargeID = qFromLittleEndian<quint32>(hexBytes016.mid(31, 4).data());
    //Process Bytes 36-43: replacement Charge Amount 13
    quint64 replacementChargeAmount = qFromLittleEndian<quint64>(hexBytes016.mid(35, 8).data());

    fileData->FieldParsedValue.append({QString::number(chargeID)});
    fileData->FieldParsedValue.append({QString::number(chargeAmount)});
    fileData->FieldParsedValue.append({QString::number(replacementChargeID)});
    fileData->FieldParsedValue.append({QString::number(replacementChargeAmount)});






    // Credit Limit Data
    //Process Bytes 44-51: low Credit Limit Value 14
    quint64 lowCreditLimitValue = qFromLittleEndian<quint64>(hexBytes016.mid(43, 8).data());
    //Process Bytes 52: low Credit Percentage 15
    quint8 lowCreditPercentage = qFromLittleEndian<quint8>(hexBytes016.mid(51, 1).data());
    //Process Bytes 53: low Credit Limit Option 16
    quint8 lowCreditLimitOption = qFromLittleEndian<quint8>(hexBytes016.mid(52, 1).data());

    fileData->FieldParsedValue.append({QString::number(lowCreditLimitValue)});
    fileData->FieldParsedValue.append({QString::number(lowCreditPercentage)});
    fileData->FieldParsedValue.append({QString::number(lowCreditLimitOption)});





    // Deduction Data
    //Process Bytes 54-57: Deduction ID 17
    quint32 deductionID = qFromLittleEndian<quint32>(hexBytes016.mid(53, 4).data());
    //Process Bytes 58: Number Of Cycles 18
    quint8 numberOfCycles = qFromLittleEndian<quint8>(hexBytes016.mid(57, 1).data());
    //Process Bytes 59-66: Deducted Value 19
    quint64 deductedValue = qFromLittleEndian<quint64>(hexBytes016.mid(58, 8).data());
    //Process Bytes 67: Deduction StartYear 20
    quint8 deductionStartYear = qFromLittleEndian<quint8>(hexBytes016.mid(66, 1).data());
    //Process Bytes 68: Deduction StartMonth 21
    quint8 deductionStartMonth = qFromLittleEndian<quint8>(hexBytes016.mid(67, 1).data());

    fileData->FieldParsedValue.append({QString::number(deductionID)});
    fileData->FieldParsedValue.append({QString::number(numberOfCycles)});
    fileData->FieldParsedValue.append({QString::number(deductedValue)});
    fileData->FieldParsedValue.append({QString::number(deductionStartYear)});
    fileData->FieldParsedValue.append({QString::number(deductionStartMonth)});







    // Reading Data
    //Process Bytes 69-76: Reading 22
    quint64 reading = qFromLittleEndian<quint64>(hexBytes016.mid(68, 8).data());
    fileData->FieldParsedValue.append({QString::number(reading)});


    // Monthly Consumption Data (12 months)
    //Process Bytes 77-364: Monthly consumption 23
    QVector<QString> monthlyConsumption;
    for (int i = 0; i < 12; ++i) {
        quint64 consumption = qFromLittleEndian<quint64>(hexBytes016.mid(76 + i * 24, 24).data());
        monthlyConsumption.append(QString::number(consumption));
    }
        fileData->FieldParsedValue.append(monthlyConsumption);


     //Process Bytes 365-372: Overdraft consumption 24

    quint64 overdraftConsumption = qFromLittleEndian<quint64>(hexBytes016.mid(364, 8).data());
    //Process Bytes 373-380: Leakage Consumption 25

    quint64 leakageConsumption = qFromLittleEndian<quint64>(hexBytes016.mid(372, 8).data());

        fileData->FieldParsedValue.append({QString::number(overdraftConsumption)});
        fileData->FieldParsedValue.append({QString::number(leakageConsumption)});




    // Tariff Data
    //Process Bytes 381-384: Tariff ID 26

    quint32 tariffID = qFromLittleEndian<quint32>(hexBytes016.mid(380, 4).data());
    //Process Bytes 385-386: Monthly Fixed Fees 27
    quint16 monthlyFixedFees = qFromLittleEndian<quint16>(hexBytes016.mid(384, 2).data());
    //Process Bytes 387-388: Monthly FixedFees Per Unit 28
    quint16 monthlyFixedFeesPerUnit = qFromLittleEndian<quint16>(hexBytes016.mid(386, 2).data());

    fileData->FieldParsedValue.append({QString::number(tariffID)});
    fileData->FieldParsedValue.append({QString::number(monthlyFixedFees)});
    fileData->FieldParsedValue.append({QString::number(monthlyFixedFeesPerUnit)});





    //Process Bytes 389-420: Monthly Fixed Step Fees 29
    QVector<QString> monthlyFixedStepFees;
    for (int i = 0; i < 16; ++i) {
        quint16 fee = qFromLittleEndian<quint16>(hexBytes016.mid(388 + i * 2, 2).data());
        monthlyFixedStepFees.append(QString::number(fee));
    }
    fileData->FieldParsedValue.append(monthlyFixedStepFees);


        //Process Bytes 421-422: Monthly Fixed Fees Per Cubic Meter 30
    quint16 monthlyFixedFeesPerCubicMeter = qFromLittleEndian<quint16>(hexBytes016.mid(420, 2).data());
    fileData->FieldParsedValue.append({QString::number(monthlyFixedFeesPerCubicMeter)});



    //Process Bytes 423: Monthly Fixed Fees Options 31
    quint8 monthlyFixedFeesOptions = qFromLittleEndian<quint8>(hexBytes016.mid(422, 1).data());
    fileData->FieldParsedValue.append({QString::number(monthlyFixedFeesOptions)});


    //Process Bytes 424-425: Accumulated Tier Flag 32
    quint16 accumulatedTierFlag = qFromLittleEndian<quint16>(hexBytes016.mid(423, 2).data());
    fileData->FieldParsedValue.append({QString::number(accumulatedTierFlag)});



    //Process Bytes 426-457: Consumption Price 33
    QVector<QString> consumptionPrice;
    for (int i = 0; i < 16; ++i) {
        quint16 price = qFromLittleEndian<quint16>(hexBytes016.mid(425 + i * 2, 2).data());
        consumptionPrice.append(QString::number(price));
    }
    fileData->FieldParsedValue.append(consumptionPrice);



    //Process Bytes 458-489:  Max Consumption Per Tier 34
    QVector<QString> maxConsumptionPerTier;
    for (int i = 0; i < 16; ++i) {
        quint16 maxConsumption = qFromLittleEndian<quint16>(hexBytes016.mid(457 + i * 2, 2).data());
        maxConsumptionPerTier.append(QString::number(maxConsumption));
    }
    fileData->FieldParsedValue.append(maxConsumptionPerTier);


    // Sewage Data
    //Process Bytes 490-491: Sewage Percentage 35
    quint16 sewagePercentage = qFromLittleEndian<quint16>(hexBytes016.mid(489, 2).data());
    //Process Bytes 492-493: replacement Charge Amount 36
    quint16 sewagePrice = qFromLittleEndian<quint16>(hexBytes016.mid(491, 2).data());

    fileData->FieldParsedValue.append({QString::number(sewagePercentage)});
    fileData->FieldParsedValue.append({QString::number(sewagePrice)});




    // Start Date
    //Process Bytes 494: replacement Charge Amount 37
    quint8 startYear = qFromLittleEndian<quint8>(hexBytes016.mid(493, 1).data());
    //Process Bytes 495: replacement Charge Amount 38
    quint8 startMonth = qFromLittleEndian<quint8>(hexBytes016.mid(494, 1).data());
    //Process Bytes 496: replacement Charge Amount 39
    quint16 startMin = qFromLittleEndian<quint16>(hexBytes016.mid(495, 2).data());

    fileData->FieldParsedValue.append({QString::number(startYear )});
    fileData->FieldParsedValue.append({QString::number(startMonth)});
    fileData->FieldParsedValue.append({QString::number(startMin)});






    //Process Bytes 498: Units Tiers Table Flag 40
    quint8 unitsTiersTableFlag = qFromLittleEndian<quint8>(hexBytes016.mid(497, 1).data());
    fileData->FieldParsedValue.append({QString::number(unitsTiersTableFlag)});



    //Process Bytes 499-502: Friendly Hours Data 41
    quint32 friendlyHoursID = qFromLittleEndian<quint32>(hexBytes016.mid(498, 4).data());
    //Process Bytes 503: Work Days 42
    quint8 workdays = qFromLittleEndian<quint8>(hexBytes016.mid(502, 1).data());
    //Process Bytes 504: Start Working Hour 43
    quint8 startWorkingHour = qFromLittleEndian<quint8>(hexBytes016.mid(503, 1).data());

    // Process Byte 505: 44

    quint8 endWorkingHour = qFromLittleEndian<quint8>(hexBytes016.mid(504, 1).data());

       fileData->FieldParsedValue.append({QString::number(friendlyHoursID)});
       fileData->FieldParsedValue.append({QString::number(workdays)});
       fileData->FieldParsedValue.append({QString::number(startWorkingHour)});
       fileData->FieldParsedValue.append({QString::number(endWorkingHour)});


       // Process Byte 506: Overdraft amount of time 45
       quint8 overdraftAmountOfTime = qFromLittleEndian<quint8>(hexBytes016.mid(505, 1).data());
       fileData->FieldParsedValue.append({QString::number(overdraftAmountOfTime)});


       // Process Bytes 507-514: Overdraft amount of money 46
       quint64 overdraftAmountOfMoney = qFromLittleEndian<quint64>(hexBytes016.mid(506, 8).data());
       fileData->FieldParsedValue.append({QString::number(overdraftAmountOfMoney)});


       // Process Bytes 515-539: Holiday month 47
       QVector<QString> holidayMonths;
       for (int i = 0; i < 25; ++i) {
           quint8 holidayMonth = qFromLittleEndian<quint8>(hexBytes016.mid(514 + i, 1).data());
           holidayMonths.append(QString::number(holidayMonth));
       }
       fileData->FieldParsedValue.append(holidayMonths);


       // Process Bytes 540-564: Holiday day 48
       QVector<QString> holidayDays;
       for (int i = 0; i < 25; ++i) {
           quint8 holidayDay = qFromLittleEndian<quint8>(hexBytes016.mid(539 + i, 1).data());
           holidayDays.append(QString::number(holidayDay));
       }
       fileData->FieldParsedValue.append(holidayDays);


       // Process Bytes 565-572: Credit balance 49
       quint64 creditBalance = qFromLittleEndian<quint64>(hexBytes016.mid(564, 8).data());
       fileData->FieldParsedValue.append({QString::number(creditBalance)});


       // Process Bytes 573-580: Overdraft credit 50
       quint64 overdraftCredit = qFromLittleEndian<quint64>(hexBytes016.mid(572, 8).data());
       fileData->FieldParsedValue.append({QString::number(overdraftCredit)});


       // Process Bytes 581-588: Consumed credit 51
       quint64 consumedCredit = qFromLittleEndian<quint64>(hexBytes016.mid(580, 8).data());
       fileData->FieldParsedValue.append({QString::number(consumedCredit)});


       // Process Bytes 589-876: Consumed credit monthly 52
       QVector<QString> consumedCreditMonthly;
       for (int i = 0; i < 12; ++i) {
           quint64 monthlyCredit = qFromLittleEndian<quint64>(hexBytes016.mid(588 + i * 24, 24).data());
           consumedCreditMonthly.append(QString::number(monthlyCredit));
       }
       fileData->FieldParsedValue.append(consumedCreditMonthly);


       // Process Bytes 877-884: Cumulative charge 53
       quint64 cumulativeCharge = qFromLittleEndian<quint64>(hexBytes016.mid(876, 8).data());
       fileData->FieldParsedValue.append({QString::number(cumulativeCharge)});


       // Process Bytes 885-888: Active Tariff ID 54
       quint32 activeTariffID = qFromLittleEndian<quint32>(hexBytes016.mid(884, 4).data());
       fileData->FieldParsedValue.append({QString::number(activeTariffID)});


       // Process Bytes 889-892: Active Tariff start date 55-57
       quint8 activeTariffYear = qFromLittleEndian<quint8>(hexBytes016.mid(888, 1).data());
       quint8 activeTariffMonth = qFromLittleEndian<quint8>(hexBytes016.mid(889, 1).data());
       quint16 activeTariffMin = qFromLittleEndian<quint16>(hexBytes016.mid(890, 2).data());

       fileData->FieldParsedValue.append({QString::number(activeTariffYear)});
       fileData->FieldParsedValue.append({QString::number(activeTariffMonth)});
       fileData->FieldParsedValue.append({QString::number(activeTariffMin)});


       // Process Bytes 893-896: Other Tariff ID 58
       quint32 otherTariffID = qFromLittleEndian<quint32>(hexBytes016.mid(892, 4).data());
       fileData->FieldParsedValue.append({QString::number(otherTariffID)});


       // Process Bytes 897-924: Last occurrence date 59
       std::array<int, 7> occurrenceOffsets = {896, 900, 904, 908, 912, 916, 920};
       QStringList lastOccurrenceDates;

       for (int offset : occurrenceOffsets) {
           quint8 year = qFromLittleEndian<quint8>(hexBytes016.mid(offset, 1).data());
           quint8 month = qFromLittleEndian<quint8>(hexBytes016.mid(offset + 1, 1).data());
           quint16 minutes = qFromLittleEndian<quint16>(hexBytes016.mid(offset + 2, 2).data());

           QString dateStr = QString("%1-%2-%3")
               .arg(year + 2000)
               .arg(month, 2, 10, QChar('0'))
               .arg(minutes);
           lastOccurrenceDates.append(dateStr);
       }
       fileData->FieldParsedValue.append(lastOccurrenceDates.toVector());

       // Process Bytes 925-952: Last clearance date 60
       std::array<int, 7> clearanceOffsets = {924, 928, 932, 936, 940, 944, 948};
       QStringList lastClearanceDates;

       for (int offset : clearanceOffsets) {
           quint8 year = qFromLittleEndian<quint8>(hexBytes016.mid(offset, 1).data());
           quint8 month = qFromLittleEndian<quint8>(hexBytes016.mid(offset + 1, 1).data());
           quint16 minutes = qFromLittleEndian<quint16>(hexBytes016.mid(offset + 2, 2).data());

           QString dateStr = QString("%1-%2-%3")
               .arg(year + 2000)
               .arg(month, 2, 10, QChar('0'))
               .arg(minutes);
           lastClearanceDates.append(dateStr);
       }
       fileData->FieldParsedValue.append(lastClearanceDates.toVector());

       // Process Bytes 953-958: Tamper count 61
       QVector<QString> tamperCounts;
       for (int i = 0; i < 6; ++i) {
           quint8 tamperCount = qFromLittleEndian<quint8>(hexBytes016.mid(952 + i, 1).data());
           tamperCounts.append(QString::number(tamperCount));
       }
       fileData->FieldParsedValue.append(tamperCounts);


       // Process Bytes 959-982: Last occurrence date 62
       std::array<int, 6> tamperOffsets = {958, 962, 966, 970, 974, 978};
       QStringList tamperDates;

       for (int offset : tamperOffsets) {
           quint8 year = qFromLittleEndian<quint8>(hexBytes016.mid(offset, 1).data());
           quint8 month = qFromLittleEndian<quint8>(hexBytes016.mid(offset + 1, 1).data());
           quint16 minutes = qFromLittleEndian<quint16>(hexBytes016.mid(offset + 2, 2).data());

           QString dateStr = QString("%1-%2-%3")
               .arg(year + 2000)
               .arg(month, 2, 10, QChar('0'))
               .arg(minutes);
           tamperDates.append(dateStr);
       }
       fileData->FieldParsedValue.append(tamperDates.toVector());

       // Process Bytes 983-999: Valve history 63
       QVector<QString> valveHistory;
       for (int i = 0; i < 17; ++i) {
           quint8 valveState = qFromLittleEndian<quint8>(hexBytes016.mid(982 + i, 1).data());
           valveHistory.append(QString::number(valveState));
       }
       fileData->FieldParsedValue.append(valveHistory);


       // Process Bytes 1000-1007: Current Month Reading 64
       quint64 currentMonthReading = qFromLittleEndian<quint64>(hexBytes016.mid(999, 8).data());
       fileData->FieldParsedValue.append({QString::number(currentMonthReading)});


       // Process Bytes 1008-1009: Error Code 65
       quint16 errorCode = qFromLittleEndian<quint16>(hexBytes016.mid(1007, 2).data());
       fileData->FieldParsedValue.append({QString::number(errorCode)});


       // Process Bytes 1010-1024: Not used 66
       QVector<QString> notUsed;
       for (int i = 0; i < 15; ++i) {
           quint8 unused = qFromLittleEndian<quint8>(hexBytes016.mid(1009 + i, 1).data());
           notUsed.append(QString::number(unused));
       }
       fileData->FieldParsedValue.append(notUsed);


       return fileData;
   }


/**
     * @brief Holds the parsed file data.
     */
QSharedPointer<FileData> WaterMeterParserModel::getFileData() const {
    return fileData; // Return the stored FileData object
}
