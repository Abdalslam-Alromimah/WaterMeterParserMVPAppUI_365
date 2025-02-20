/**
 * @file watermeterparser_presenter.h
 * @brief Header of the WaterMeterParserPresenter class that handles the interaction between views and models in the Water Meter Parser application.
 *
 * This class acts as a mediator between the view and model layers, initializing and registering views and models,
 * handling user inputs, and processing data to update the user interface.
 */

#ifndef WATERMETERPARSER_PRESENTER_H
#define WATERMETERPARSER_PRESENTER_H

#include <QSharedPointer>
#include "../interfaces/basepresenter.h"
#include "../model/filedata.h"
#include "../view/watermeterparser_view.h"
#include "../view/loginform.h"
#include "../model/WaterMeterParser_Model.h"
#include <QtDebug>
#include <QEnableSharedFromThis>
#include "../interfaces/basepresenter.h"
#include "../model/databasemodel.h"
class AboutForm;

/**
 * @class WaterMeterParserPresenter
 * @brief The presenter that acts as a mediator between the view and model layers in the Water Meter Parser application.
 *
 * The `WaterMeterParserPresenter` class is responsible for managing the interactions between the application's views and models.
 * It handles user input requests, processes the data, and updates the user interface with the results. The presenter
 * also manages login functionality and controls the overall flow of data between views and models.
 */
class WaterMeterParserPresenter : public BasePresenter, public QEnableSharedFromThis<WaterMeterParserPresenter> {
    Q_OBJECT

public:
    /**
     * @brief Constructs the `WaterMeterParserPresenter`.
     *
     * This constructor initializes the presenter object and sets up necessary resources.
     *
     * @param parent The parent object for this presenter, default is `nullptr`.
     */
    explicit WaterMeterParserPresenter(QObject* parent = nullptr);

    /**
     * @brief Initializes the presenter with the specified database path.
     *
     * This method initializes the model and view with the given database path.
     *
     * @param path The path to the database.
     */
    void initialize(const QString& path);

    /**
     * @brief Handles login requests by validating user credentials.
     *
     * This method checks if the provided username and password are valid by querying the database.
     * It emits the `loginSuccess` or `loginFailure` signal based on the result of the validation.
     *
     * @param username The username entered by the user.
     * @param password The password entered by the user.
     */
    void handleLoginRequest(const QString& username, const QString& password);

    /**
     * @brief Handles parse requests to process the selected file's hexadecimal input.
     *
     * This method triggers the model to parse the input hexadecimal data for the selected file index.
     *
     * @param selectedFileIndex The index of the file to be parsed.
     * @param hexInput The hexadecimal input data to be processed.
     */
    void handleParseRequest(int selectedFileIndex, const QString& hexInput);

    /**
     * @brief Handles form-related requests.
     *
     * This method processes various form requests, such as opening or closing forms based on the specified `formType`.
     *
     * @param formType The type of the form to be handled.
     */
    void handleFormRequest(const QString& formType);

    /**
     * @brief Handles closing a form based on the `formType`.
     *
     * This method manages the closing of forms in the user interface based on the provided `formType`.
     *
     * @param formType The type of the form to be closed.
     */
    void handleFormClose(const QString& formType);

signals:
    /**
     * @brief Emitted when the login is successful.
     *
     * This signal is emitted when the user credentials have been successfully validated.
     */
    void loginSuccess();

    /**
     * @brief Emitted when the login fails.
     *
     * This signal is emitted when the login fails, along with a message describing the failure reason.
     *
     * @param message A message explaining the failure.
     */
    void loginFailure(const QString& message);

    /**
     * @brief Emitted when the parsing is complete.
     *
     * This signal is emitted once the parsing of the file is complete, providing the result and the selected file index.
     *
     * @param result A `QSharedPointer<FileData>` containing the parsed data.
     * @param index The index of the parsed file.
     */
    void parseComplete(QSharedPointer<FileData> result, int index);

public slots:
    /**
     * @brief Shows the "About" dialog.
     *
     * This slot is connected to the signal that triggers the display of the "About" dialog.
     */
    void handleShowAboutDialog();

private slots:
    /**
     * @brief Handles data changes in the model.
     *
     * This slot is triggered when the model's data changes, allowing the presenter to update the view accordingly.
     *
     * @param result A `QSharedPointer<FileData>` containing the updated file data.
     * @param selectedFileIndex The index of the file whose data has been updated.
     */
    void onModelDataChanged(QSharedPointer<FileData> result, int selectedFileIndex);

private:

    QSharedPointer<WaterMeterParserModel> parserModel;
    /** A map to hold views related to the "About" dialog. */

    //QSharedPointer<AboutForm> aboutView;
    QSharedPointer<AboutForm> aboutView;

   /** The database model used for login and user management. */
    QSharedPointer<DatabaseModel> parserModelDB;

    /** The main view of the application. */
    QSharedPointer<WaterMeterParserView> mainView;

    /** The LoginForm view of the application. */

    QSharedPointer<LoginForm> mainViewlogin ;
   // QMap<QString, QSharedPointer<BaseView>> mainViewlogin;


    /** The path to the database used for user authentication. */
    QString pathdb;
};

#endif // WATERMETERPARSER_PRESENTER_H
