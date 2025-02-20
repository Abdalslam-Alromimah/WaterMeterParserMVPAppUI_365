/**
 * @file aboutform.h
 * @brief Header of the AboutForm class which represents the "About" dialog in the Water Meter Parser application.
 *
 * This class is a view that displays information about the application. It interacts with the presenter to show the relevant
 * details and also supports the close action to dismiss the dialog.
 */

#ifndef ABOUTFORM_H
#define ABOUTFORM_H

#include "../interfaces/baseview.h"
//#include "../presenter/watermeterparser_presenter.h"
class WaterMeterParserPresenter;
class WaterMeterParserViewSignals;
namespace Ui {
class AboutForm;
}

/**
 * @class AboutForm
 * @brief Represents the "About" dialog in the Water Meter Parser application.
 *
 * The `AboutForm` class provides the user interface for displaying information about the application. It includes the logic to
 * interact with the presenter for closing the form and triggering actions from the view.
 *
 * This form is shown when the user requests to view information about the application.
 */
class AboutForm : public BaseView {
    Q_OBJECT

public:
    /**
     * @brief Constructs the `AboutForm` and initializes its UI.
     *
     * The constructor sets up the UI elements and prepares the dialog for display.
     *
     * @param parent The parent widget, default is `nullptr`.
     */
    explicit AboutForm(QWidget* parent = nullptr);  
    /**
     * @brief Sets the presenter for the `AboutForm`.
     *
     * This method binds the presenter to the view, allowing communication between them. The presenter handles the logic for
     * any interactions within this form.
     *
     * @param presenter The `WaterMeterParserPresenter` to be set.
     */
    void setPresenter(QSharedPointer<WaterMeterParserPresenter> presenter) ;

    /**
     * @brief Destructor for the `AboutForm`.
     *
     * Cleans up resources and ensures proper memory management when the form is destroyed.
     */
    ~AboutForm();



private:
    /** Pointer to the UI object associated with the AboutForm. */
    Ui::AboutForm* ui;

    /** The presenter that controls the logic for the AboutForm. */
    QSharedPointer<WaterMeterParserPresenter> m_presenter;

    /** Signals associated with the `WaterMeterParserView` (if any). */
    QSharedPointer<WaterMeterParserViewSignals> viewSignals;
};

#endif // ABOUTFORM_H
