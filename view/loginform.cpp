
/**
 * \file loginform.cpp
 * \brief Login point for the Water Meter Parser application.
 *
 * This file defines the behavior of the login screen in the Water Meter Parser application. It provides the
 * functionality for user authentication, handling user input for the username and password, and communicating
 * with the presenter to process the login logic.
 */
#include "loginform.h"
#include "ui_loginform.h"
#include <QtDebug>
#include <QMessageBox>
#include <QTimer>
#include <QSqlQuery>
#include <QSqlError>
#include<QSqlDriver>
#include "../model/dbmanager.h"
#include "../presenter/watermeterparser_presenter.h"
#include "../interfaces/baseview.h"

#include<QDialog>

static const QString path = "C:/Program Files/sqlite/ex1.db";



LoginForm::LoginForm(QWidget* parent)
    : BaseView(parent)
    , ui(new Ui::LoginForm)
{
 //   ui->setupUi(this);
   ui->setupUi(this);
   this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
   connect(ui->loginButton, &QPushButton::clicked, this, &LoginForm::onLoginClicked);
   connect(ui->closeButton, &QPushButton::clicked, this, &BaseView::onCloseClicked, Qt::DirectConnection);
}



LoginForm::~LoginForm() {
    delete ui;
}
void LoginForm::setPresenter(QSharedPointer<WaterMeterParserPresenter> m_presenter) {
    if (!m_presenter) {
        qDebug() << "Error: Presenter is null";
        return;
    }
   this-> presenter = m_presenter;

    // Here, we keep the presenter as a member variable, no need to set it as the parent
    viewSignals = QSharedPointer<WaterMeterParserViewSignals>::create();
}
void LoginForm::onLoginClicked() {
    QString username = ui->usernameInput->text();
    QString password = ui->passwordInput->text();

    if (viewSignals) {
        emit viewSignals->onLoginClicked(username, password);
    } else {
        qDebug() << "viewSignals is null!";
    }

   presenter->handleLoginRequest(username, password);

}


/*void LoginForm::onCloseClicked() {
    if(presenter){
        emit closeFormRequested("login");
        presenter->handleFormClose("login");
    }else{
        qDebug() << "Error: Presenter is null";
    }

}*/


