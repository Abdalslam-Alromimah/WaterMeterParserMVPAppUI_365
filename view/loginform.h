/**
 * @file loginform.h
 * @brief Header file for the LoginForm class, which represents the login view in the Water Meter Parser application.
 *
 * This class is responsible for handling the user interface for login operations, including user authentication
 * and closing the login form. It communicates with the WaterMeterParserPresenter to manage login actions and
 * form interactions.
 */

#ifndef LOGINFORM_H
#define LOGINFORM_H

#include "../interfaces/baseview.h"
#include <QDialog>
//#include "../presenter/watermeterparser_presenter.h"
class WaterMeterParserPresenter;
class WaterMeterParserViewSignals;
namespace Ui {
    class LoginForm;
}

/**
 * @class LoginForm
 * @brief The LoginForm class is a view for user login in the Water Meter Parser application.
 *
 * This class inherits from BaseView and provides a user interface to allow users to enter their login credentials
 * (username and password). Upon successful authentication, it communicates with the presenter to proceed to the main
 * application view. The form can also be closed by the user.
 *
 * The LoginForm interacts with the presenter (WaterMeterParserPresenter) through signals and slots, allowing
 * it to trigger actions like login and form closing.
 */
class LoginForm : public BaseView {
    Q_OBJECT

public:
    /**
     * @brief Constructs a LoginForm object.
     * @param parent The parent widget (default is nullptr).
     */
    explicit LoginForm(QWidget* parent = nullptr);

    /**
     * @brief Destroys the LoginForm object.
     */
    ~LoginForm();

    /**
     * @brief Sets the presenter for the LoginForm.
     *
     * The presenter handles the logic behind the login process and form interactions.
     *
     * @param presenter A shared pointer to the WaterMeterParserPresenter object.
     */
    void setPresenter(QSharedPointer<WaterMeterParserPresenter> presenter);

    /**
     * @brief Provides the signal handler for the LoginForm.
     *
     * This function returns the shared pointer to the signals handler that listens for
     * events like login clicks or form closure.
     *
     * @return A shared pointer to the WaterMeterParserViewSignals object.
     */
    QSharedPointer<WaterMeterParserViewSignals> signalsHandler() {
        return viewSignals;
    }

signals:
    /**
     * @brief Signal emitted when the form is requested to be closed.
     *
     * The signal is emitted with the name of the form that triggered the request.
     *
     * @param formname The name of the form to close.
     */
   // void closeFormRequested(const QString& formname);

private slots:
    /**
     * @brief Slot that handles the login button click event.
     *
     * This slot is triggered when the user clicks the login button in the LoginForm.
     * It sends the entered credentials to the presenter for validation.
     */
    void onLoginClicked();

 private:
    Ui::LoginForm* ui; ///< The user interface object for the LoginForm.
    QSharedPointer<WaterMeterParserPresenter> presenter; ///< Shared pointer to the presenter responsible for handling login logic.
    QSharedPointer<WaterMeterParserViewSignals> viewSignals; ///< Shared pointer to the view signals handler.
};

#endif // LOGINFORM_H
