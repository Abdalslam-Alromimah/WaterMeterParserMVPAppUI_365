/**
 * @file base_presenter.h
 * @brief Base class for all presenters in the application.
 *
 * The `BasePresenter` class provides a base class for all presenters in the application. It manages
 * the interaction between models and views, allowing presenters to register models, views, and access them.
 */

#ifndef BASE_PRESENTER_H
#define BASE_PRESENTER_H

#include <QObject>
#include <QSharedPointer>
#include <QMap>
#include "basemodel.h"
//#include "../interfaces/baseview.h"
class BaseView;
/**
 * @class BasePresenter
 * @brief Base class for all presenters in the application.
 *
 * The `BasePresenter` class is designed to serve as a base for presenter classes in the application.
 * A presenter acts as an intermediary between models and views, handling interactions between them.
 * It provides a mechanism to register, retrieve, and manage models and views.
 */
class BasePresenter : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructs a BasePresenter object.
     *
     * This constructor initializes the `BasePresenter` instance with an optional parent QObject.
     *
     * @param parent The parent object, default is nullptr.
     */
    explicit BasePresenter(QObject* parent = nullptr) : QObject(parent) {}

    /**
     * @brief Virtual destructor for the BasePresenter class.
     *
     * The destructor ensures proper cleanup of derived classes.
     */
    virtual ~BasePresenter() = default;
    virtual void handleFormClose(const QString &formName) = 0; // Virtual function

    /**
     * @brief Pure virtual method to initialize the presenter.
     *
     * This method must be implemented by derived presenter classes. It is responsible for
     * initializing any required models, views, and connections.
     *
     * @param path The path (e.g., for database configuration or resource location) to initialize the presenter.
     */
    virtual void initialize(const QString& path) = 0;

public:
    /** A map holding models registered by key. */
    QMap<QString, QSharedPointer<BaseModel>> models;

    /** A map holding views registered by key. */
    QMap<QString, QSharedPointer<BaseView>> views;

    /**
     * @brief Registers a model with a specified key.
     *
     * This method stores a model instance in the `models` map, which can be later retrieved by the given key.
     *
     * @param key The key to identify the model.
     * @param model A shared pointer to the model to register.
     */
    template<typename T>
    void registerModel(const QString& key, QSharedPointer<T> model) {
        models[key] = qSharedPointerCast<BaseModel>(model);
    }

    /**
     * @brief Registers a view with a specified key.
     *
     * This method stores a view instance in the `views` map, which can be later retrieved by the given key.
     *
     * @param key The key to identify the view.
     * @param view A shared pointer to the view to register.
     */
    template<typename T>
    void registerView(const QString& key, QSharedPointer<T> view) {
        views[key] = qSharedPointerCast<BaseView>(view);
    }

    /**
     * @brief Retrieves a model by key.
     *
     * This method returns a shared pointer to the model registered under the specified key.
     *
     * @param key The key used to identify the model.
     * @return A shared pointer to the model.
     */
    template<typename T>
    QSharedPointer<T> getModel(const QString& key) {
        return qSharedPointerCast<T>(models[key]);
    }

    /**
     * @brief Retrieves a view by key.
     *
     * This method returns a shared pointer to the view registered under the specified key.
     *
     * @param key The key used to identify the view.
     * @return A shared pointer to the view.
     */
    template<typename T>
    QSharedPointer<T> getView(const QString& key) {
        return qSharedPointerCast<T>(views[key]);
    }
};

#endif // BASE_PRESENTER_H
