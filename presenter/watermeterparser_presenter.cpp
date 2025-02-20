/**
 * @file watermeterparser_presenter.cpp
 * @brief Implementation of the WaterMeterParserPresenter class that handles the interaction between views and models in the Water Meter Parser application.
 *
 * This class acts as a mediator between the view and model layers, initializing and registering views and models,
 * handling user inputs, and processing data to update the user interface.
 */

#include "watermeterparser_presenter.h"
#include "../view/watermeterparser_view.h"
#include "../view/loginform.h"
#include "../model/WaterMeterParser_Model.h"
#include "../model/databasemodel.h"
#include "../view/aboutform.h"
#include <QSharedPointer>

/**
 * @class WaterMeterParserPresenter
 * @brief The presenter class for handling the application's logic and coordinating with models and views.
 *
 * The presenter handles user interactions, processes data from models, and updates the views accordingly. It interacts
 * with various components like `WaterMeterParserModel`, `DatabaseModel`, and views such as `WaterMeterParserView`,
 * `LoginForm`, and `AboutForm`.
 */
WaterMeterParserPresenter::WaterMeterParserPresenter(QObject* parent)
    : BasePresenter(parent) {
    // Constructor implementation
}

/**
 * @brief Initializes the presenter, sets up models, and registers views.
 *
 * This method is responsible for initializing the models and views required for the application, including the database model,
 * water meter parser model, login view, and main view. It also connects the appropriate signals to their respective slots.
 *
 * @param path The path to the database used by the application.
 */
void WaterMeterParserPresenter::initialize(const QString& path) {
    pathdb = path;
    parserModelDB = QSharedPointer<DatabaseModel>::create(pathdb);
    registerModel("database", parserModelDB);

     parserModel = QSharedPointer<WaterMeterParserModel>::create();
    if (!parserModel) {
        qDebug() << "Error: Failed to create Parser Model.";
        return;
    }
    registerModel("parser", parserModel);
    parserModel = getModel<WaterMeterParserModel>("parser");
    if (!parserModel) {
        qDebug() << "Error: Water Meter Parser model not found or registered.";
        return;
    }

    connect(parserModel.data(), &WaterMeterParserModel::parsingComplete,
            this, &WaterMeterParserPresenter::onModelDataChanged);

     mainViewlogin = QSharedPointer<LoginForm>::create();
    if (!mainViewlogin) {
        qDebug() << "Error: Failed to create main view.";
        return;
    }

    registerView("login", mainViewlogin);
    mainViewlogin = getView<LoginForm>("login");
    if (!mainViewlogin) {
        qDebug() << "Error: Login view model not found or registered.";
        return;
    }
    connect(mainViewlogin->signalsHandler().data(), &WaterMeterParserViewSignals::onLoginClicked,
            this, &WaterMeterParserPresenter::handleLoginRequest);

    connect(mainViewlogin->signalsHandler().data(), &WaterMeterParserViewSignals::onCloseClicked,
            this, &WaterMeterParserPresenter::handleFormClose);

    mainView = QSharedPointer<WaterMeterParserView>::create();
    registerView("main", mainView);
}

/**
 * @brief Handles login requests and validates user credentials.
 *
 * This method is called when the user attempts to log in. It validates the provided credentials against the database
 * and opens the main view if the login is successful. If the login fails, it emits a `loginFailure` signal.
 *
 * @param username The username provided by the user.
 * @param password The password provided by the user.
 */
void WaterMeterParserPresenter::handleLoginRequest(const QString& username, const QString& password) {
    if (!parserModelDB) {
        qDebug() << "Error: Failed to create DB Model.";
        return;
    }
    parserModelDB = getModel<DatabaseModel>("database");
    if (!parserModelDB) {
        qDebug() << "Error: Database model not found or registered.";
        return;
    }

    if (parserModelDB->validateCredentials(pathdb, username, password)) {
        if (!mainView) {
            qDebug() << "Error: Failed to create main view.";
            return;
        }

        mainView = getView<WaterMeterParserView>("main");
        if (!mainView) {
            qDebug() << "Error: WatermeterView model not found or registered.";
            return;
        }

        connect(mainView->signalsHandler().data(),
                &WaterMeterParserViewSignals::parseRequested,
                this, &WaterMeterParserPresenter::handleParseRequest);

        connect(mainView.data(),
                &WaterMeterParserView::showAboutDialogRequested,
                this, &WaterMeterParserPresenter::handleShowAboutDialog);

        if (mainView) {
            mainView->show();
            auto loginView = getView<LoginForm>("login");
            if (loginView) {
                loginView->close();
            } else {
                qDebug() << "Error: Main view is null.";
            }
        } else {
            qDebug() << "Error: Main view is null.";
        }
    } else {
        emit loginFailure("Invalid credentials");
    }
}

/**
 * @brief Handles the display of the About dialog.
 *
 * This method creates an `AboutForm` view and registers it. It shows the About dialog when requested by the user.
 */
void WaterMeterParserPresenter::handleShowAboutDialog() {
    qDebug() << "handleShowAboutDialog.";

     aboutView = QSharedPointer<AboutForm>::create();
    registerView("about", aboutView);
    aboutView->setPresenter(sharedFromThis());
    aboutView->show();
}

/**
 * @brief Handles the parse request when the user selects a file to parse.
 *
 * This method is invoked when the user requests to parse a file. It forwards the request to the `WaterMeterParserModel`
 * to perform the parsing and handle the result.
 *
 * @param selectedFileIndex The index of the selected file to parse.
 * @param hexInput The hexadecimal input string that is to be parsed.
 */
void WaterMeterParserPresenter::handleParseRequest(int selectedFileIndex, const QString& hexInput) {
    qDebug() << "Handling parse request for file index:" << selectedFileIndex;
     parserModel = getModel<WaterMeterParserModel>("parser");
    if (parserModel) {
        parserModel->parseFile(selectedFileIndex, hexInput);
    } else {
        qDebug() << "Error: Parser model is null.";
    }
}

/**
 * @brief Updates the view when the model data has changed.
 *
 * This method is invoked when the model's data is updated (e.g., after parsing). It updates the main view with the
 * new data by calling the `displayResult` method of the `WaterMeterParserView`.
 *
 * @param result The parsed result data.
 * @param selectedFileIndex The index of the file that was parsed.
 */
void WaterMeterParserPresenter::onModelDataChanged(QSharedPointer<FileData> result, int selectedFileIndex) {
    qDebug() << "Model data changed for file index:" << selectedFileIndex;
     mainView = getView<WaterMeterParserView>("main");

    if (mainView) {
        mainView->displayResult(result, selectedFileIndex);
    } else {
        qDebug() << "Error: Main view is null.";
    }
}

/**
 * @brief Handles the closing of a form (login or about form).
 *
 * This method is called when the user requests to close a form. It checks the form type and closes the corresponding view.
 *
 * @param formType The type of form that is being closed. It can be either "login" or "AboutForm".
 */
void WaterMeterParserPresenter::handleFormClose(const QString& formType) {
    if (formType == "login") {
        qDebug() << "Close login.";
        auto view = getView<LoginForm>("login");
        view->close();
    }

    if (formType == "AboutForm") {
         aboutView = getView<AboutForm>("AboutForm");
        aboutView->close();
    }
}
