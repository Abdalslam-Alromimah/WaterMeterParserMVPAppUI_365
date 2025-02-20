/**
 * @file base_model.h
 * @brief Base class for all models in the application.
 *
 * The `BaseModel` class provides a base class for all models in the application. It inherits from `QObject`
 * and serves as the foundation for any model that needs to be part of the application.
 */

#ifndef BASE_MODEL_H
#define BASE_MODEL_H

#include <QObject>

/**
 * @class BaseModel
 * @brief Base class for all models in the Water Meter Parser application.
 *
 * The `BaseModel` class is a simple QObject-based class meant to be inherited by other model classes.
 * It doesn't implement any business logic itself but serves as a base for future models.
 */
class BaseModel : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructs a BaseModel object.
     *
     * This constructor initializes the `BaseModel` instance with an optional parent QObject.
     *
     * @param parent The parent object, default is nullptr.
     */
    explicit BaseModel(QObject* parent = nullptr) : QObject(parent) {}

    /**
     * @brief Destructor for the BaseModel class.
     *
     * The destructor is virtual and ensures proper cleanup of derived classes.
     */
    virtual ~BaseModel() = default;
};

#endif // BASE_MODEL_H

