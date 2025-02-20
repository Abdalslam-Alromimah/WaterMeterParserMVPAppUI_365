#ifndef WATERMETERPARSER_VIEW_H
#define WATERMETERPARSER_VIEW_H
/**
 * @file watermeterparser_view.h
 * @brief Header file for the WaterMeterParserView class, representing the main view in the Water Meter Parser application.
 *
 * This class handles the graphical user interface (GUI) for the Water Meter Parser. It is responsible for presenting
 * data to the user and interacting with the presenter to handle user inputs and application logic.
 */
#include <QMessageBox>
#include <QDateTime>
#include <QSharedPointer>
#include "../interfaces/baseview.h"
#include "watermeterparser_view_signals.h"

namespace Ui {
class WaterMeterParserView;
}

class WaterMeterParserPresenter;
class FileData;
/**
 * @class WaterMeterParserView
 * @brief Represents the main view for the Water Meter Parser application.
 *
 * This class inherits from `BaseView` and provides the user interface for interacting with the Water Meter Parser.
 * It manages the presentation of data, including parsing results, login interactions, and file handling.
 * The view communicates with the presenter to handle user actions and update the interface accordingly.
 */
class WaterMeterParserView : public BaseView {
    Q_OBJECT

public:
    /**
     * @brief Constructs a WaterMeterParserView object.
     *
     * Initializes the view with the given parent widget.
     *
     * @param parent The parent widget for the view (default is nullptr).
     */
    explicit WaterMeterParserView(QWidget* parent = nullptr);

    /**
     * @brief Destroys the WaterMeterParserView object.
     *
     * Cleans up any allocated resources when the view is destroyed.
     */
    ~WaterMeterParserView();

    /**
     * @brief Sets the window state of the view.
     *
     * Allows the window state (e.g., maximized, minimized) to be set for the view.
     *
     * @param state The desired window state.
     */
    void setWindowState(Qt::WindowState state);

    /**
     * @brief Shows the view.
     *
     * Displays the view to the user, making it visible on the screen.
     */
    void show();

    /**
     * @brief Sets the presenter for the view.
     *
     * The presenter manages the interaction between the view and model.
     * It is responsible for controlling the logic behind user actions.
     *
     * @param presenterx A shared pointer to the WaterMeterParserPresenter that will handle the logic for this view.
     */
void setPresenter(QSharedPointer<WaterMeterParserPresenter> presenterx);

/**
        * @brief Displays the parsed data in the view.
        *
        * This method is responsible for displaying the results of parsing a file's data.
        *
        * @param fileData A shared pointer to the `FileData` object containing parsed data.
        * @param selectedFileIndex The index of the file from which the data was parsed.
        */
    void displayResult(QSharedPointer<FileData> fileData, int selectedFileIndex);
    void showError(const QString& message) {
        QMessageBox::critical(this, "Error", message);
    }

    QSharedPointer<WaterMeterParserViewSignals> signalsHandler() {
        return viewSignals;
    }

    void showAboutDialog();
    void applyStylesheet(const QString &filePath);
    QString formatDateTime(const QString& hexDate);
    QString getHexFromDevice();
    QString getHexFromFile();
    int getCurrentFileIndex() const;
    QString getHexInput() const;
    void initializeUI();
    QDateTime Mintues_Days_Hours_Mintues(int year, int month, int totalMintues);

private slots:
    void onParseButtonClicked();
    void handleIndexChange(int index);
    void onActionWhiteTemplate();
    void onActionDarkTepmlate();
    void onAboutActionTriggered();
    void onActionfromDeviceAction();
    void onActionfromFileAction();
    void onActionmanuallyAction();
signals:
    void showAboutDialogRequested();
    void closeFormRequested();
private:
    /**
            * @brief to initial interface components for File 001.
            */
       void displayFile001Dataini();
       /**
            * @brief to initial interface components for File 002.
            */
       void displayFile002Dataini();
       /**
            * @brief to initial interface components for File 003.
            */
       void displayFile003Dataini();
       /**
            * @brief to initial interface components for File 004.
            */
       void displayFile004Dataini();
       /**
            * @brief to initial interface components for File 005.
            */
       void displayFile005Dataini();
       /**
            * @brief to initial interface components for File 006.
            */
       void displayFile006Dataini();
       /**
            * @brief to initial interface components for File 007.
            */
       void displayFile007Dataini();
       /**
            * @brief to initial interface components for File 0010.
            */
       void displayFile0010Dataini();
       /**
            * @brief to initial interface components for File 0011.
            */
       void displayFile0011Dataini();
       /**
            * @brief to initial interface components for File 0012.
            */
       void displayFile0012Dataini();
       /**
            * @brief to initial interface components for File 0013.
            */
       void displayFile0013Dataini();
       /**
            * @brief to initial interface components for File 0014.
            */
       void displayFile0014Dataini();
       /**
            * @brief to initial interface components for File 0015.
            */
       void displayFile0015Dataini();
       /**
            * @brief to initial interface components for File 0016.
            */
       void displayFile0016Dataini();


    //display functions
       /**
            * @brief Displays parsed data for (Client Card- Inputs (From backend to meter)- Constant Main Information 1) according to  File 001.
            *
            * @param fileData A QSharedPointer to the parsed FileData object.
            */
       void displayFile001Data(QSharedPointer<FileData> fileData) ;
       /**
            * @brief Displays parsed data for (Client Card- Inputs (From backend to meter)- Constant Main Information 2)  according to File 002.
            *
            * @param fileData A QSharedPointer to the parsed FileData object.
            */
       void displayFile002Data(QSharedPointer<FileData> fileData) ;
       /**
            * @brief Displays parsed data for Client Card- Inputs (From backend to meter)- Changed Main Data)  according to File 003.
            *
            * @param fileData A QSharedPointer to the parsed FileData object.
            */
       void displayFile003Data(QSharedPointer<FileData> fileData) ;
       /**
            * @brief Displays parsed data for (Client Card- Inputs (From backend to meter)- Charge and Deduction Data)  according to File 004.
            *
            * @param fileData A QSharedPointer to the parsed FileData object.
            */
       void displayFile004Data(QSharedPointer<FileData> fileData) ;
       /**
            * @brief Displays parsed data for (Client Card- Inputs (From backend to meter)- (Client Data + Tariff Data + Friendly Hours))  according to File 005.
            *
            * @param fileData A QSharedPointer to the parsed FileData object.
            */
       void displayFile005Data(QSharedPointer<FileData> fileData) ;
       /**
            * @brief Displays parsed data for (Maintenance Card -Input (From backend to meter) - (Changed Main Data))  according to File 006.
            *
            * @param fileData A QSharedPointer to the parsed FileData object.
            */
       void displayFile006Data(QSharedPointer<FileData> fileData) ;
       /**
            * @brief Displays parsed data for (Retrieval Card -Input (From backend to meter)-7 (Changed Main Data))  according to File 007.
            *
            * @param fileData A QSharedPointer to the parsed FileData object.
            */
       void displayFile007Data(QSharedPointer<FileData> fileData) ;
       /**
            * @brief Displays parsed data for (Client Card-(Output (From meter to backend)-(Changed Main Data))  according to File 0010.
            *
            * @param fileData A QSharedPointer to the parsed FileData object.
            */
       void displayFile0010Data(QSharedPointer<FileData> fileData) ;
       /**
            * @brief Displays parsed data for (Client Card-(Output (From meter to backend)- (Charge + Deduction + Readings)) according to File 0011.
            *
            * @param fileData A QSharedPointer to the parsed FileData object.
            */
       void displayFile0011Data(QSharedPointer<FileData> fileData) ;
       /**
            * @brief Displays parsed data for (Client Card-(Output (From meter to backend)- (Client Data + Tariff + Friendly Hours)  according to File 0012.
            *
            * @param fileData A QSharedPointer to the parsed FileData object.
            */
       void displayFile0012Data(QSharedPointer<FileData> fileData) ;
       /**
            * @brief Displays parsed data for (Client Card-(Output (From meter to backend)- (Credit Info))  according to File 0013.
            *
            * @param fileData A QSharedPointer to the parsed FileData object.
            */
       void displayFile0013Data(QSharedPointer<FileData> fileData) ;
       /**
            * @brief Displays parsed data for (Client Card-(Output (From meter to backend)- (Alarm Info + Tamper Info)) according to File 0014.
            *
            * @param fileData A QSharedPointer to the parsed FileData object.
            */
       void displayFile0014Data(QSharedPointer<FileData> fileData) ;
       /**
            * @brief Displays parsed data for (Configuration Card- Output (From meter to backend)-  (Meter Report) (Multi Objects))  according to File 0015.
            *
            * @param fileData A QSharedPointer to the parsed FileData object.
            */
       void displayFile0015Data(QSharedPointer<FileData> fileData) ;
       /**
            * @brief Displays parsed data for (Retrieval Card -Output (From meter to backend)- 6 (Retrieve Report File 1) (Multi Objects)) according to File 0016.
            *
            * @param fileData A QSharedPointer to the parsed FileData object.
            */
       void displayFile0016Data(QSharedPointer<FileData> fileData) ;


    QSharedPointer<WaterMeterParserPresenter> m_presenter;
    Ui::WaterMeterParserView* ui;
    QSharedPointer<WaterMeterParserViewSignals> viewSignals;
};

#endif // WATERMETERPARSER_VIEW_H
