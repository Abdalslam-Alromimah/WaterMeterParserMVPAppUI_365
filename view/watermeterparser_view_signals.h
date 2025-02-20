/**
 * @file watermeterparser_view.h
 * @brief Header file for the WaterMeterParserView class, representing the main view in the Water Meter Parser application.
 *
 * This class handles the graphical user interface (GUI) for the Water Meter Parser. It is responsible for presenting
 * data to the user and interacting with the presenter to handle user inputs and application logic.
 */
#ifndef WATERMETERPARSER_VIEW_SIGNALS_H
#define WATERMETERPARSER_VIEW_SIGNALS_H
#include <QObject>

/**
 * @class WaterMeterParserView
 * @brief Represents the main view for the Water Meter Parser application.
 *
 * This class inherits from `BaseView` and provides the user interface for interacting with the Water Meter Parser.
 * It manages the presentation of data, including parsing results, login interactions, and file handling.
 * The view communicates with the presenter to handle user actions and update the interface accordingly.
 */
class WaterMeterParserViewSignals : public QObject {
    Q_OBJECT
signals:
    void parseRequested(int fileIndex, const QString& hexInput);
    void onLoginClicked(const QString& username, const QString& password);
    void onCloseClicked(const QString& formType);
    void showAboutDialogRequested();

};
#endif // WATERMETERPARSER_VIEW_SIGNALS_H
