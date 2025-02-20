#include <QApplication>
#include "view/watermeterparser_view.h"
#include "presenter/watermeterparser_presenter.h"
#include "model/WaterMeterParser_Model.h"
#include <QSplashScreen>
#include <QTimer>
#include <QtDebug>
#include "view/loginform.h"
#include "config.h"
/**
 * \brief Main function to start the application.
 *
 * This function sets up the application components and starts the event loop.
 *
 * \param argc Number of command-line arguments.
 * \param argv Array of command-line arguments.
 * \return Exit status of the application.
 */
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    auto presenter = QSharedPointer<WaterMeterParserPresenter>::create();
    Config& config = Config::getInstance();

    static const QString path =config.getDatabasePath();

        // Create and show the splash screen
    QSharedPointer<QSplashScreen> splash = QSharedPointer<QSplashScreen>::create();
        splash->setPixmap(QPixmap(config.getSplashImagePath()));
        splash->show();
        Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
        splash->showMessage(QObject::tr("Progress..."), topRight, Qt::white);
        splash->showMessage(QObject::tr("Loading Parser modules..."), topRight, Qt::white);
        splash->showMessage(QObject::tr("Parser is connecting..."), topRight, Qt::white);


        QTimer::singleShot(1000, [=]() {



            presenter->initialize(path);
            auto mainViewlogin = presenter->getView<LoginForm>("login");
            mainViewlogin->setPresenter(presenter);
            if (mainViewlogin) {
                mainViewlogin->show();

            } else {
                qDebug() << "Error: Main view is null.";
            }
        });

        return app.exec();
    }
