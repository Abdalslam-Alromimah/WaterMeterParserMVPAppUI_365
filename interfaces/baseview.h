/**
 * @file base_view.h
 * @brief Base class for all views in the Water Meter Parser application.
 *
 * The `BaseView` class provides a base class for all views in the Water Meter Parser application. It defines methods
 * to show, close, and manage the visibility and title of windows.
 */

#ifndef BASE_VIEW_H
#define BASE_VIEW_H

#include <QWidget>
#include <QDialog>
#include <QtDebug>
#include "basepresenter.h"
/**
 * @class BaseView
 * @brief Base class for all views in the application.
 *
 * The `BaseView` class is meant to be inherited by all views in the application. It provides
 * basic functionality to show, close, and manage windows, as well as setting window titles.
 */
class BaseView : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructs a BaseView object.
     *
     * This constructor initializes the `BaseView` instance with an optional parent QWidget.
     *
     * @param parent The parent widget, default is nullptr.
     */
    explicit BaseView(QWidget* parent = nullptr) : QWidget(parent) {}

    /**
     * @brief Virtual destructor for the BaseView class.
     *
     * The destructor ensures proper cleanup of derived classes.
     */
    virtual ~BaseView() = default;

public slots:
    void onCloseClicked(){
       close();
        }
};

#endif // BASE_VIEW_H
