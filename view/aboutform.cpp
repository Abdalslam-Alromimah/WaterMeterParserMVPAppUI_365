#include "aboutform.h"
#include "ui_aboutform.h"
#include <QtDebug>

#include "../presenter/watermeterparser_presenter.h"


AboutForm::AboutForm(QWidget* parent)
    : BaseView(parent)
    , ui(new Ui::AboutForm)
    , m_presenter(nullptr)


{

    ui->setupUi(this);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    connect(ui->closeButton, &QPushButton::clicked, this, &AboutForm::onCloseClicked);
}


AboutForm::~AboutForm() {
    delete ui;
}

void AboutForm::setPresenter(QSharedPointer<WaterMeterParserPresenter> presenter) {
    if (!m_presenter) {
        qDebug() << "Error: Presenter is null";
        return;
    }
    m_presenter = presenter;

    // Initialize viewSignals after presenter is set.
    viewSignals = QSharedPointer<WaterMeterParserViewSignals>::create();
}

/*void AboutForm::onCloseClicked() {
    if(m_presenter){
        emit closeFormRequested("AboutForm");
        m_presenter->handleFormClose("AboutForm");
    }else{
        qDebug() << "Error: Presenter is null";
    }

}*/
