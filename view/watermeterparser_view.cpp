#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QTextBrowser>
#include <QPushButton>
#include <QMessageBox>
#include <QStackedWidget>
#include <QFileDialog>
#include <QtMath>
#include <QString>
#include <QDebug>
#include <QMenuBar>
#include <QAction>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>

#include "watermeterparser_view.h"
#include "aboutform.h"
#include "./ui_watermeterparser_view.h"
#include "../interfaces/basepresenter.h"
#include "../interfaces/baseview.h"
#include "../presenter/watermeterparser_presenter.h"
#include "config.h"
Config& config = Config::getInstance();

WaterMeterParserView::WaterMeterParserView(QWidget* parent)
    : BaseView(parent)
    , ui(new Ui::WaterMeterParserView)
 //   , presenter(nullptr)
    , viewSignals(new WaterMeterParserViewSignals)
{
    if (ui) {
        ui->setupUi(this);
    } else {
        qDebug() << "UI not initialized";
        return;
    }
QMenuBar *menuBar= new QMenuBar(this);
menuBar->setLayoutDirection( Qt::RightToLeft);
QMenu *AboutTemplate= menuBar->addMenu("Help");
        if(AboutTemplate){
            QAction *StylesheetAboutTemplate=AboutTemplate->addAction("About");
            connect(StylesheetAboutTemplate, &QAction::triggered, this, &WaterMeterParserView::onAboutActionTriggered);

        }
        // Create the Help action and connect it to the showPage function
            QAction *helpAction = AboutTemplate->addAction(tr("&Help Contents"));
           // connect(helpAction, &QAction::triggered, this, &WaterMeterParserView::help);


QMenu *stylesheetTemplate= menuBar->addMenu("Theme");

        if(stylesheetTemplate){
            QAction *StylesheetDarkTemplate=stylesheetTemplate->addAction("Dark");
            connect(StylesheetDarkTemplate, &QAction::triggered,this, &WaterMeterParserView::onActionDarkTepmlate);
            QAction *StylesheetWhiteTemplate=stylesheetTemplate->addAction("White");
            connect(StylesheetWhiteTemplate, &QAction::triggered,this, &WaterMeterParserView::onActionWhiteTemplate);
          }
// Connect the "About" action to the slot
    if(ui->parserButton){

        connect(ui->parserButton,&QPushButton::clicked,
               this,&WaterMeterParserView::onParseButtonClicked);
    }
    else{
        qDebug()<<"Error :parserButton is not initialized";
    }
    // Connect comboBox index change to a custom handler
    connect(ui->fileSelector,SIGNAL(currentIndexChanged(int)),
            this, SLOT(handleIndexChange(int)));


    QMenu *setHexInputMenu= menuBar->addMenu("Set Hex Input");
    QAction *fromDeviceAction=setHexInputMenu->addAction("From Device");
    QAction *fromFileAction=setHexInputMenu->addAction("From File");
    QAction *manuallyAction=setHexInputMenu->addAction("Manually");
    connect(fromDeviceAction, &QAction::triggered,this, &WaterMeterParserView::onActionfromDeviceAction);
    connect(fromFileAction, &QAction::triggered,this, &WaterMeterParserView::onActionfromFileAction);
    connect(manuallyAction, &QAction::triggered,this, &WaterMeterParserView::onActionmanuallyAction);


    initializeUI();
    onActionWhiteTemplate();



}
/*
void WaterMeterParserView::help()
{
    HelpBrowser::showPage("index.html");  // Show the main help page (index.html)
}
*/
WaterMeterParserView::~WaterMeterParserView() {
    delete ui;
}

void WaterMeterParserView::setPresenter(QSharedPointer<WaterMeterParserPresenter> presenter) {
    if(!presenter){
        qDebug()<<"Error: Presenter is null";
        return;

    }
    this->m_presenter = presenter;
}

void WaterMeterParserView::setWindowState(Qt::WindowState state) {
    QWidget ::setWindowState(state);
    }

void WaterMeterParserView::showAboutDialog() {

    if (m_presenter) {
m_presenter->handleShowAboutDialog();

    } else {
        qDebug() << "Presenter is null.";
    }
}
void WaterMeterParserView::onAboutActionTriggered() {
    emit showAboutDialogRequested();

}

void WaterMeterParserView::onActionfromDeviceAction(){
    QString hexInput=getHexFromDevice();
    ui->hexInput->setText(hexInput);
}

void WaterMeterParserView::onActionfromFileAction(){
    QString hexInput=getHexFromFile();
    ui->hexInput->setText(hexInput);
}

void WaterMeterParserView::onActionmanuallyAction(){
    ui->hexInput->setFocus();
}

void WaterMeterParserView::onActionDarkTepmlate(){
    QString exePath = QCoreApplication::applicationDirPath();
    applyStylesheet(config.getDarkStylesheetPath());

}
void WaterMeterParserView::onActionWhiteTemplate(){
    QString exePath = QCoreApplication::applicationDirPath();
    applyStylesheet(config.getLightStylesheetPath());


}


void WaterMeterParserView::applyStylesheet(const QString &filePath)
{
    QFile styleFile(filePath);
    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&styleFile);
        QString styleSheet = stream.readAll();
        if (!styleSheet.isEmpty()) {
            qApp->setStyleSheet(styleSheet);
            qDebug() << "Stylesheet loaded successfully.";
        } else {
            qDebug() << "Stylesheet is empty.";
        }
        styleFile.close();
    } else {
        qDebug() << "Failed to open stylesheet file:" << styleFile.errorString();
    }
}



void WaterMeterParserView::show(){
    QWidget::show();
}

void WaterMeterParserView::handleIndexChange(int index) {

    if (index >= 0 && index <ui->stackedWidgetMain->count()) {
        ui->stackedWidgetMain->setCurrentIndex(index+1);
    } else {
        qDebug() << "Invalid index:" << index;
    }

    int fileType=1;
        int selectedIndex = ui->fileSelector->currentIndex();
        if (selectedIndex >= 0) {
            fileType = selectedIndex + 1;

        } else {
            fileType = selectedIndex;

        }

        if (fileType < 1 || fileType > 15) {
                qWarning() << "Invalid fileType:" << fileType;
                fileType = selectedIndex;
            }



        switch (fileType) {
            case 1: // File 0001
                 displayFile001Dataini();
                break;

        }

}

void WaterMeterParserView::onParseButtonClicked() {//Note
    //  emit the signal from the interface
    viewSignals->emit parseRequested(getCurrentFileIndex(), getHexInput());
}

int WaterMeterParserView::getCurrentFileIndex() const {
        // Always return a positive value (index starts from 1)
        return ui->fileSelector->currentIndex() + 1;


}

QString WaterMeterParserView::getHexInput() const {
    return ui->hexInput->text();
}

QString WaterMeterParserView::formatDateTime(const QString& hexDate) {
    // format: "0xYY-0xMM-0xMMMM" where MMMM is minutes
    QStringList parts = hexDate.split("-");
    if (parts.size() != 3) return "Invalid Date";

    // Convert year
    bool yearOk;
    int year = parts[0].mid(2).toInt(&yearOk, 16);
    if (!yearOk) return "Invalid Year";
    year += 2000;

    // Convert month
    bool monthOk;
    int month = parts[1].mid(2).toInt(&monthOk, 16);
    if (!monthOk || month < 1 || month > 12) return "Invalid Month";

    // Convert minutes to hours
    bool minOk;
    int minutes = parts[2].mid(2).toInt(&minOk, 16);
    if (!minOk) return "Invalid Time";
    int hours = minutes / 60;

    // Month names
    QStringList monthNames = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };

    return QString("%1-%2-%3")
            .arg(year)
            .arg(monthNames[month - 1])
            .arg(hours);
}

QDateTime WaterMeterParserView::Mintues_Days_Hours_Mintues(int year, int month,int totalMintues ){

    if(year<0 || month>12 || totalMintues<0){
        qDebug()<<"Invalid date or time components";
        return QDateTime() ;
    }
     int dayx=(totalMintues/(24*60));
     int hoursx = (totalMintues / 60) % 24;
     int minutesx = totalMintues % 60;
     return QDateTime(QDate(year, month, dayx), QTime(hoursx, minutesx));
}

QString WaterMeterParserView::getHexFromDevice(){
    QString hexData="00 01";
    return hexData;
}


QString WaterMeterParserView::getHexFromFile(){
    QString filePath= QFileDialog::getOpenFileName(this, tr("OPen Hex File"),"",tr("Hex Files(*.hex);;All Files(*)"));
    if(filePath.isEmpty()){
        qDebug()<<"No file selected.";
        return "";
    }
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug()<<"Failed to open file"<<filePath;
        return "";
    }
        QTextStream in(&file);
        QString hexData= in.readAll().trimmed();
        file.close();

        return hexData;
}

void WaterMeterParserView::initializeUI() {
    setWindowTitle("Water Meter: Hex Files Parser (MVP)");
    resize(1200, 800);
        ui->fileSelector->addItem("File 0001 (Constant Main Information 1)");
        ui->fileSelector->addItem("File 0002 (Constant Main Information 2)");
        ui->fileSelector->addItem("File 0003 (Changed Main Data)");
        ui->fileSelector->addItem("File 0004 (Charge and Deduction Data)");
        ui->fileSelector->addItem("File 0005 (Client Data + Tariff Data + Friendly Hours)");
        ui->fileSelector->addItem("File 0006 (Changed Main Data)");
        ui->fileSelector->addItem("File 0007 (Changed Main Data)");
        ui->fileSelector->addItem("File 0010 (Changed Main Data)");
        ui->fileSelector->addItem("File 0011 (Charge + Deduction + Readings)");
        ui->fileSelector->addItem("File 0012 (Client Data + Tariff + Friendly Hours)");
        ui->fileSelector->addItem("File 0013 (Credit Info)");
        ui->fileSelector->addItem("File 0014 (Alarm Info + Tamper Info)");
        ui->fileSelector->addItem("File 0015 (Meter Report) (Multi Objects)");
        ui->fileSelector->addItem("File 0016 ((Retrieve Report File 1) (Multi Objects)");



   setWindowState(Qt::WindowState::WindowMaximized);
    if(ui->fileSelector->count()>0){
        ui->fileSelector->setCurrentIndex(0);
        ui->stackedWidgetMain->setCurrentIndex(1);

     }
    else {
        qDebug()<<"Error: fileSelector has no items";
    }


    // Initialize output Functions
       displayFile001Dataini();
       displayFile002Dataini();
       displayFile003Dataini();
       displayFile004Dataini();
       displayFile005Dataini();
       displayFile006Dataini();
       displayFile007Dataini();
       displayFile0010Dataini();
       displayFile0011Dataini();
       displayFile0012Dataini();
       displayFile0013Dataini();
       displayFile0014Dataini();
       displayFile0015Dataini();
       displayFile0016Dataini();


  //  ui->scrollArea::setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff) ;
}


/**
         * @brief Implemnation function to initial interface components for File 001.
     */
void WaterMeterParserView::displayFile001Dataini() {
    ui->T_CardFunction->setText("Unknown Card Function");
    ui->T_UtilityID->setText("");
}
/**
         * @brief Implemnation function to initial interface components for File 002.
     */
void WaterMeterParserView::displayFile002Dataini() {
    ui->T_MeterSerial->setText("");
    ui->T_MeterInitialReading->setText("");
    ui->T_DiameterID->setText("");
    ui->T_ProductionYear->setText("");
    ui->T_MeterSupplierID->setText("");
    ui->T_SubscriptionNumber->setText("");
}
/**
         * @brief Implemnation function to initial interface components for File 003.
     */
void WaterMeterParserView::displayFile003Dataini() {
    ui->dateTime_Lastservertransactiondate->setDateTime(QDateTime::currentDateTime());
    ui->dateTime_Lastservertransactiondate->setDisplayFormat("yyyy-MM-dd hh:mm:ss AP");

    ui->T_CardStatus->setText("Inactive");

    ui->MeterActions_chBox1->setChecked(false);
    ui->MeterActions_chBox2->setChecked(false);
    ui->MeterActions_chBox3->setChecked(false);
    ui->MeterActions_chBox4->setChecked(false);
    ui->MeterActions_chBox5->setChecked(false);
    ui->MeterActions_chBox6->setChecked(false);
    ui->MeterActions_chBox7->setChecked(false);
    ui->MeterActions_chBox8->setChecked(false);

    ui->T_ValvemaintenancecycleDayofMonth->setText("");

    ui->ValvemaintenancecycleMIN_chBox_AM->setChecked(false);
    ui->ValvemaintenancecycleMIN_chBox_PM->setChecked(false);
    ui->T_ValvemaintenancecycleMIN->setText("");
}
/**
         * @brief Implemnation function to initial interface components for File 004.
     */
void WaterMeterParserView::displayFile004Dataini() {
    ui->T_ChargeID->setText("");
    ui->T_ChargeAmount->setText("");
    ui->T_ReplacementChargeID->setText("");
    ui->T_ReplacementChargeAmount->setText("");
    ui->T_LowCreditLimitValue->setText("");
    ui->T_Lowcreditpercentagevalue->setText("");
    ui->T_Lowcreditlimitoption->setText("");
    ui->T_DeductionID->setText("");
    ui->T_Numberofcycles->setText("");
    ui->T_Deductedvalue->setText("");
    ui->dat_DeductionstartcycleDate->setDateTime(QDateTime::currentDateTime());
    ui->dat_DeductionstartcycleDate->setDisplayFormat("yyyy-MM-dd hh:mm:ss AP");
}
/**
         * @brief Implemnation function to initial interface components for File 005.
     */
void WaterMeterParserView::displayFile005Dataini() {
    ui->T_Service_point_category_id_5->setText("");
    ui->T_Service_point_activity_id_5->setText("");

    ui->radioB_Service_cut_flag_Pre_Paid_5->setChecked(false);
    ui->radioB_Service_cut_flag_Post_Paid_5->setChecked(false);

    ui->T_Number_of_Units_5->setText("");

    ui->radioB_Sewage_service_flag_service_covered_5->setChecked(false);
    ui->radioB_Sewage_service_flag_service_not_covered_5->setChecked(false);

    ui->T_TariffID_5->setText("");
    ui->T_Month_Fixed_Fees_5->setText("");
    ui->T_Month_Fixed_Fees_Per_Unit_5->setText("");

    ui->table_Month_Fixed_Step_Fees_5->setRowCount(1);
    ui->table_Month_Fixed_Step_Fees_5->setColumnCount(7);
    ui->table_Month_Fixed_Step_Fees_5->setHorizontalHeaderLabels(
        QStringList() << "1" << "2" << "3" << "4" << "5" << "6" << "7" <<"8" <<"9"  <<"10" <<"11" <<"12" <<"13" <<"14" << "15" <<"16");
    ui->table_Month_Fixed_Step_Fees_5->clearContents();

    ui->T_Month_Fixed_Fees_per_cubic_meter_5->setText("");

    ui->Month_Fixed_Fees_Chbox_5->setChecked(false);
    ui->Month_Fixed_Fees_per_Unit_ChBox_5->setChecked(false);
    ui->Month_Fixed_Steps_ChBox_5->setChecked(false);
    ui->Month_Fixed_Fees_per_Cubic_Meter_ChBox_5->setChecked(false);

    ui->Tier_chBox1_5->setChecked(false);
    ui->Tier_chBox2_5->setChecked(false);
    ui->Tier_chBox3_5->setChecked(false);
    ui->Tier_chBox4_5->setChecked(false);
    ui->Tier_chBox5_5->setChecked(false);
    ui->Tier_chBox6_5->setChecked(false);
    ui->Tier_chBox7_5->setChecked(false);
    ui->Tier_chBox8_5->setChecked(false);
    ui->Tier_chBox9_5->setChecked(false);
    ui->Tier_chBox10_5->setChecked(false);
    ui->Tier_chBox11_5->setChecked(false);
    ui->Tier_chBox12_5->setChecked(false);
    ui->Tier_chBox13_5->setChecked(false);
    ui->Tier_chBox14_5->setChecked(false);
    ui->Tier_chBox15_5->setChecked(false);
    ui->Tier_chBox16_5->setChecked(false);

    ui->table_Consumption_Price_5->setRowCount(1);
    ui->table_Consumption_Price_5->setColumnCount(7);
    ui->table_Consumption_Price_5->setHorizontalHeaderLabels(
                QStringList() << "1" << "2" << "3" << "4" << "5" << "6" << "7" <<"8" <<"9"  <<"10" <<"11" <<"12" <<"13" <<"14" << "15" <<"16");
    ui->table_Consumption_Price_5->clearContents();
    ui->table_Max_Consumption_Per_Tier_5->setRowCount(1);
    ui->table_Max_Consumption_Per_Tier_5->setColumnCount(7);
    ui->table_Max_Consumption_Per_Tier_5->setHorizontalHeaderLabels(
                QStringList() << "1" << "2" << "3" << "4" << "5" << "6" << "7" <<"8" <<"9"  <<"10" <<"11" <<"12" <<"13" <<"14" << "15" <<"16");
    ui->table_Max_Consumption_Per_Tier_5->clearContents();

    ui->T_Sewage_Percentage_5->setText("");
    ui->T_Sewage_Price_5->setText("");

    ui->DE_Sewage_Start_Date_5->setDateTime(QDateTime::currentDateTime());
    ui->DE_Sewage_Start_Date_5->setDisplayFormat("yyyy-MM-dd hh:mm:ss AP");

    ui->radioB_Unit_Tiers_Table_flag_on_5->setChecked(false);
    ui->radioB_Unit_Tiers_Table_flag_off_5->setChecked(false);

    ui->T_Friendly_Hours_ID_5->setText("");

    ui->Workdays_sunday_chBox_5->setChecked(false);
    ui->Workdays_monday_chBox_5->setChecked(false);
    ui->Workdays_tuesday_chBox_5->setChecked(false);
    ui->Workdays_wensday_chBox_5->setChecked(false);
    ui->Workdays_thursday_chBox_5->setChecked(false);
    ui->Workdays_frieday_chBox_5->setChecked(false);
    ui->Workdays_saturday_chBox_5->setChecked(false);

    ui->radioB_Start_Working_Hour_AM__1->setChecked(false);
    ui->radioB_Start_Working_Hour_AM__2->setChecked(false);
    ui->radioB_Start_Working_Hour_AM__3->setChecked(false);
    ui->radioB_Start_Working_Hour_AM__4->setChecked(false);
    ui->radioB_Start_Working_Hour_AM__5->setChecked(false);
    ui->radioB_Start_Working_Hour_AM__6->setChecked(false);
    ui->radioB_Start_Working_Hour_AM__7->setChecked(false);
    ui->radioB_Start_Working_Hour_AM__8->setChecked(false);
    ui->radioB_Start_Working_Hour_AM__9->setChecked(false);
    ui->radioB_Start_Working_Hour_AM__10->setChecked(false);
    ui->radioB_Start_Working_Hour_AM__11->setChecked(false);
    ui->radioB_Start_Working_Hour_AM__12->setChecked(false);

    ui->radioB_End_Working_Hour_PM__1->setChecked(false);
    ui->radioB_End_Working_Hour_PM__2->setChecked(false);
    ui->radioB_End_Working_Hour_PM__3->setChecked(false);
    ui->radioB_End_Working_Hour_PM__4->setChecked(false);
    ui->radioB_End_Working_Hour_PM__5->setChecked(false);
    ui->radioB_End_Working_Hour_PM__6->setChecked(false);
    ui->radioB_End_Working_Hour_PM__7->setChecked(false);
    ui->radioB_End_Working_Hour_PM__8->setChecked(false);
    ui->radioB_End_Working_Hour_PM__9->setChecked(false);
    ui->radioB_End_Working_Hour_PM__10->setChecked(false);
    ui->radioB_End_Working_Hour_PM__11->setChecked(false);
    ui->radioB_End_Working_Hour_PM__12->setChecked(false);

    ui->T_Overdraft_amount_of_Time_5->setText("");
    ui->T_Overdraft_amount_of_Money_5->setText("");
    ui->T_Holiday_month_day_5->setText("");
}
/**
         * @brief Implemnation function to initial interface components for File 006.
     */
void WaterMeterParserView::displayFile006Dataini() {
    ui->dateTime_Lastservertransactiondate_6->setDateTime(QDateTime::currentDateTime());
    ui->dateTime_Lastservertransactiondate_6->setDisplayFormat("yyyy-MM-dd hh:mm:ss AP");

    ui->T_CardStatus_6->setText("Inactive");

    ui->MeterActions_chBox1_6->setChecked(false);
    ui->MeterActions_chBox2_6->setChecked(false);
    ui->MeterActions_chBox3_6->setChecked(false);
    ui->MeterActions_chBox4_6->setChecked(false);
    ui->MeterActions_chBox5_6->setChecked(false);
    ui->MeterActions_chBox6_6->setChecked(false);

    ui->dateTime_MeterDate->setDateTime(QDateTime::currentDateTime());
    ui->dateTime_MeterDate->setDisplayFormat("yyyy-MM-dd hh:mm:ss AP");
}
/**
         * @brief Implemnation function to initial interface components for File 007.
     */
void WaterMeterParserView::displayFile007Dataini() {
    ui->dateTime_Lastservertransactiondate_7->setDateTime(QDateTime::currentDateTime());
    ui->dateTime_Lastservertransactiondate_7->setDisplayFormat("yyyy-MM-dd hh:mm:ss AP");

    ui->T_CardStatus_7->setText("Inactive");

    ui->MeterActions_chBox1_7->setChecked(false);
    ui->MeterActions_chBox2_7->setChecked(false);
    ui->MeterActions_chBox3_7->setChecked(false);
    ui->MeterActions_chBox4_7->setChecked(false);
    ui->MeterActions_chBox5_7->setChecked(false);
    ui->MeterActions_chBox6_7->setChecked(false);
}
/**
         * @brief Implemnation function to initial interface components for File 0010.
     */
void WaterMeterParserView::displayFile0010Dataini() {
    ui->dateTime_Lastservertransactiondate_10->setDateTime(QDateTime::currentDateTime());
    ui->dateTime_Lastservertransactiondate_10->setDisplayFormat("yyyy-MM-dd hh:mm:ss AP");

    ui->T_CardStatus_10->setText("Inactive");

    ui->MeterActions_chBox1_10->setChecked(false);
    ui->MeterActions_chBox2_10->setChecked(false);
    ui->MeterActions_chBox3_10->setChecked(false);
    ui->MeterActions_chBox4_10->setChecked(false);
    ui->MeterActions_chBox5_10->setChecked(false);
    ui->MeterActions_chBox6_10->setChecked(false);
    ui->MeterActions_chBox7_10->setChecked(false);
    ui->MeterActions_chBox8_10->setChecked(false);
}
/**
         * @brief Implemnation function to initial interface components for File 0011.
     */
void WaterMeterParserView::displayFile0011Dataini() {
    ui->T_ChargeID_11->setText("");
    ui->T_ChargeAmount_11->setText("");
    ui->T_ReplacementChargeID_11->setText("");
    ui->T_ReplacementChargeAmount_11->setText("");
    ui->T_LowCreditLimitValue_11->setText("");
    ui->T_Lowcreditpercentagevalue_11->setText("");
    ui->T_Lowcreditlimitoption_11->setText("");
    ui->T_DeductionID_11->setText("");
    ui->T_Numberofcycles_11->setText("");
    ui->T_Deductedvalue_11->setText("");

    ui->dat_DeductionstartcycleDate_11->setDateTime(QDateTime::currentDateTime());
    ui->dat_DeductionstartcycleDate_11->setDisplayFormat("yyyy-MM-dd hh:mm:ss AP");

    ui->T_Reading_11->setText("");
    ui->T_CurrentMonthReading_11->setText("");

    ui->table_Monthlyconsumption_11->setRowCount(1);
    ui->table_Monthlyconsumption_11->setColumnCount(12);
    ui->table_Monthlyconsumption_11->setHorizontalHeaderLabels(
        QStringList() << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10" << "11" << "12");

    ui->T_Overdraftconsumption_11->setText("");
    ui->T_LeakageConsumption_11->setText("");
}
/**
         * @brief Implemnation function to initial interface components for File 0012.
     */
void WaterMeterParserView::displayFile0012Dataini() {
    ui->T_Service_point_category_id_12->setText("");
    ui->T_Service_point_activity_id_12->setText("");

    ui->radioB_Service_cut_flag_Pre_Paid_12->setChecked(false);
    ui->radioB_Service_cut_flag_Post_Paid_12->setChecked(false);

    ui->T_Number_of_Units_12->setText("");

    ui->radioB_Sewage_service_flag_service_covered_12->setChecked(false);
    ui->radioB_Sewage_service_flag_service_not_covered_12->setChecked(false);

    ui->T_TariffID_12->setText("");
    ui->T_Month_Fixed_Fees_12->setText("");
    ui->T_Month_Fixed_Fees_Per_Unit_12->setText("");

    ui->table_Month_Fixed_Step_Fees_12->setRowCount(1);
    ui->table_Month_Fixed_Step_Fees_12->setColumnCount(7);
    ui->table_Month_Fixed_Step_Fees_12->setHorizontalHeaderLabels(
                QStringList() << "1" << "2" << "3" << "4" << "5" << "6" << "7" <<"8" <<"9"  <<"10" <<"11" <<"12" <<"13" <<"14" << "15" <<"16");

    ui->T_Month_Fixed_Fees_per_cubic_meter_12->setText("");

    ui->Month_Fixed_Fees_Chbox_12->setChecked(false);
    ui->Month_Fixed_Fees_per_Unit_ChBox_12->setChecked(false);
    ui->Month_Fixed_Steps_ChBox_12->setChecked(false);
    ui->Month_Fixed_Fees_per_Cubic_Meter_ChBox_12->setChecked(false);

    ui->Tier_chBox1_12->setChecked(false);
    ui->Tier_chBox2_12->setChecked(false);
    ui->Tier_chBox3_12->setChecked(false);
    ui->Tier_chBox4_12->setChecked(false);
    ui->Tier_chBox5_12->setChecked(false);
    ui->Tier_chBox6_12->setChecked(false);
    ui->Tier_chBox7_12->setChecked(false);
    ui->Tier_chBox8_12->setChecked(false);
    ui->Tier_chBox9_12->setChecked(false);
    ui->Tier_chBox10_12->setChecked(false);
    ui->Tier_chBox11_12->setChecked(false);
    ui->Tier_chBox12_12->setChecked(false);
    ui->Tier_chBox13_12->setChecked(false);
    ui->Tier_chBox14_12->setChecked(false);
    ui->Tier_chBox15_12->setChecked(false);
    ui->Tier_chBox16_12->setChecked(false);

    ui->table_Consumption_Price_12->setRowCount(1);
    ui->table_Consumption_Price_12->setColumnCount(7);
    ui->table_Consumption_Price_12->setHorizontalHeaderLabels(
                QStringList() << "1" << "2" << "3" << "4" << "5" << "6" << "7" <<"8" <<"9"  <<"10" <<"11" <<"12" <<"13" <<"14" << "15" <<"16");

    ui->table_Max_Consumption_Per_Tier_12->setRowCount(1);
    ui->table_Max_Consumption_Per_Tier_12->setColumnCount(7);
    ui->table_Max_Consumption_Per_Tier_12->setHorizontalHeaderLabels(
                QStringList() << "1" << "2" << "3" << "4" << "5" << "6" << "7" <<"8" <<"9"  <<"10" <<"11" <<"12" <<"13" <<"14" << "15" <<"16");

    ui->T_Sewage_Percentage_12->setText("");
    ui->T_Sewage_Price_12->setText("");

    ui->DE_Sewage_Start_Date_12->setDateTime(QDateTime::currentDateTime());
    ui->DE_Sewage_Start_Date_12->setDisplayFormat("yyyy-MM-dd hh:mm:ss AP");

    ui->radioB_Unit_Tiers_Table_flag_on_12->setChecked(false);
    ui->radioB_Unit_Tiers_Table_flag_off_12->setChecked(false);

    ui->T_Friendly_Hours_ID_12->setText("");

    ui->Workdays_sunday_chBox_12->setChecked(false);
    ui->Workdays_monday_chBox_12->setChecked(false);
    ui->Workdays_tuesday_chBox_12->setChecked(false);
    ui->Workdays_wensday_chBox_12->setChecked(false);
    ui->Workdays_thursday_chBox_12->setChecked(false);
    ui->Workdays_frieday_chBox_12->setChecked(false);
    ui->Workdays_saturday_chBox_12->setChecked(false);

    ui->radioB_Start_Working_Hour_AM__1_12->setChecked(false);
    ui->radioB_Start_Working_Hour_AM__2_12->setChecked(false);
    ui->radioB_Start_Working_Hour_AM__3_12->setChecked(false);
    ui->radioB_Start_Working_Hour_AM__4_12->setChecked(false);
    ui->radioB_Start_Working_Hour_AM__5_12->setChecked(false);
    ui->radioB_Start_Working_Hour_AM__6_12->setChecked(false);
    ui->radioB_Start_Working_Hour_AM__7_12->setChecked(false);
    ui->radioB_Start_Working_Hour_AM__8_12->setChecked(false);
    ui->radioB_Start_Working_Hour_AM__9_12->setChecked(false);
    ui->radioB_Start_Working_Hour_AM__10_12->setChecked(false);
    ui->radioB_Start_Working_Hour_AM__11_12->setChecked(false);
    ui->radioB_Start_Working_Hour_AM__12_12->setChecked(false);

    ui->radioB_End_Working_Hour_PM__1_12->setChecked(false);
    ui->radioB_End_Working_Hour_PM__2_12->setChecked(false);
    ui->radioB_End_Working_Hour_PM__3_12->setChecked(false);
    ui->radioB_End_Working_Hour_PM__4_12->setChecked(false);
    ui->radioB_End_Working_Hour_PM__5_12->setChecked(false);
    ui->radioB_End_Working_Hour_PM__6_12->setChecked(false);
    ui->radioB_End_Working_Hour_PM__7_12->setChecked(false);
    ui->radioB_End_Working_Hour_PM__8_12->setChecked(false);
    ui->radioB_End_Working_Hour_PM__9_12->setChecked(false);
    ui->radioB_End_Working_Hour_PM__10_12->setChecked(false);
    ui->radioB_End_Working_Hour_PM__11_12->setChecked(false);
    ui->radioB_End_Working_Hour_PM__12_12->setChecked(false);

    ui->T_Overdraft_amount_of_Time_12->setText("");
    ui->T_Overdraft_amount_of_Money_12->setText("");
    ui->T_Holiday_month_day_12->setText("");
}
/**
         * @brief Implemnation function to initial interface components for File 0013.
     */
void WaterMeterParserView::displayFile0013Dataini() {
    ui->T_Creditbalance_13->setText("");
    ui->T_Overdraftcredit_13->setText("");
    ui->T_Consumedcredit_13->setText("");

    // Initialize Consumed Credit Monthly Table
    ui->table_Consumedcreditmonthly_13->setRowCount(1);
    ui->table_Consumedcreditmonthly_13->setColumnCount(12);
    ui->table_Consumedcreditmonthly_13->setHorizontalHeaderLabels(
        QStringList() << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10" << "11" << "12");

    ui->T_Cumulativecharge_13->setText("");
    ui->T_ActiveTariffID_13->setText("");

    ui->dat_Activetariffstartdate_13->setDateTime(QDateTime::currentDateTime());
    ui->dat_Activetariffstartdate_13->setDisplayFormat("yyyy-MM-dd hh:mm:ss AP");

    ui->T_OtherTariffID_13->setText("");
}
/**
         * @brief Implemnation function to initial interface components for File 0014.
     */
void WaterMeterParserView::displayFile0014Dataini() {
    // Initialize Last Occurrence Date Table
    ui->table_Lastoccurrencedate_14->setRowCount(1);
    ui->table_Lastoccurrencedate_14->setColumnCount(12);
    ui->table_Lastoccurrencedate_14->setHorizontalHeaderLabels(
        QStringList() << "1" << "2" << "3" << "4" << "5" << "6" << "7");

    // Initialize Last Clearance Date Table
    ui->table_Lastclearancedate_14->setRowCount(1);
    ui->table_Lastclearancedate_14->setColumnCount(12);
    ui->table_Lastclearancedate_14->setHorizontalHeaderLabels(
        QStringList() << "1" << "2" << "3" << "4" << "5" << "6" << "7");

    // Initialize Tamper Count Table
    ui->table_Tampercount_14->setRowCount(1);
    ui->table_Tampercount_14->setColumnCount(12);
    ui->table_Tampercount_14->setHorizontalHeaderLabels(
        QStringList() << "1" << "2" << "3" << "4" << "5" << "6");

    // Initialize Tamper Last Occurrence Date Table
    ui->table_TamperLastoccurrencedate_14->setRowCount(1);
    ui->table_TamperLastoccurrencedate_14->setColumnCount(12);
    ui->table_TamperLastoccurrencedate_14->setHorizontalHeaderLabels(
        QStringList() << "1" << "2" << "3" << "4" << "5" << "6");

    ui->T_ValveHistory_14->setText("");

    // Initialize Tamper Last Clearance Date Table
    ui->table_TamperLastclearancedate_14->setRowCount(1);
    ui->table_TamperLastclearancedate_14->setColumnCount(12);
    ui->table_TamperLastclearancedate_14->setHorizontalHeaderLabels(
        QStringList() << "1" << "2" << "3" << "4");
}
/**
         * @brief Implemnation function to initial interface components for File 0015.
     */
void WaterMeterParserView::displayFile0015Dataini() {
    ui->dateTime_LastMeterTransactionDate_15->setDateTime(QDateTime::currentDateTime());
    ui->dateTime_LastMeterTransactionDate_15->setDisplayFormat("yyyy-MM-dd hh:mm:ss AP");

    ui->ReturnedMeterActions_chBox1_15->setChecked(false);
    ui->ReturnedMeterActions_chBox2_15->setChecked(false);
    ui->ReturnedMeterActions_chBox3_15->setChecked(false);
    ui->ReturnedMeterActions_chBox4_15->setChecked(false);
    ui->ReturnedMeterActions_chBox5_15->setChecked(false);
    ui->ReturnedMeterActions_chBox6_15->setChecked(false);
    ui->ReturnedMeterActions_chBox7_15->setChecked(false);
    ui->ReturnedMeterActions_chBox8_15->setChecked(false);

    ui->T_ErrorCode_15->setText("");
    ui->T_notused_15->setText("");
}
/**
         * @brief Implemnation function to initial interface components for File 0016.
     */
void WaterMeterParserView::displayFile0016Dataini() {
   //005
    ui->T_Service_point_category_id_16->setText("");
    ui->T_Service_point_activity_id_16->setText("");

    ui->radioB_Service_cut_flag_Pre_Paid_16->setChecked(false);
    ui->radioB_Service_cut_flag_Post_Paid_16->setChecked(false);

    ui->T_Number_of_Units_16->setText("");

    ui->radioB_Sewage_service_flag_service_covered_16->setChecked(false);
    ui->radioB_Sewage_service_flag_service_not_covered_16->setChecked(false);

    ui->T_TariffID_16->setText("");
    ui->T_Month_Fixed_Fees_16->setText("");
    ui->T_Month_Fixed_Fees_Per_Unit_16->setText("");

    ui->table_Month_Fixed_Step_Fees_16->setRowCount(1);
    ui->table_Month_Fixed_Step_Fees_16->setColumnCount(7);
    ui->table_Month_Fixed_Step_Fees_16->setHorizontalHeaderLabels(
        QStringList() << "1" << "2" << "3" << "4" << "5" << "6" << "7" <<"8" <<"9"  <<"10" <<"11" <<"12" <<"13" <<"14" << "15" <<"16");
    ui->table_Month_Fixed_Step_Fees_16->clearContents();

    ui->T_Month_Fixed_Fees_per_cubic_meter_16->setText("");

    ui->Month_Fixed_Fees_Chbox_16->setChecked(false);
    ui->Month_Fixed_Fees_per_Unit_ChBox_16->setChecked(false);
    ui->Month_Fixed_Steps_ChBox_16->setChecked(false);
    ui->Month_Fixed_Fees_per_Cubic_Meter_ChBox_16->setChecked(false);

    ui->Tier_chBox1_16->setChecked(false);
    ui->Tier_chBox2_16->setChecked(false);
    ui->Tier_chBox3_16->setChecked(false);
    ui->Tier_chBox4_16->setChecked(false);
    ui->Tier_chBox5_16->setChecked(false);
    ui->Tier_chBox6_16->setChecked(false);
    ui->Tier_chBox7_16->setChecked(false);
    ui->Tier_chBox8_16->setChecked(false);
    ui->Tier_chBox9_16->setChecked(false);
    ui->Tier_chBox10_16->setChecked(false);
    ui->Tier_chBox11_16->setChecked(false);
    ui->Tier_chBox12_16->setChecked(false);
    ui->Tier_chBox13_16->setChecked(false);
    ui->Tier_chBox14_16->setChecked(false);
    ui->Tier_chBox15_16->setChecked(false);
    ui->Tier_chBox16_16->setChecked(false);

    ui->table_Consumption_Price_16->setRowCount(1);
    ui->table_Consumption_Price_16->setColumnCount(7);
    ui->table_Consumption_Price_16->setHorizontalHeaderLabels(
                QStringList() << "1" << "2" << "3" << "4" << "5" << "6" << "7" <<"8" <<"9"  <<"10" <<"11" <<"12" <<"13" <<"14" << "15" <<"16");
    ui->table_Consumption_Price_16->clearContents();
    ui->table_Max_Consumption_Per_Tier_16->setRowCount(1);
    ui->table_Max_Consumption_Per_Tier_16->setColumnCount(7);
    ui->table_Max_Consumption_Per_Tier_16->setHorizontalHeaderLabels(
                QStringList() << "1" << "2" << "3" << "4" << "5" << "6" << "7" <<"8" <<"9"  <<"10" <<"11" <<"12" <<"13" <<"14" << "15" <<"16");
    ui->table_Max_Consumption_Per_Tier_16->clearContents();

    ui->T_Sewage_Percentage_16->setText("");
    ui->T_Sewage_Price_16->setText("");

    ui->DE_Sewage_Start_Date_16->setDateTime(QDateTime::currentDateTime());
    ui->DE_Sewage_Start_Date_16->setDisplayFormat("yyyy-MM-dd hh:mm:ss AP");

    ui->radioB_Unit_Tiers_Table_flag_on_16->setChecked(false);
    ui->radioB_Unit_Tiers_Table_flag_off_16->setChecked(false);

    ui->T_Friendly_Hours_ID_16->setText("");

    ui->Workdays_sunday_chBox_16->setChecked(false);
    ui->Workdays_monday_chBox_16->setChecked(false);
    ui->Workdays_tuesday_chBox_16->setChecked(false);
    ui->Workdays_wensday_chBox_16->setChecked(false);
    ui->Workdays_thursday_chBox_16->setChecked(false);
    ui->Workdays_frieday_chBox_16->setChecked(false);
    ui->Workdays_saturday_chBox_16->setChecked(false);

    ui->radioB_Start_Working_Hour_AM__1_16->setChecked(false);
    ui->radioB_Start_Working_Hour_AM__2_16->setChecked(false);
    ui->radioB_Start_Working_Hour_AM__3_16->setChecked(false);
    ui->radioB_Start_Working_Hour_AM__4_16->setChecked(false);
    ui->radioB_Start_Working_Hour_AM__5_16->setChecked(false);
    ui->radioB_Start_Working_Hour_AM__6_16->setChecked(false);
    ui->radioB_Start_Working_Hour_AM__7_16->setChecked(false);
    ui->radioB_Start_Working_Hour_AM__8_16->setChecked(false);
    ui->radioB_Start_Working_Hour_AM__9_16->setChecked(false);
    ui->radioB_Start_Working_Hour_AM__10_16->setChecked(false);
    ui->radioB_Start_Working_Hour_AM__11_16->setChecked(false);
    ui->radioB_Start_Working_Hour_AM__12_16->setChecked(false);

    ui->radioB_End_Working_Hour_PM__1_16->setChecked(false);
    ui->radioB_End_Working_Hour_PM__2_16->setChecked(false);
    ui->radioB_End_Working_Hour_PM__3_16->setChecked(false);
    ui->radioB_End_Working_Hour_PM__4_16->setChecked(false);
    ui->radioB_End_Working_Hour_PM__5_16->setChecked(false);
    ui->radioB_End_Working_Hour_PM__6_16->setChecked(false);
    ui->radioB_End_Working_Hour_PM__7_16->setChecked(false);
    ui->radioB_End_Working_Hour_PM__8_16->setChecked(false);
    ui->radioB_End_Working_Hour_PM__9_16->setChecked(false);
    ui->radioB_End_Working_Hour_PM__10_16->setChecked(false);
    ui->radioB_End_Working_Hour_PM__11_16->setChecked(false);
    ui->radioB_End_Working_Hour_PM__12_16->setChecked(false);

    ui->T_Overdraft_amount_of_Time_16->setText("");
    ui->T_Overdraft_amount_of_Money_16->setText("");
    ui->T_Holiday_month_day_16->setText("");
    //11
    ui->T_ChargeID_16->setText("");
    ui->T_ChargeAmount_16->setText("");
    ui->T_ReplacementChargeID_16->setText("");
    ui->T_ReplacementChargeAmount_16->setText("");
    ui->T_LowCreditLimitValue_16->setText("");
    ui->T_Lowcreditpercentagevalue_16->setText("");
    ui->T_Lowcreditlimitoption_16->setText("");
    ui->T_DeductionID_16->setText("");
    ui->T_Numberofcycles_16->setText("");
    ui->T_Deductedvalue_16->setText("");

//11
    ui->T_Reading_16->setText("");
    ui->T_CurrentMonthReading_16->setText("");

    ui->table_Monthlyconsumption_16->setRowCount(1);
    ui->table_Monthlyconsumption_16->setColumnCount(12);
    ui->table_Monthlyconsumption_16->setHorizontalHeaderLabels(
        QStringList() << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10" << "11" << "12");

    ui->T_Overdraftconsumption_16->setText("");
    ui->T_LeakageConsumption_16->setText("");

    //14
    // Initialize Last Occurrence Date Table
    ui->table_Lastoccurrencedate_16->setRowCount(1);
    ui->table_Lastoccurrencedate_16->setColumnCount(12);
    ui->table_Lastoccurrencedate_16->setHorizontalHeaderLabels(
        QStringList() << "1" << "2" << "3" << "4" << "5" << "6" << "7");

    // Initialize Last Clearance Date Table
    ui->table_Lastclearancedate_16->setRowCount(1);
    ui->table_Lastclearancedate_16->setColumnCount(12);
    ui->table_Lastclearancedate_16->setHorizontalHeaderLabels(
        QStringList() << "1" << "2" << "3" << "4" << "5" << "6" << "7");

    // Initialize Tamper Count Table
    ui->table_Tampercount_16->setRowCount(1);
    ui->table_Tampercount_16->setColumnCount(12);
    ui->table_Tampercount_16->setHorizontalHeaderLabels(
        QStringList() << "1" << "2" << "3" << "4" << "5" << "6");

    // Initialize Tamper Last Occurrence Date Table
    ui->table_TamperLastoccurrencedate_16->setRowCount(1);
    ui->table_TamperLastoccurrencedate_16->setColumnCount(12);
    ui->table_TamperLastoccurrencedate_16->setHorizontalHeaderLabels(
        QStringList() << "1" << "2" << "3" << "4" << "5" << "6");

    ui->T_ValveHistory_14->setText("");

    // Initialize Tamper Last Clearance Date Table
    ui->table_TamperLastclearancedate_16->setRowCount(1);
    ui->table_TamperLastclearancedate_16->setColumnCount(12);
    ui->table_TamperLastclearancedate_16->setHorizontalHeaderLabels(
        QStringList() << "1" << "2" << "3" << "4");

    //13
    ui->T_Creditbalance_16->setText("");
    ui->T_Overdraftcredit_16->setText("");
    ui->T_Consumedcredit_16->setText("");

    // Initialize Consumed Credit Monthly Table
    ui->table_Consumedcreditmonthly_16->setRowCount(1);
    ui->table_Consumedcreditmonthly_16->setColumnCount(12);
    ui->table_Consumedcreditmonthly_16->setHorizontalHeaderLabels(
        QStringList() << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10" << "11" << "12");

    ui->T_Cumulativecharge_16->setText("");
    ui->T_ActiveTariffID_16->setText("");

    ui->dat_Activetariffstartdate_16->setDateTime(QDateTime::currentDateTime());
    ui->dat_Activetariffstartdate_16->setDisplayFormat("yyyy-MM-dd hh:mm:ss AP");

    ui->T_OtherTariffID_16->setText("");


    //15:

    ui->dateTime_LastMeterTransactionDate_16->setDateTime(QDateTime::currentDateTime());
    ui->dateTime_LastMeterTransactionDate_16->setDisplayFormat("yyyy-MM-dd hh:mm:ss AP");

    ui->ReturnedMeterActions_chBox1_16->setChecked(false);
    ui->ReturnedMeterActions_chBox2_16->setChecked(false);
    ui->ReturnedMeterActions_chBox3_16->setChecked(false);
    ui->ReturnedMeterActions_chBox4_16->setChecked(false);
    ui->ReturnedMeterActions_chBox5_16->setChecked(false);
    ui->ReturnedMeterActions_chBox6_16->setChecked(false);
    ui->ReturnedMeterActions_chBox7_16->setChecked(false);
    ui->ReturnedMeterActions_chBox8_16->setChecked(false);

    ui->T_ErrorCode_16->setText("");
    ui->T_notused_16->setText("");

    //2

    ui->T_SubscriptionNumber_16->setText("");


}


/**
     * @brief Implementation function of Displays the parsed files data in the view.
     *
     * @param fileData A QSharedPointer to the parsed FileData object.
     */
void WaterMeterParserView::displayResult(QSharedPointer<FileData> fileData,int selectedFileIndex) {

    if(!fileData){
        qDebug()<<"Error: fileDate is null.";
        return;
    }
    switch (selectedFileIndex) {
        case 1: // File 0001
             displayFile001Data(fileData);
            break;
        case 2: // File 0002
            displayFile002Data(fileData);
            break;
        case 3: // File 0003
             displayFile003Data(fileData);
            break;
        case 4: // File 0004
             displayFile004Data(fileData);
            break;
        case 5: // File 0005
             displayFile005Data(fileData);
            break;
        case 6: // File 0006
             displayFile006Data(fileData);
            break;
        case 7: // File 0007
             displayFile007Data(fileData);
            break;
        case 8: // File 0010
             displayFile0010Data(fileData);
            break;
        case 9: // File 0011
             displayFile0011Data(fileData);
            break;
        case 10: // File 0012
             displayFile0012Data(fileData);
            break;
        case 11: // File 0013
             displayFile0013Data(fileData);
            break;
        case 12: // File 0014
             displayFile0014Data(fileData);
            break;
        case 13: // File 0015
             displayFile0015Data(fileData);
            break;
        case 14:// File 0016
            displayFile0016Data(fileData);
            break;
    }
}

/**
         * @brief Implementation a function to Displays parsed data for (Client Card- Inputs (From backend to meter)- Constant Main Information 1) according to  File 001.
         *
         * @param fileData A QSharedPointer to the parsed FileData object.
         */
void WaterMeterParserView::displayFile001Data(QSharedPointer<FileData> fileData) {
    if(!fileData || fileData->FieldParsedValue.empty()){
        qDebug()<<"Error: fileData or FieldParsedValue is empty.";
        return;
    }
    // Card function (1)
    if (!fileData->FieldParsedValue[0].isEmpty()) {

        int cardFunctionInt =fileData->FieldParsedValue[0].first().toInt();

        // Update the UI with the Card Function description
            QString cardFunctionDescription;
            switch (cardFunctionInt) {
                case 0:
                    cardFunctionDescription = "Customer Card";
                    break;
                case 1:
                    cardFunctionDescription = "Configuration Card";
                    break;
                case 2:
                    cardFunctionDescription = "Retrieval Card";
                    break;
                case 3:
                    cardFunctionDescription = "Maintenance Card";
                    break;
                default:
                    cardFunctionDescription = "Unknown Card Function :" +QString::number( cardFunctionInt);
                    break;
            }
            ui->T_CardFunction->setText(cardFunctionDescription);

    }

    // Utility ID (2)
    if (!fileData->FieldParsedValue[1].isEmpty()) {
        int utilityIdInt = fileData->FieldParsedValue[1].first().toInt();

        // Update the UI with the Utility ID integer value
            ui->T_UtilityID->setText(QString::number(utilityIdInt));

    }
}
/**
         * @brief Implementation a function to Displays parsed data for (Client Card- Inputs (From backend to meter)- Constant Main Information 2)  according to File 002.
         *
         * @param fileData A QSharedPointer to the parsed FileData object.
         */
void WaterMeterParserView::displayFile002Data(QSharedPointer<FileData> fileData) {
    if(!fileData || fileData->FieldParsedValue.empty()){
        qDebug()<<"Error: fileData or FieldParsedValue is empty.";
        return;
    }
    // MeterSerial 0
    ui->T_MeterSerial->setText(fileData->FieldParsedValue[0].first());

    // MeterInitialReading 1
    ui->T_MeterInitialReading->setText(fileData->FieldParsedValue[1].first());
    // DiameterID 2
    ui->T_DiameterID->setText(fileData->FieldParsedValue[2].first());
    // ProductionYear 3
    ui->T_ProductionYear->setText(QString::number(fileData->FieldParsedValue[3].first().toInt()+2000));
    // MeterSupplierID 4
    ui->T_MeterSupplierID->setText(fileData->FieldParsedValue[4].first());
    // SubscriptionNumber 5
    ui->T_SubscriptionNumber->setText(fileData->FieldParsedValue[5].first());
}
/**
         * @brief Implementation a function to Displays parsed data for Client Card- Inputs (From backend to meter)- Changed Main Data)  according to File 003.
         *
         * @param fileData A QSharedPointer to the parsed FileData object.
         */
void WaterMeterParserView::displayFile003Data(QSharedPointer<FileData> fileData) {
        if(!fileData || fileData->FieldParsedValue.empty()){
            qDebug()<<"Error: fileData or FieldParsedValue is empty.";
            return;
        }
        // Last server transaction Date
        // Process Byte 1:Lastservertransactiondate(Year) 0
        // Process Byte 2: Lastservertransactiondate (Month) 1
        // Process Bytes 3-4: LastservertransactionTime (Min) 2


        // Get the parsed values from the model


        // Extract the values
        int year =          fileData->FieldParsedValue[0].first().toInt()+2000;
        int month =         fileData->FieldParsedValue[1].first().toInt();
        int minutes =       fileData->FieldParsedValue[2].first().toInt();

        // Combine date and time into a QDateTime object
        QDateTime Lastservertransactiondate=WaterMeterParserView:: Mintues_Days_Hours_Mintues(year,month,minutes);

        ui->dateTime_Lastservertransactiondate->setDateTime(Lastservertransactiondate);

     //CardStatus 3
        if (!fileData->FieldParsedValue[3].isEmpty()) {

            int CardStatusInt = fileData->FieldParsedValue[3].first().toInt();

            // Update the UI with the Card Function description
                QString CardStatusDescription;
                switch (CardStatusInt) {
                    case 0:
                        CardStatusDescription = "New";
                        break;
                    case 1:
                        CardStatusDescription = "Active";
                        break;
                    case 2:
                        CardStatusDescription = "Initialized";
                        break;
                    case 3:
                        CardStatusDescription = "Replacement";
                        break;
                    case 4:
                        CardStatusDescription = "Inactive"+QString::number( CardStatusInt);
                    break;
                        break;
                }
                ui->T_CardStatus->setText(CardStatusDescription);

        }

    //MeterActions 4


        if (!fileData->FieldParsedValue[4].isEmpty()) {
            QVector<QString> MeterActionsFlagVector = fileData->FieldParsedValue[4];
            if (!MeterActionsFlagVector.isEmpty()) {
                QString MeterActionsFlagStr = MeterActionsFlagVector.first();
                int MeterActionsFlag = MeterActionsFlagStr.toInt();

                    for (int i = 7; i >= 0; --i) {
                        if (MeterActionsFlag & (1 << i)) { // Check if the i-th bit is set
                            int tierNumber = 8 - i; // Calculate the MeterActions
                            switch (tierNumber) {
                                case 1: ui->MeterActions_chBox1->setChecked(true); break;
                                case 2: ui->MeterActions_chBox2->setChecked(true); break;
                                case 3: ui->MeterActions_chBox3->setChecked(true); break;
                                case 4: ui->MeterActions_chBox4->setChecked(true); break;
                                case 5: ui->MeterActions_chBox5->setChecked(true); break;
                                case 6: ui->MeterActions_chBox6->setChecked(true); break;
                                case 7: ui->MeterActions_chBox7->setChecked(true); break;
                                case 8: ui->MeterActions_chBox8->setChecked(true); break;

                                default: break;
                            }

                    }
                }
            }
        }


    //ValvemaintenancecycleDayofMonth 5
        if (!fileData->FieldParsedValue[5].isEmpty()) {
            ui->T_ValvemaintenancecycleDayofMonth->setText(QStringList::fromVector(fileData->FieldParsedValue[5]).first());

        }
     //T_ValvemaintenancecycleMIN 6

        ui->ValvemaintenancecycleMIN_chBox_AM->setChecked(false);
        ui->ValvemaintenancecycleMIN_chBox_PM->setChecked(false);
        ui->T_ValvemaintenancecycleMIN->setText("");

        QStringList startHourList = QStringList::fromVector(fileData->FieldParsedValue[6]);
                      qDebug()<<"startHourList"<<startHourList.first();

                      if (!startHourList.isEmpty()) {
                              QString hour = startHourList.first();
                              int hourNumDum = hour.toInt(); // Convert the hex string to an integer
                             int hourNum = hourNumDum >> 8;
                             // Convert to 12-hour format with AM/PM
                                     QString amPm = (hourNum >= 12) ? "PM" : "AM";
                                //     int hour12Format = (hourNum == 0) ? 12 : (hourNum > 12 ? hourNum - 12 : hourNum);

                                    ui->T_ValvemaintenancecycleMIN->setText(QString::number(hourNum));


                          if(amPm.contains("AM", Qt::CaseInsensitive)) {
                                  ui->ValvemaintenancecycleMIN_chBox_AM->setChecked(true);
                                  ui->ValvemaintenancecycleMIN_chBox_PM->setChecked(false);

                              } else {
                                  ui->ValvemaintenancecycleMIN_chBox_PM->setChecked(true);
                                  ui->ValvemaintenancecycleMIN_chBox_AM->setChecked(false);

                              }




        }
}
/**
         * @brief Implementation a function to Displays parsed data for (Client Card- Inputs (From backend to meter)- Charge and Deduction Data)  according to File 004.
         *
         * @param fileData A QSharedPointer to the parsed FileData object.
         */
void WaterMeterParserView::displayFile004Data(QSharedPointer<FileData>fileData){
    if(!fileData || fileData->FieldParsedValue.empty()){
        qDebug()<<"Error: fileData or FieldParsedValue is empty.";
        return;
    }
    // ChargeID 0

        ui->T_ChargeID ->setText(fileData->FieldParsedValue[0].first());
    // ChargeAmount 1


    if (!fileData->FieldParsedValue[1].isEmpty()) {
        double ChargeAmountnum = fileData->FieldParsedValue[1].first().toDouble()/1000.0;
        ui->T_ChargeAmount->setText(QString::number(ChargeAmountnum)+" .LE");
    }
    //ReplacementChargeID 2
    ui->T_ReplacementChargeID->setText(fileData->FieldParsedValue[2].first());

    //ReplacementChargeAmount 3
    ui->T_ReplacementChargeAmount->setText(QString::number(fileData->FieldParsedValue[3].first().toDouble()/1000)+" .LE");

    //Lowcreditlimitoption 6
    ui->T_Lowcreditlimitoption->setText(fileData->FieldParsedValue[6].first());

    if(fileData->FieldParsedValue[6].first()==1){

    //LowCreditLimitValue 4

        ui->T_LowCreditLimitValue ->setText(QString::number(fileData->FieldParsedValue[4].first().toDouble()/1000)+" .LE");
        ui->T_Lowcreditpercentagevalue->setText("");
         }
    else {
        //Lowcreditpercentagevalue 5
        ui->T_Lowcreditpercentagevalue->setText(fileData->FieldParsedValue[5].first()+"%");
         ui->T_LowCreditLimitValue ->setText("");
    }

   //   DeductionID 7
    ui->T_DeductionID->setText(fileData->FieldParsedValue[7].first());

 //    Numberofcycles 8
    ui->T_Numberofcycles->setText(fileData->FieldParsedValue[8].first());

 //  process Deductedvalue 9

        ui->T_Deductedvalue ->setText(QString::number(fileData->FieldParsedValue[9].first().toDouble()/1000)+" .LE");

   //Process DeductionstartcycleDate
    // Process Byte 48: Deduction start cycle Date (Year) 10
    // Process Byte 49: Deduction start cycle Date (Month) 11

    // Extract the values
    int year =  fileData->FieldParsedValue[10].first().toInt()+2000;
    int month = fileData->FieldParsedValue[11].first().toInt();

    // Create a QDate object
    QDate date(year, month,1); // Default to the first day of the month

    // Combine date  into a QDateTime object
    QDateTime DeductionstartcycleDate(date);

    // Format the date and time as "yyyy-MM"
    QString formattedDateTime = DeductionstartcycleDate.toString("yyyy-MM");

    ui->dat_DeductionstartcycleDate->setDateTime(DeductionstartcycleDate);
    ui->dat_DeductionstartcycleDate->setDisplayFormat("yyyy-MM");

}
/**
         * @brief Implementation a function to Displays parsed data for (Client Card- Inputs (From backend to meter)- (Client Data + Tariff Data + Friendly Hours))  according to File 005.
         *
         * @param fileData A QSharedPointer to the parsed FileData object.
         */
void WaterMeterParserView::displayFile005Data(QSharedPointer<FileData> fileData) {

    if(!fileData || fileData->FieldParsedValue.empty()){
        qDebug()<<"Error: fileData or FieldParsedValue is empty.";
        return;
    }
    // Service point category id 0
    ui->T_Service_point_category_id_5->setText(fileData->FieldParsedValue[0].first());
    // Service point activity id 1
    ui->T_Service_point_activity_id_5->setText(fileData->FieldParsedValue[1].first());
    // Service cut flag 2
    if (!fileData->FieldParsedValue[2].isEmpty()) {
        int Servicecutflag = fileData->FieldParsedValue[2].first().toInt();
        if(Servicecutflag==0)
        ui->radioB_Service_cut_flag_Pre_Paid_5 ->setChecked(true);
        else {
        ui->radioB_Service_cut_flag_Post_Paid_5->setChecked(true);

        }
    }

    // Number of Units 3
    ui->T_Number_of_Units_5 ->setText(fileData->FieldParsedValue[3].first());
   // Sewage service flag 4

    if (!fileData->FieldParsedValue[4].isEmpty()) {
        int SewageServiceflag = fileData->FieldParsedValue[4].first().toInt();
        if(SewageServiceflag==1)
        ui->radioB_Sewage_service_flag_service_covered_5 ->setChecked(true);
        else {
        ui->radioB_Sewage_service_flag_service_not_covered_5->setChecked(true);

        }
    }


    // Tariff 5
    ui->T_TariffID_5->setText(fileData->FieldParsedValue[5].first());
    // T_Month_Fixed_Fees_5 6
    ui->T_Month_Fixed_Fees_5->setText(QString::number( fileData->FieldParsedValue[6].first().toDouble()/100)+".LE");
    // T_Month_Fixed_Fees_5_Per_Unit_5 7
    ui->T_Month_Fixed_Fees_Per_Unit_5->setText(QString::number(fileData->FieldParsedValue[7].first().toDouble()/100)+" .LE");
    // Monthly Fixed Step Fees (Bytes 15-46) 8
    QStringList stepFees = QStringList::fromVector(fileData->FieldParsedValue[8]);
    for (int i = 0; i < stepFees.size(); ++i) {
        ui->table_Month_Fixed_Step_Fees_5->setItem(0, i,
            new QTableWidgetItem(QString::number((stepFees[i].toInt()/100)) + " .LE"));
    }

    // T_Month_Fixed_Fees_5_per_cubic_meter_5 9
    ui->T_Month_Fixed_Fees_per_cubic_meter_5 ->setText(QString::number(fileData->FieldParsedValue[9].first().toDouble()/100)+" .LE");

    // Process Byte 49: Monthly fixed fees options 10

    if (!fileData->FieldParsedValue[10].isEmpty()) {
        QStringList MonthlyfixedfeesoptionsList = QStringList::fromVector(fileData->FieldParsedValue[10]);
        bool ok;
    int binaryNumber = MonthlyfixedfeesoptionsList.first().toInt(&ok);

    if (ok) {

        ui->Month_Fixed_Fees_Chbox_5->setChecked((binaryNumber & 0b10000000)!= 0);
        ui->Month_Fixed_Fees_per_Unit_ChBox_5->setChecked((binaryNumber & 0b01000000)!= 0);
        ui->Month_Fixed_Steps_ChBox_5->setChecked((binaryNumber & 0b00100000)!= 0);
        ui->Month_Fixed_Fees_per_Cubic_Meter_ChBox_5->setChecked((binaryNumber & 0b00010000)!= 0);
    }

}

    // Process Bytes 50-51: Accumulated tier flag 11

    if (!fileData->FieldParsedValue[11].isEmpty()) {
        QVector<QString> tierFlagVector = fileData->FieldParsedValue[11];
        if (!tierFlagVector.isEmpty()) {
            QString tierFlagStr = tierFlagVector.first();
            int tierFlag = tierFlagStr.toInt();

                   for (int i = 15; i >= 0; --i) {
                    if (tierFlag & (1 << i)) { // Check if the i-th bit is set
                        int tierNumber = 16 - i; // Calculate the tier number
                        switch (tierNumber) {
                            case 1: ui->Tier_chBox1_5->setChecked(true); break;
                            case 2: ui->Tier_chBox2_5->setChecked(true); break;
                            case 3: ui->Tier_chBox3_5->setChecked(true); break;
                            case 4: ui->Tier_chBox4_5->setChecked(true); break;
                            case 5: ui->Tier_chBox5_5->setChecked(true); break;
                            case 6: ui->Tier_chBox6_5->setChecked(true); break;
                            case 7: ui->Tier_chBox7_5->setChecked(true); break;
                            case 8: ui->Tier_chBox8_5->setChecked(true); break;
                            case 9: ui->Tier_chBox9_5->setChecked(true); break;
                            case 10: ui->Tier_chBox10_5->setChecked(true); break;
                            case 11: ui->Tier_chBox11_5->setChecked(true); break;
                            case 12: ui->Tier_chBox12_5->setChecked(true); break;
                            case 13: ui->Tier_chBox13_5->setChecked(true); break;
                            case 14: ui->Tier_chBox14_5->setChecked(true); break;
                            case 15: ui->Tier_chBox15_5->setChecked(true); break;
                            case 16: ui->Tier_chBox16_5->setChecked(true); break;
                            default: break;
                        }

                }
            }
        }
    }



    // Consumption Price (Bytes 52-83) 12
    if(!ui->table_Consumption_Price_5){
        qDebug()<<"Error: table_Consumption_Price_5 is not initialized";
        return;
    }
    QStringList prices = QStringList::fromVector(fileData->FieldParsedValue[12]);
    for (int i = 0; i < prices.size(); ++i) {
        ui->table_Consumption_Price_5->setItem(0, i,
            new QTableWidgetItem(QString::number(prices[i].toDouble()/100) + " .LE"));
    }


    //  Process Bytes 84-115: Max consumption per tier 13
    if(!ui->table_Max_Consumption_Per_Tier_5){
        qDebug()<<"Error: table_Max_Consumption_Per_Tier_5 is not initialized";
        return;
    }
    QStringList pertier = QStringList::fromVector(fileData->FieldParsedValue[13]);
    for (int i = 0; i < pertier.size(); ++i) {
        ui->table_Max_Consumption_Per_Tier_5->setItem(
        0, i,
            new QTableWidgetItem(QString::number(pertier[i].toDouble()) + " .m³"));
    }

    // Process Bytes 116-117: Sewage percentage 14
    ui->T_Sewage_Percentage_5 ->setText(fileData->FieldParsedValue[14].first()+" %");

    // Process Bytes 118-119: Sewage price 15
    ui->T_Sewage_Price_5 ->setText(QString::number(fileData->FieldParsedValue[15].first().toDouble()/100)+" .LE");
    // Sewage Start Date
    // Process Byte 120: Start date (Year) 16
    // Process Byte 121: Start date (Month) 17
    // Process Bytes 122-123: Start date (Min) 18


    // Extract the values
    int year =  fileData->FieldParsedValue[16].first().toInt()+2000;
    int month = fileData->FieldParsedValue[17].first().toInt();
    int minutes =  fileData->FieldParsedValue[18].first().toInt();


     QDateTime datetimex = WaterMeterParserView:: Mintues_Days_Hours_Mintues(year,month,minutes);

    // Format the date and time as "yyyy-MM-dd"
    QString formattedDateTime = datetimex.toString("yyyy-MM-dd hh:mm");

    ui->DE_Sewage_Start_Date_5->setDateTime(datetimex);
    ui->DE_Sewage_Start_Date_5->setDisplayFormat("yyyy-MM-dd hh:mm");


    // Process Byte 124: Units tiers table flag 19
    if (!fileData->FieldParsedValue[19].isEmpty()) {
        int Unitstierstableflag = fileData->FieldParsedValue[19].first().toInt();
        if(Unitstierstableflag==1){
        ui->radioB_Unit_Tiers_Table_flag_on_5->setChecked(true);
        }
        else {
            ui->radioB_Unit_Tiers_Table_flag_off_5->setChecked(true);
        }

    }
    // Friendly Hours ID 27
    ui->T_Friendly_Hours_ID_5 ->setText(fileData->FieldParsedValue[27].first());

    // Workdays (Byte 129) 20

    if (!fileData->FieldParsedValue[20].isEmpty()) {
        QString workdaysString = fileData->FieldParsedValue[20].first();
            // Split the string by comma and trim spaces
        qDebug()<<workdaysString;
            QStringList workdaysList = workdaysString.split(", ", QString::SkipEmptyParts);

            for(int i=0;i<workdaysList.size();i++){
                if(workdaysList[i]=="Sun")
              ui->Workdays_sunday_chBox_5->setChecked(true);
                if(workdaysList[i]=="Mon")
              ui->Workdays_monday_chBox_5->setChecked(true);
                if(workdaysList[i]=="Thu")
              ui->Workdays_tuesday_chBox_5->setChecked(true);
                if(workdaysList[i]=="Wed")
              ui->Workdays_wensday_chBox_5->setChecked(true);
                if(workdaysList[i]=="Thu")
              ui->Workdays_thursday_chBox_5->setChecked(true);
                if(workdaysList[i]=="Fri")
              ui->Workdays_frieday_chBox_5->setChecked(true);
                if(workdaysList[i]=="Sat")
              ui->Workdays_saturday_chBox_5->setChecked(true);

          }

          }

    // Process Byte 130: Start Working Hour 21
    QStringList startHourList = QStringList::fromVector(fileData->FieldParsedValue[21]);
    if (!startHourList.isEmpty()) {

    for(const QString& timeStr : startHourList) {
        // Split the time string by ":" and get the hour
            QString hour = timeStr.split(":").first();
            int hourNum = hour.toInt();

            if(timeStr.contains("AM", Qt::CaseInsensitive)) {
                // Use a switch statement to set the appropriate radio button
                switch(hourNum) {
                case 1:ui->radioB_Start_Working_Hour_AM__1->setChecked(true); break;
                case 2:ui->radioB_Start_Working_Hour_AM__2->setChecked(true); break;
                case 3:ui->radioB_Start_Working_Hour_AM__3->setChecked(true); break;
                case 4:ui->radioB_Start_Working_Hour_AM__4->setChecked(true); break;
                case 5:ui->radioB_Start_Working_Hour_AM__5->setChecked(true); break;
                case 6:ui->radioB_Start_Working_Hour_AM__6->setChecked(true); break;
                case 7:ui->radioB_Start_Working_Hour_AM__7->setChecked(true); break;
                case 8:ui->radioB_Start_Working_Hour_AM__8->setChecked(true); break;
                case 9:ui->radioB_Start_Working_Hour_AM__9->setChecked(true); break;
                case 10:ui->radioB_Start_Working_Hour_AM__10->setChecked(true);break;
                case 11:ui->radioB_Start_Working_Hour_AM__11->setChecked(true);break;
                case 12:ui->radioB_Start_Working_Hour_AM__12->setChecked(true);break;

                }
            } else if(timeStr.contains("PM", Qt::CaseInsensitive)) {
                switch(hourNum) {
                case 1:ui->radioB_Start_Working_Hour_PM__1->setChecked(true); break;
                case 2:ui->radioB_Start_Working_Hour_PM__2->setChecked(true); break;
                case 3:ui->radioB_Start_Working_Hour_PM__3->setChecked(true); break;
                case 4:ui->radioB_Start_Working_Hour_PM__4->setChecked(true); break;
                case 5:ui->radioB_Start_Working_Hour_PM__5->setChecked(true); break;
                case 6:ui->radioB_Start_Working_Hour_PM__6->setChecked(true); break;
                case 7:ui->radioB_Start_Working_Hour_PM__7->setChecked(true); break;
                case 8:ui->radioB_Start_Working_Hour_PM__8->setChecked(true); break;
                case 9:ui->radioB_Start_Working_Hour_PM__9->setChecked(true); break;
                case 10:ui->radioB_Start_Working_Hour_PM__10->setChecked(true);break;
                case 11:ui->radioB_Start_Working_Hour_PM__11->setChecked(true);break;
                case 12:ui->radioB_Start_Working_Hour_PM__12->setChecked(true);break;


            }
                }
         }
       }

    // Process Byte 131: End Working Hour 22
    QStringList endHourList = QStringList::fromVector(fileData->FieldParsedValue[22]);
    if (!endHourList.isEmpty()) {
        for(const QString& timeStr : endHourList) {
            // Split the time string by ":" and get the hour
                QString hour = timeStr.split(":").first();
                int hourNum = hour.toInt();

                if(timeStr.contains("AM", Qt::CaseInsensitive)) {
                    // Use a switch statement to set the appropriate radio button
                    switch(hourNum) {
                        case 1:ui->radioB_End_Working_Hour_AM__1->setChecked(true); break;
                        case 2:ui->radioB_End_Working_Hour_AM__2->setChecked(true); break;
                        case 3:ui->radioB_End_Working_Hour_AM__3->setChecked(true); break;
                        case 4:ui->radioB_End_Working_Hour_AM__4->setChecked(true); break;
                        case 5:ui->radioB_End_Working_Hour_AM__5->setChecked(true); break;
                        case 6:ui->radioB_End_Working_Hour_AM__6->setChecked(true); break;
                        case 7:ui->radioB_End_Working_Hour_AM__7->setChecked(true); break;
                        case 8:ui->radioB_End_Working_Hour_AM__8->setChecked(true); break;
                        case 9:ui->radioB_End_Working_Hour_AM__9->setChecked(true); break;
                        case 10:ui->radioB_End_Working_Hour_AM__10->setChecked(true);break;
                        case 11:ui->radioB_End_Working_Hour_AM__11->setChecked(true);break;
                        case 12:ui->radioB_End_Working_Hour_AM__12->setChecked(true);break;
                    }
                } else if(timeStr.contains("PM", Qt::CaseInsensitive)) {
                    switch(hourNum) {
                    case 1:ui->radioB_End_Working_Hour_PM__1->setChecked(true); break;
                    case 2:ui->radioB_End_Working_Hour_PM__2->setChecked(true); break;
                    case 3:ui->radioB_End_Working_Hour_PM__3->setChecked(true); break;
                    case 4:ui->radioB_End_Working_Hour_PM__4->setChecked(true); break;
                    case 5:ui->radioB_End_Working_Hour_PM__5->setChecked(true); break;
                    case 6:ui->radioB_End_Working_Hour_PM__6->setChecked(true); break;
                    case 7:ui->radioB_End_Working_Hour_PM__7->setChecked(true); break;
                    case 8:ui->radioB_End_Working_Hour_PM__8->setChecked(true); break;
                    case 9:ui->radioB_End_Working_Hour_PM__9->setChecked(true); break;
                    case 10:ui->radioB_End_Working_Hour_PM__10->setChecked(true);break;
                    case 11:ui->radioB_End_Working_Hour_PM__11->setChecked(true);break;
                    case 12:ui->radioB_End_Working_Hour_PM__12->setChecked(true);break;

                    }
                }
        }

    }
    // Process Byte 132: Overdraft amount of time 23
    ui->T_Overdraft_amount_of_Time_5 ->setText(fileData->FieldParsedValue[23].first()+" hours");

    // Process Bytes 133-140: Overdraft amount of money 24
    ui->T_Overdraft_amount_of_Money_5 ->setText(QString::number( fileData->FieldParsedValue[24].first().toDouble()/1000)+" .LE");


    // Process Bytes 141-165: Holiday month 25
     // Process Bytes 166-190: Holiday day 26
    // Get month and day vectors
       QVector<QString> months = fileData->FieldParsedValue[25];
       QVector<QString> days =  fileData->FieldParsedValue[26];

       // Array of month names
       QStringList monthNames = {
           "January", "February", "March", "April", "May", "June",
           "July", "August", "September", "October", "November", "December"
       };

       // Create formatted holiday list
       QStringList holidayList;
       holidayList.append("Holiday Month : Day");  // Header

       // Process each month-day pair
       for (int i = 0; i < months.size(); ++i) {
           bool monthOk, dayOk;
           int monthNumber = months[i].toInt(&monthOk);
           int dayNumber = days[i].toInt(&dayOk);

           // Only process valid month-day pairs
           if (monthOk && dayOk &&
               monthNumber >= 1 && monthNumber <= 12 &&
               dayNumber > 0 && dayNumber <= 31) {

               QString monthName = monthNames[monthNumber - 1];
               QString formattedHoliday = QString("%1 - %2")
                   .arg(monthName)
                   .arg(dayNumber);

               // Only add unique entries
               if (!holidayList.contains(formattedHoliday)) {
                   holidayList.append(formattedHoliday);
               }
           }
       }

       // Sort the holidays by month and day
       auto sortByMonthDay = [&monthNames](const QString& a, const QString& b) {
           if (a == "Holiday Month : Day") return true;  // Keep header at top
           if (b == "Holiday Month : Day") return false;

           QStringList partsA = a.split(" - ");
           QStringList partsB = b.split(" - ");

           if (partsA.size() != 2 || partsB.size() != 2) return false;

           int monthIndexA = monthNames.indexOf(partsA[0]);
           int monthIndexB = monthNames.indexOf(partsB[0]);

           if (monthIndexA != monthIndexB) {
               return monthIndexA < monthIndexB;
           }

           return partsA[1].toInt() < partsB[1].toInt();
       };

       std::sort(holidayList.begin() + 1, holidayList.end(), sortByMonthDay);

       // Set the formatted text to the UI
       if (ui->T_Holiday_month_day_5) {
           ui->T_Holiday_month_day_5->setText(holidayList.join("\n"));
       }
   }
/**
         * @brief Implementation a function to Displays parsed data for (Maintenance Card -Input (From backend to meter) - (Changed Main Data))  according to File 006.
         *
         * @param fileData A QSharedPointer to the parsed FileData object.
         */
void WaterMeterParserView::displayFile006Data(QSharedPointer<FileData> fileData) {
    if(!fileData || fileData->FieldParsedValue.empty()){
        qDebug()<<"Error: fileData or FieldParsedValue is empty.";
        return;
    }
    // Last server transaction Date
            // Process Byte 1:Lastservertransactiondate(Year) 0
            // Process Byte 2: Lastservertransactiondate (Month) 1
            // Process Bytes 3-4: LastservertransactionTime (Min) 2


            // Get the parsed values from the model

            QVector<QString> LastservertransactiondateYearParsed = fileData->FieldParsedValue[0];
            QVector<QString> LastservertransactiondateMonthParsed =fileData->FieldParsedValue[1];
            QVector<QString> LastservertransactiondateMinParsed = fileData->FieldParsedValue[2];

            // Extract the values
            int year =  LastservertransactiondateYearParsed[0].toInt()+2000;
            int month = LastservertransactiondateMonthParsed[0].toInt();
            int minutes = LastservertransactiondateMinParsed[0].toInt();


            // Combine date and time into a QDateTime object
            QDateTime Lastservertransactiondate=WaterMeterParserView:: Mintues_Days_Hours_Mintues(year,month,minutes);


            // Format the date and time as "yyyy-MM-dd hh:mm:ss AP"
            QString formattedDateTime = Lastservertransactiondate.toString("yyyy-MM-dd hh:mm");

            ui->dateTime_Lastservertransactiondate_6->setDateTime(Lastservertransactiondate);
            ui->dateTime_Lastservertransactiondate_6->setDisplayFormat("yyyy-MM-dd hh:mm");


            //CardStatus 3
               if (!fileData->FieldParsedValue[3].isEmpty()) {
                   QString CardStatusValue = fileData->FieldParsedValue[3].first();
                  int CardStatusInt = CardStatusValue.toInt();
                   // Update the UI with the Card Function description
                       QString CardStatusDescription;
                       switch (CardStatusInt) {
                           case 0:
                               CardStatusDescription = "New";
                               break;
                           case 1:
                               CardStatusDescription = "Active";
                               break;
                           case 2:
                               CardStatusDescription = "Initialized";
                               break;
                           case 3:
                               CardStatusDescription = "Replacement";
                               break;
                           default:
                               CardStatusDescription = "Inactive :"+QString::number(CardStatusInt);
                               break;
                       }
                       ui->T_CardStatus_6->setText(CardStatusDescription);

               }

               //MeterActions 4

                       if (!fileData->FieldParsedValue[4].isEmpty()) {
                           QVector<QString> MeterActionsFlagVector = fileData->FieldParsedValue[4];
                           if (!MeterActionsFlagVector.isEmpty()) {
                               QString MeterActionsFlagStr = MeterActionsFlagVector.first();
                               int MeterActionsFlag = MeterActionsFlagStr.toInt();

                                   for (int i = 5; i >= 0; --i) {
                                       if (MeterActionsFlag & (1 << i)) { // Check if the i-th bit is set
                                           int MeterNumber = 6 - i; // Calculate the MeterActions
                                           switch (MeterNumber) {
                                               case 1: ui->MeterActions_chBox1_6->setChecked(true); break;
                                               case 2: ui->MeterActions_chBox2_6->setChecked(true); break;
                                               case 3: ui->MeterActions_chBox3_6->setChecked(true); break;
                                               case 4: ui->MeterActions_chBox4_6->setChecked(true); break;
                                               case 5: ui->MeterActions_chBox5_6->setChecked(true); break;
                                               case 6: ui->MeterActions_chBox6_6->setChecked(true); break;
                                               default: break;
                                           }
                                       }

                               }
                           }
                       }

                       // Meter Date
                               // Process Byte 7:MeterDate(Year) 5
                               // Process Byte 8: MeterDate (Month) 6
                               // Process Bytes 9-10: MeterDate (Min) 7
                               // Get the parsed values from the model


                               // Extract the values
                           int     year1 =  fileData->FieldParsedValue[5].first().toInt()+2000;
                           int     month1 = fileData->FieldParsedValue[6].first().toInt();
                           int     minutes1 = fileData->FieldParsedValue[7].first().toInt();


                               // Combine date and time into a QDateTime object
                               QDateTime MeterDate=WaterMeterParserView:: Mintues_Days_Hours_Mintues(year1,month1,minutes1);


                               // Format the date and time as "yyyy-MM-dd hh:mm:ss AP"
                                formattedDateTime = MeterDate.toString("yyyy-MM-dd hh:mm");

                               ui->dateTime_MeterDate->setDateTime(MeterDate);
                               ui->dateTime_MeterDate->setDisplayFormat("yyyy-MM-dd hh:mm");



}
/**
         * @brief Implementation a function to Displays parsed data for (Retrieval Card -Input (From backend to meter)-7 (Changed Main Data))  according to File 007.
         *
         * @param fileData A QSharedPointer to the parsed FileData object.
         */
void WaterMeterParserView::displayFile007Data(QSharedPointer<FileData> fileData) {
    if(!fileData || fileData->FieldParsedValue.empty()){
        qDebug()<<"Error: fileData or FieldParsedValue is empty.";
        return;
    }
    // Last server transaction Date
        // Process Byte 1:Lastservertransactiondate(Year) 0
        // Process Byte 2: Lastservertransactiondate (Month) 1
        // Process Bytes 3-4: LastservertransactionTime (Min) 2


       // Extract the values
        int year =  fileData->FieldParsedValue[0].first().toInt()+2000;
        int month = fileData->FieldParsedValue[1].first().toInt();
        int minutes =  fileData->FieldParsedValue[2].first().toInt();


        // Combine date and time into a QDateTime object
        QDateTime Lastservertransactiondate=WaterMeterParserView:: Mintues_Days_Hours_Mintues(year,month,minutes);
        // Format the date and time as "yyyy-MM-dd hh:mm:ss AP"
        QString formattedDateTime = Lastservertransactiondate.toString("yyyy-MM-dd hh:mm");

        ui->dateTime_Lastservertransactiondate_7->setDateTime(Lastservertransactiondate);
        ui->dateTime_Lastservertransactiondate_7->setDisplayFormat("yyyy-MM-dd hh:mm");

     //CardStatus 3
        if (!fileData->FieldParsedValue[3].isEmpty()) {

            int CardStatusInt =  fileData->FieldParsedValue[3].first().toInt();

            // Update the UI with the Card Function description
                QString CardStatusDescription;
                switch (CardStatusInt) {
                    case 0:
                        CardStatusDescription = "New";
                        break;
                    case 1:
                        CardStatusDescription = "Active";
                        break;
                    case 2:
                        CardStatusDescription = "Initialized";
                        break;
                    case 3:
                        CardStatusDescription = "Replacement";
                        break;
                    default:
                        CardStatusDescription = "Inactive : " + QString::number(CardStatusInt);
                        break;
                }
                ui->T_CardStatus_7->setText(CardStatusDescription);

        }

    //MeterActions 4

        if (!fileData->FieldParsedValue[4].isEmpty()) {
            QVector<QString> MeterActionsFlagVector = fileData->FieldParsedValue[4];
            if (!MeterActionsFlagVector.isEmpty()) {
                QString MeterActionsFlagStr = MeterActionsFlagVector.first();
                int MeterActionsFlag = MeterActionsFlagStr.toInt();
                    for (int i = 8; i >= 0; --i) {
                        if (MeterActionsFlag & (1 << i)) { // Check if the i-th bit is set
                            int MeterNumber = 9 - i; // Calculate the MeterActions
                            switch (MeterNumber) {

                            case 1: ui->MeterActions_chBox1_7->setChecked(true); break;
                            case 2: ui->MeterActions_chBox2_7->setChecked(true); break;
                            case 3: ui->MeterActions_chBox3_7->setChecked(true); break;
                            case 4: ui->MeterActions_chBox4_7->setChecked(true); break;
                            case 5: ui->MeterActions_chBox5_7->setChecked(true); break;
                            case 6: ui->MeterActions_chBox6_7->setChecked(true); break;
                            case 7: ui->MeterActions_chBox6_7->setChecked(true); break;
                            case 8: ui->MeterActions_chBox6_8->setChecked(true); break;
                            case 9: ui->MeterActions_chBox6_9->setChecked(true); break;

                                default: break;
                            }

                    }
                }
            }
        }



}
/**
         * @brief Implementation a function to Displays parsed data for (Client Card-(Output (From meter to backend)-(Changed Main Data))  according to File 0010.
         *
         * @param fileData A QSharedPointer to the parsed FileData object.
         */
void WaterMeterParserView::displayFile0010Data(QSharedPointer<FileData> fileData) {
    if(!fileData || fileData->FieldParsedValue.empty()){
        qDebug()<<"Error: fileData or FieldParsedValue is empty.";
        return;
    }
    // Last server transaction Date
        // Process Byte 1:Lastservertransactiondate(Year) 0
        // Process Byte 2: Lastservertransactiondate (Month) 1
        // Process Bytes 3-4: LastservertransactionTime (Min) 2


        // Get the parsed values from the model


        // Extract the values
        int year =  fileData->FieldParsedValue[0].first().toInt()+2000;
        int month = fileData->FieldParsedValue[1].first().toInt();
        int minutes = fileData->FieldParsedValue[2].first().toInt();


        // Combine date and time into a QDateTime object
        QDateTime Lastservertransactiondate=WaterMeterParserView:: Mintues_Days_Hours_Mintues(year,month,minutes);

        // Format the date and time as "yyyy-MM-dd hh:mm:ss AP"
        QString formattedDateTime = Lastservertransactiondate.toString("yyyy-MM-dd hh:mm");

        ui->dateTime_Lastservertransactiondate_10->setDateTime(Lastservertransactiondate);
        ui->dateTime_Lastservertransactiondate_10->setDisplayFormat("yyyy-MM-dd hh:mm");

     //CardStatus 3
        if (!fileData->FieldParsedValue[3].isEmpty()) {

            int CardStatusInt = fileData->FieldParsedValue[3].first().toInt();

            // Update the UI with the Card Function description
                QString CardStatusDescription;
                switch (CardStatusInt) {
                    case 0:
                        CardStatusDescription = "New";
                        break;
                    case 1:
                        CardStatusDescription = "Active";
                        break;
                    case 2:
                        CardStatusDescription = "Initialized";
                        break;
                    case 3:
                        CardStatusDescription = "Replacement";
                        break;
                    default:
                        CardStatusDescription = "Inactive: "+ QString::number(CardStatusInt);
                        break;
                }
                ui->T_CardStatus_10->setText(CardStatusDescription);

        }

    //MeterActions 4

        if (!fileData->FieldParsedValue[4].isEmpty()) {
            if (!fileData->FieldParsedValue.isEmpty()) {
                int MeterActionsFlag = fileData->FieldParsedValue[4].first().toInt();


                    for (int i = 7; i >= 0; --i) {
                        if (MeterActionsFlag & (1 << i)) { // Check if the i-th bit is set
                            int MeterNumber = 8 - i; // Calculate the MeterActions
                            switch (MeterNumber) {

                            case 1: ui->MeterActions_chBox1_10->setChecked(true); break;
                            case 2: ui->MeterActions_chBox2_10->setChecked(true); break;
                            case 3: ui->MeterActions_chBox3_10->setChecked(true); break;
                            case 4: ui->MeterActions_chBox4_10->setChecked(true); break;
                            case 5: ui->MeterActions_chBox5_10->setChecked(true); break;
                            case 6: ui->MeterActions_chBox6_10->setChecked(true); break;
                            case 7: ui->MeterActions_chBox7_10->setChecked(true); break;
                            case 8: ui->MeterActions_chBox8_10->setChecked(true); break;


                                default: break;
                            }
                        }

                }
            }
        }



}
/**
         * @brief Implementation a function to Displays parsed data for (Client Card-(Output (From meter to backend)- (Charge + Deduction + Readings)) according to File 0011.
         *
         * @param fileData A QSharedPointer to the parsed FileData object.
         */
void WaterMeterParserView::displayFile0011Data(QSharedPointer<FileData>fileData){
    if(!fileData || fileData->FieldParsedValue.empty()){
        qDebug()<<"Error: fileData or FieldParsedValue is empty.";
        return;
    }
    // ChargeID 0

        ui->T_ChargeID_11 ->setText(QStringList::fromVector(fileData->FieldParsedValue[0]).first());

    // ChargeAmount 1


        ui->T_ChargeAmount_11->setText(QString::number(fileData->FieldParsedValue[1].first().toDouble()/1000)+" .LE");
    //ReplacementChargeID 2
    ui->T_ReplacementChargeID_11->setText(fileData->FieldParsedValue[2].first());

    //ReplacementChargeAmount 3
    ui->T_ReplacementChargeAmount_11->setText(QString::number(fileData->FieldParsedValue[3].first().toDouble()/1000)+" .LE");

    //LowCreditLimitValue 4

        ui->T_LowCreditLimitValue_11 ->setText(QString::number(fileData->FieldParsedValue[4].first().toDouble()/1000)+" .LE");


    //Lowcreditpercentagevalue 5
    ui->T_Lowcreditpercentagevalue_11->setText(fileData->FieldParsedValue[5].first());

    //Lowcreditlimitoption 6
    ui->T_Lowcreditlimitoption_11->setText(fileData->FieldParsedValue[6].first());
   //   DeductionID 7
    ui->T_DeductionID_11->setText(fileData->FieldParsedValue[7].first());

 //    Numberofcycles 8
    ui->T_Numberofcycles_11->setText(fileData->FieldParsedValue[8].first());

 //  process Deductedvalue 9

        ui->T_Deductedvalue_11 ->setText(QString::number(fileData->FieldParsedValue[9].first().toDouble()/1000)+" .LE");


   //Process DeductionstartcycleDate
    // Process Byte 48: Deduction start cycle Date (Year) 10
    // Process Byte 49: Deduction start cycle Date (Month) 11

    // Extract the values
    int year =  fileData->FieldParsedValue[10].first().toInt()+2000;
    int month = fileData->FieldParsedValue[11].first().toInt();

       // Create a QDate object
    QDate date(year, month, 1); // Default to the first day of the month

    // Combine date  into a QDateTime object
    QDateTime DeductionstartcycleDate(date);

    // Format the date and time as "yyyy-MM-dd hh:mm:ss AP"
    QString formattedDateTime = DeductionstartcycleDate.toString("yyyy-MM-dd");

    ui->dat_DeductionstartcycleDate_11->setDateTime(DeductionstartcycleDate);
    ui->dat_DeductionstartcycleDate_11->setDisplayFormat("yyyy-MM-dd");

   // Reading 12
    ui->T_Reading_11->setText(fileData->FieldParsedValue[12].first());
    //Current Month Reading 13

    ui->T_CurrentMonthReading_11->setText(fileData->FieldParsedValue[13].first());

    //Monthly Cunsuming 14
    if(!ui->table_Monthlyconsumption_11){
        qDebug()<<"Error: table_Monthlyconsumption_11 is not initialized";
        return;
    }
    QStringList Monthlyconsumption = QStringList::fromVector(fileData->FieldParsedValue[14]);
    for (int i = 0; i < qMin(Monthlyconsumption.size(), ui->table_Monthlyconsumption_11->columnCount()); ++i) {
        ui->table_Monthlyconsumption_11->setItem(0, i,
            new QTableWidgetItem(Monthlyconsumption[i]));
    }

    // Overdraft consumption 15
     ui->T_Overdraftconsumption_11->setText(fileData->FieldParsedValue[15].first());
     //Leakage Consumption 16

     ui->T_LeakageConsumption_11->setText(fileData->FieldParsedValue[16].first());


}
/**
         * @brief Implementation a function to Displays parsed data for (Client Card-(Output (From meter to backend)- (Client Data + Tariff + Friendly Hours)  according to File 0012.
         *
         * @param fileData A QSharedPointer to the parsed FileData object.
         */
void WaterMeterParserView::displayFile0012Data(QSharedPointer<FileData> fileData) {

    if(!fileData || fileData->FieldParsedValue.empty()){
        qDebug()<<"Error: fileData or FieldParsedValue is empty.";
        return;
    }
    // Service point category id 0
    ui->T_Service_point_category_id_12->setText(fileData->FieldParsedValue[0].first());
    // Service point activity id 1
    ui->T_Service_point_activity_id_12->setText(fileData->FieldParsedValue[1].first());
    // Service cut flag 2
    if (!fileData->FieldParsedValue[2].isEmpty()) {
        int Servicecutflag = fileData->FieldParsedValue[2].first().toInt();
        if(Servicecutflag==0)
        ui->radioB_Service_cut_flag_Pre_Paid_12 ->setChecked(true);
        else {
        ui->radioB_Service_cut_flag_Post_Paid_12->setChecked(true);

        }
    }

    // Number of Units 3
    ui->T_Number_of_Units_12 ->setText(fileData->FieldParsedValue[3].first());
   // Sewage service flag 4

    if (!fileData->FieldParsedValue[4].isEmpty()) {
        int SewageServiceflag = fileData->FieldParsedValue[4].first().toInt();
        if(SewageServiceflag==1)
        ui->radioB_Sewage_service_flag_service_covered_12 ->setChecked(true);
        else {
        ui->radioB_Sewage_service_flag_service_not_covered_12->setChecked(true);

        }
    }


    // Tariff 5
    ui->T_TariffID_12->setText(fileData->FieldParsedValue[5].first());
    // T_Month_Fixed_Fees_5 6
    ui->T_Month_Fixed_Fees_12->setText(QString::number( fileData->FieldParsedValue[6].first().toDouble()/100)+".LE");
    // T_Month_Fixed_Fees_5_Per_Unit_5 7
    ui->T_Month_Fixed_Fees_Per_Unit_12->setText(QString::number(fileData->FieldParsedValue[7].first().toDouble()/100)+" .LE");
    // Monthly Fixed Step Fees (Bytes 15-46) 8
    if(!ui->table_Month_Fixed_Step_Fees_12){
        qDebug()<<"Error: table_Month_Fixed_Step_Fees_12 is not initialized";
        return;
    }
    QStringList stepFees = QStringList::fromVector(fileData->FieldParsedValue[8]);
    for (int i = 0; i < stepFees.size(); ++i) {
        ui->table_Month_Fixed_Step_Fees_12->setItem(0, i,
            new QTableWidgetItem(QString::number((stepFees[i].toInt()/100)) + " .LE"));
    }

    // T_Month_Fixed_Fees_5_per_cubic_meter_5 9
    ui->T_Month_Fixed_Fees_per_cubic_meter_12 ->setText(QString::number(fileData->FieldParsedValue[9].first().toDouble()/100)+" .LE");

    // Process Byte 49: Monthly fixed fees options 10

    if (!fileData->FieldParsedValue[10].isEmpty()) {
        QStringList MonthlyfixedfeesoptionsList = QStringList::fromVector(fileData->FieldParsedValue[10]);
        bool ok;
    int binaryNumber = MonthlyfixedfeesoptionsList.first().toInt(&ok);

    if (ok) {

        ui->Month_Fixed_Fees_Chbox_12->setChecked(binaryNumber & 0b10000000);
        ui->Month_Fixed_Fees_per_Unit_ChBox_12->setChecked(binaryNumber & 0b01000000);
        ui->Month_Fixed_Steps_ChBox_12->setChecked(binaryNumber & 0b00100000);
        ui->Month_Fixed_Fees_per_Cubic_Meter_ChBox_12->setChecked(binaryNumber & 0b00010000);
    }

}

    // Process Bytes 50-51: Accumulated tier flag 11

    if (!fileData->FieldParsedValue[11].isEmpty()) {
        QVector<QString> tierFlagVector = fileData->FieldParsedValue[11];
        if (!tierFlagVector.isEmpty()) {
            QString tierFlagStr = tierFlagVector.first();
            int tierFlag = tierFlagStr.toInt();

                   for (int i = 15; i >= 0; --i) {
                    if (tierFlag & (1 << i)) { // Check if the i-th bit is set
                        int tierNumber = 16 - i; // Calculate the tier number
                        switch (tierNumber) {
                            case 1: ui->Tier_chBox1_12->setChecked(true); break;
                            case 2: ui->Tier_chBox2_12->setChecked(true); break;
                            case 3: ui->Tier_chBox3_12->setChecked(true); break;
                            case 4: ui->Tier_chBox4_12->setChecked(true); break;
                            case 5: ui->Tier_chBox5_12->setChecked(true); break;
                            case 6: ui->Tier_chBox6_12->setChecked(true); break;
                            case 7: ui->Tier_chBox7_12->setChecked(true); break;
                            case 8: ui->Tier_chBox8_12->setChecked(true); break;
                            case 9: ui->Tier_chBox9_12->setChecked(true); break;
                            case 10: ui->Tier_chBox10_12->setChecked(true); break;
                            case 11: ui->Tier_chBox11_12->setChecked(true); break;
                            case 12: ui->Tier_chBox12_12->setChecked(true); break;
                            case 13: ui->Tier_chBox13_12->setChecked(true); break;
                            case 14: ui->Tier_chBox14_12->setChecked(true); break;
                            case 15: ui->Tier_chBox15_12->setChecked(true); break;
                            case 16: ui->Tier_chBox16_12->setChecked(true); break;
                            default: break;
                        }

                }
            }
        }
    }


    // Consumption Price (Bytes 52-83) 12
    if(!ui->table_Consumption_Price_12){
        qDebug()<<"Error: table_Consumption_Price_12 is not initialized";
        return;
    }
    QStringList prices = QStringList::fromVector(fileData->FieldParsedValue[12]);
    for (int i = 0; i < prices.size(); ++i) {
        ui->table_Consumption_Price_12->setItem(0, i,
            new QTableWidgetItem(QString::number(prices[i].toDouble()/100) + " .LE"));
    }


    //  Process Bytes 84-115: Max consumption per tier 13
    if(!ui->table_Max_Consumption_Per_Tier_12){
        qDebug()<<"Error: table_Max_Consumption_Per_Tier_12 is not initialized";
        return;
    }
    QStringList pertier = QStringList::fromVector(fileData->FieldParsedValue[13]);
    for (int i = 0; i < pertier.size(); ++i) {
        ui->table_Max_Consumption_Per_Tier_12->setItem(
        0, i,
            new QTableWidgetItem(QString::number(pertier[i].toDouble()) + " .m³"));
    }

    // Process Bytes 116-117: Sewage percentage 14
    ui->T_Sewage_Percentage_12 ->setText(fileData->FieldParsedValue[14].first()+" %");

    // Process Bytes 118-119: Sewage price 15
    ui->T_Sewage_Price_12 ->setText(QString::number(fileData->FieldParsedValue[15].first().toDouble()/100)+" .LE");
    // Sewage Start Date
    // Process Byte 120: Start date (Year) 16
    // Process Byte 121: Start date (Month) 17
    // Process Bytes 122-123: Start date (Min) 18


    // Extract the values
    int year =  fileData->FieldParsedValue[16].first().toInt()+2000;
    int month = fileData->FieldParsedValue[17].first().toInt();
    int minutes =  fileData->FieldParsedValue[18].first().toInt();

    QDateTime Lastservertransactiondate=WaterMeterParserView:: Mintues_Days_Hours_Mintues(year,month,minutes);

    // Format the date and time as "yyyy-MM-dd"
    QString formattedDateTime = Lastservertransactiondate.toString("yyyy-MM-dd hh:mm");

    ui->DE_Sewage_Start_Date_12->setDateTime(Lastservertransactiondate);
    ui->DE_Sewage_Start_Date_12->setDisplayFormat("yyyy-MM-dd hh:mm");


    // Process Byte 124: Units tiers table flag 19
    if (!fileData->FieldParsedValue[19].isEmpty()) {
        int Unitstierstableflag = fileData->FieldParsedValue[19].first().toInt();
        if(Unitstierstableflag==1){
        ui->radioB_Unit_Tiers_Table_flag_on_12->setChecked(true);
        }
        else {
            ui->radioB_Unit_Tiers_Table_flag_off_12->setChecked(true);
        }

    }
    // Friendly Hours ID 27
    ui->T_Friendly_Hours_ID_12 ->setText(fileData->FieldParsedValue[27].first());

    // Workdays (Byte 129) 20

    if (!fileData->FieldParsedValue[20].isEmpty()) {
        QString workdaysString = fileData->FieldParsedValue[20].first();
            // Split the string by comma and trim spaces
        qDebug()<<workdaysString;
            QStringList workdaysList = workdaysString.split(", ", QString::SkipEmptyParts);

            for(int i=0;i<workdaysList.size();i++){
                if(workdaysList[i]=="Sun")
              ui->Workdays_sunday_chBox_12->setChecked(true);
                if(workdaysList[i]=="Mon")
              ui->Workdays_monday_chBox_12->setChecked(true);
                if(workdaysList[i]=="Thu")
              ui->Workdays_tuesday_chBox_12->setChecked(true);
                if(workdaysList[i]=="Wed")
              ui->Workdays_wensday_chBox_12->setChecked(true);
                if(workdaysList[i]=="Thu")
              ui->Workdays_thursday_chBox_12->setChecked(true);
                if(workdaysList[i]=="Fri")
              ui->Workdays_frieday_chBox_12->setChecked(true);
                if(workdaysList[i]=="Sat")
              ui->Workdays_saturday_chBox_12->setChecked(true);

          }

          }

    // Process Byte 130: Start Working Hour 21
    QStringList startHourList = QStringList::fromVector(fileData->FieldParsedValue[21]);
    if (!startHourList.isEmpty()) {

    for(const QString& timeStr : startHourList) {
        // Split the time string by ":" and get the hour
            QString hour = timeStr.split(":").first();
            int hourNum = hour.toInt();
            if(timeStr.contains("AM", Qt::CaseInsensitive)) {
                // Use a switch statement to set the appropriate radio button
                switch(hourNum) {
                case 1:ui->radioB_Start_Working_Hour_AM__1_12->setChecked(true); break;
                case 2:ui->radioB_Start_Working_Hour_AM__2_12->setChecked(true); break;
                case 3:ui->radioB_Start_Working_Hour_AM__3_12->setChecked(true); break;
                case 4:ui->radioB_Start_Working_Hour_AM__4_12->setChecked(true); break;
                case 5:ui->radioB_Start_Working_Hour_AM__5_12->setChecked(true); break;
                case 6:ui->radioB_Start_Working_Hour_AM__6_12->setChecked(true); break;
                case 7:ui->radioB_Start_Working_Hour_AM__7_12->setChecked(true); break;
                case 8:ui->radioB_Start_Working_Hour_AM__8_12->setChecked(true); break;
                case 9:ui->radioB_Start_Working_Hour_AM__9_12->setChecked(true); break;
                case 10:ui->radioB_Start_Working_Hour_AM__10_12->setChecked(true);break;
                case 11:ui->radioB_Start_Working_Hour_AM__11_12->setChecked(true);break;
                case 12:ui->radioB_Start_Working_Hour_AM__12_12->setChecked(true);break;

                }
            } else if(timeStr.contains("PM", Qt::CaseInsensitive)) {
                switch(hourNum) {
                case 1:ui->radioB_Start_Working_Hour_PM__1_12->setChecked(true); break;
                case 2:ui->radioB_Start_Working_Hour_PM__2_12->setChecked(true); break;
                case 3:ui->radioB_Start_Working_Hour_PM__3_12->setChecked(true); break;
                case 4:ui->radioB_Start_Working_Hour_PM__4_12->setChecked(true); break;
                case 5:ui->radioB_Start_Working_Hour_PM__5_12->setChecked(true); break;
                case 6:ui->radioB_Start_Working_Hour_PM__6_12->setChecked(true); break;
                case 7:ui->radioB_Start_Working_Hour_PM__7_12->setChecked(true); break;
                case 8:ui->radioB_Start_Working_Hour_PM__8_12->setChecked(true); break;
                case 9:ui->radioB_Start_Working_Hour_PM__9_12->setChecked(true); break;
                case 10:ui->radioB_Start_Working_Hour_PM__10_12->setChecked(true);break;
                case 11:ui->radioB_Start_Working_Hour_PM__11_12->setChecked(true);break;
                case 12:ui->radioB_Start_Working_Hour_PM__12_12->setChecked(true);break;


            }
                }



    }
}

    // Process Byte 131: End Working Hour 22
    QStringList endHourList = QStringList::fromVector(fileData->FieldParsedValue[22]);
    if (!endHourList.isEmpty()) {
        for(const QString& timeStr : endHourList) {
            // Split the time string by ":" and get the hour
                QString hour = timeStr.split(":").first();
                int hourNum = hour.toInt();

                if(timeStr.contains("AM", Qt::CaseInsensitive)) {
                    // Use a switch statement to set the appropriate radio button
                    switch(hourNum) {
                        case 1:ui->radioB_End_Working_Hour_AM__1_12->setChecked(true); break;
                        case 2:ui->radioB_End_Working_Hour_AM__2_12->setChecked(true); break;
                        case 3:ui->radioB_End_Working_Hour_AM__3_12->setChecked(true); break;
                        case 4:ui->radioB_End_Working_Hour_AM__4_12->setChecked(true); break;
                        case 5:ui->radioB_End_Working_Hour_AM__5_12->setChecked(true); break;
                        case 6:ui->radioB_End_Working_Hour_AM__6_12->setChecked(true); break;
                        case 7:ui->radioB_End_Working_Hour_AM__7_12->setChecked(true); break;
                        case 8:ui->radioB_End_Working_Hour_AM__8_12->setChecked(true); break;
                        case 9:ui->radioB_End_Working_Hour_AM__9_12->setChecked(true); break;
                        case 10:ui->radioB_End_Working_Hour_AM__10_12->setChecked(true);break;
                        case 11:ui->radioB_End_Working_Hour_AM__11_12->setChecked(true);break;
                        case 12:ui->radioB_End_Working_Hour_AM__12_12->setChecked(true);break;
                    }
                } else if(timeStr.contains("PM", Qt::CaseInsensitive)) {
                    switch(hourNum) {
                    case 1:ui->radioB_End_Working_Hour_PM__1_12->setChecked(true); break;
                    case 2:ui->radioB_End_Working_Hour_PM__2_12->setChecked(true); break;
                    case 3:ui->radioB_End_Working_Hour_PM__3_12->setChecked(true); break;
                    case 4:ui->radioB_End_Working_Hour_PM__4_12->setChecked(true); break;
                    case 5:ui->radioB_End_Working_Hour_PM__5_12->setChecked(true); break;
                    case 6:ui->radioB_End_Working_Hour_PM__6_12->setChecked(true); break;
                    case 7:ui->radioB_End_Working_Hour_PM__7_12->setChecked(true); break;
                    case 8:ui->radioB_End_Working_Hour_PM__8_12->setChecked(true); break;
                    case 9:ui->radioB_End_Working_Hour_PM__9_12->setChecked(true); break;
                    case 10:ui->radioB_End_Working_Hour_PM__10_12->setChecked(true);break;
                    case 11:ui->radioB_End_Working_Hour_PM__11_12->setChecked(true);break;
                    case 12:ui->radioB_End_Working_Hour_PM__12_12->setChecked(true);break;

                    }
                }


        }

    }
    // Process Byte 132: Overdraft amount of time 23
    ui->T_Overdraft_amount_of_Time_12 ->setText(fileData->FieldParsedValue[23].first()+" hours");

    // Process Bytes 133-140: Overdraft amount of money 24
    ui->T_Overdraft_amount_of_Money_12 ->setText(QString::number( fileData->FieldParsedValue[24].first().toDouble()/1000)+" .LE");


    // Process Bytes 141-165: Holiday month 25
     // Process Bytes 166-190: Holiday day 26
    // Get month and day vectors
       QVector<QString> months = fileData->FieldParsedValue[25];
       QVector<QString> days =  fileData->FieldParsedValue[26];

       // Array of month names
       QStringList monthNames = {
           "January", "February", "March", "April", "May", "June",
           "July", "August", "September", "October", "November", "December"
       };

       // Create formatted holiday list
       QStringList holidayList;
       holidayList.append("Holiday Month : Day");  // Header

       // Process each month-day pair
       for (int i = 0; i < months.size(); ++i) {
           bool monthOk, dayOk;
           int monthNumber = months[i].toInt(&monthOk);
           int dayNumber = days[i].toInt(&dayOk);

           // Only process valid month-day pairs
           if (monthOk && dayOk &&
               monthNumber >= 1 && monthNumber <= 12 &&
               dayNumber > 0 && dayNumber <= 31) {

               QString monthName = monthNames[monthNumber - 1];
               QString formattedHoliday = QString("%1 - %2")
                   .arg(monthName)
                   .arg(dayNumber);

               // Only add unique entries
               if (!holidayList.contains(formattedHoliday)) {
                   holidayList.append(formattedHoliday);
               }
           }
       }

       // Sort the holidays by month and day
       auto sortByMonthDay = [&monthNames](const QString& a, const QString& b) {
           if (a == "Holiday Month : Day") return true;  // Keep header at top
           if (b == "Holiday Month : Day") return false;

           QStringList partsA = a.split(" - ");
           QStringList partsB = b.split(" - ");

           if (partsA.size() != 2 || partsB.size() != 2) return false;

           int monthIndexA = monthNames.indexOf(partsA[0]);
           int monthIndexB = monthNames.indexOf(partsB[0]);

           if (monthIndexA != monthIndexB) {
               return monthIndexA < monthIndexB;
           }

           return partsA[1].toInt() < partsB[1].toInt();
       };

       std::sort(holidayList.begin() + 1, holidayList.end(), sortByMonthDay);

       // Set the formatted text to the UI
       if (ui->T_Holiday_month_day_12) {
           ui->T_Holiday_month_day_12->setText(holidayList.join("\n"));
       }
   }
/**
         * @brief Implementation a function to Displays parsed data for (Client Card-(Output (From meter to backend)- (Credit Info))  according to File 0013.
         *
         * @param fileData A QSharedPointer to the parsed FileData object.
         */
void WaterMeterParserView::displayFile0013Data(QSharedPointer<FileData>fileData){
    if(!fileData || fileData->FieldParsedValue.empty()){
        qDebug()<<"Error: fileData or FieldParsedValue is empty.";
        return;
    }
    if(!ui->dat_Activetariffstartdate_13){
        qDebug()<<"Error: dat_Activetariffstartdate_13 is not initialized";
        return;
    }
    //    Creditbalance 0
    ui->T_Creditbalance_13->setText(QString::number(fileData->FieldParsedValue[0].first().toDouble() / 1000) + " LTE");

   // Overdraftcredit 1
    ui->T_Overdraftcredit_13->setText(QString::number(fileData->FieldParsedValue[1].first().toDouble() / 1000) + " LTE");

   // Consumedcredit 2
    ui->T_Consumedcredit_13->setText(QString::number(fileData->FieldParsedValue[2].first().toDouble() / 1000) + " LTE");

   // Consumed Credit Monthly 3
    if(!ui->table_Consumedcreditmonthly_13){
        qDebug()<<"Error: table_Consumedcreditmonthly_13 is not initialized";
        return;
    }
    QStringList ConsumedCreditMonthly = QStringList::fromVector(fileData->FieldParsedValue[3]);
    for (int i = 0; i < qMin(ConsumedCreditMonthly.size(), ui->table_Consumedcreditmonthly_13->columnCount()); ++i) {
        ui->table_Consumedcreditmonthly_13->setItem(0, i,
            new QTableWidgetItem(ConsumedCreditMonthly[i]));
    }

   // Cumulativecharge 4
    ui->T_Cumulativecharge_13->setText(QString::number(fileData->FieldParsedValue[4].first().toDouble() / 1000) + " LTE");

   //   ActiveTariffID 5
    ui->T_ActiveTariffID_13->setText(fileData->FieldParsedValue[5].first());

   // Activetariffstartdate13    6
   // ActivetariffstartdateYear
   // ActivetariffstartdateMon
  //  ActivetariffstartdateMin


    // Get the parsed values from the model


    // Extract the values
    int year =     fileData->FieldParsedValue[6].first().toInt()+2000;
    int month =    fileData->FieldParsedValue[7].first().toInt();
    int minutes =  fileData->FieldParsedValue[8].first().toInt();

qDebug()<<"year Active traffic"<<year;
    QDateTime Activetariffstartdate=WaterMeterParserView::Mintues_Days_Hours_Mintues(year,month,minutes);

    // Format the date and time as "yyyy-MM-dd hh:mm:ss AP"
    QString formattedDateTime = Activetariffstartdate.toString("yyyy-MM-dd hh:mm");

    ui->dat_Activetariffstartdate_13->setDateTime(Activetariffstartdate);
    ui->dat_Activetariffstartdate_13->setDisplayFormat("yyyy-MM-dd hh:mm");

  //OtherTariffID 7
    ui->T_OtherTariffID_13->setText(fileData->FieldParsedValue[7].first());

}
/**
         * @brief Implementation a function to Displays parsed data for (Client Card-(Output (From meter to backend)- (Alarm Info + Tamper Info)) according to File 0014.
         *
         * @param fileData A QSharedPointer to the parsed FileData object.
         */
void WaterMeterParserView::displayFile0014Data(QSharedPointer<FileData> fileData) {
    if(!fileData || fileData->FieldParsedValue.empty()){
        qDebug()<<"Error: fileData or FieldParsedValue is empty.";
        return;
    }

    // Last occurrence date 0
    if(!ui->table_Lastoccurrencedate_14){
        qDebug()<<"Error: table_Lastoccurrencedate_14 is not initialized";
        return;
    }
    if (fileData->FieldParsedValue.size() > 0) {
        QStringList Lastoccurrencedate = QStringList::fromVector(fileData->FieldParsedValue[0]);
        if (ui->table_Lastoccurrencedate_14) {
            for (int i = 0; i < Lastoccurrencedate.size(); ++i) {
                QString formattedDate = formatDateTime(Lastoccurrencedate[i]);
                QTableWidgetItem* item = new QTableWidgetItem(formattedDate);
                if (item) {
                    ui->table_Lastoccurrencedate_14->setItem(0, i, item);
                }
            }
        }
    }

    // Last clearance date 1
    if(!ui->table_Lastclearancedate_14){
        qDebug()<<"Error: table_Lastclearancedate_14 is not initialized";
        return;
    }
    if (fileData->FieldParsedValue.size() > 1) {
        QStringList Lastclearancedate = QStringList::fromVector(fileData->FieldParsedValue[1]);
        if (ui->table_Lastclearancedate_14) {
            for (int i = 0; i <Lastclearancedate.size(); ++i) {
                QString formattedDate = formatDateTime(Lastclearancedate[i]);
                QTableWidgetItem* item = new QTableWidgetItem(formattedDate);
                if (item) {
                    ui->table_Lastclearancedate_14->setItem(0, i, item);
                }
            }
        }
    }

    // Tamper count 2
    if(!ui->table_Tampercount_14){
        qDebug()<<"Error: table_Tampercount_14 is not initialized";
        return;
    }
    if (fileData->FieldParsedValue.size() > 2) {
        QStringList Tampercount = QStringList::fromVector(fileData->FieldParsedValue[2]);
        if (ui->table_Tampercount_14) {
            for (int i = 0; i < Tampercount.size(); ++i) {
                QTableWidgetItem* item = new QTableWidgetItem(Tampercount[i]);
                if (item) {
                    ui->table_Tampercount_14->setItem(0, i, item);
                }
            }
        }
    }

    // Tamper Last occurrence date 3
    if(!ui->table_TamperLastoccurrencedate_14){
        qDebug()<<"Error: table_TamperLastoccurrencedate_14 is not initialized";
        return;
    }
    if (fileData->FieldParsedValue.size() > 3) {
        QStringList TamperLastoccurrencedate = QStringList::fromVector(fileData->FieldParsedValue[3]);
        if (ui->table_TamperLastoccurrencedate_14) {
            for (int i = 0; i < qMin(TamperLastoccurrencedate.size(), ui->table_TamperLastoccurrencedate_14->columnCount()); ++i) {
                QString formattedDate = formatDateTime(TamperLastoccurrencedate[i]);
                QTableWidgetItem* item = new QTableWidgetItem(formattedDate);
                if (item) {
                    ui->table_TamperLastoccurrencedate_14->setItem(0, i, item);
                }
            }
        }
    }

    // Valve history 4
    if (fileData->FieldParsedValue.size() > 4 && !fileData->FieldParsedValue[4].isEmpty()) {
        QString valveStatus = fileData->FieldParsedValue[4][0];
        int valveStatusInt = valveStatus.toInt();

            QString valveStatusStr;
            switch (valveStatusInt) {
                case 0:
                    valveStatusStr = "Open Valve";
                    break;
                case 1:
                    valveStatusStr = "Close Valve";
                    break;
                case 2:
                    valveStatusStr = "Error Valve";
                    break;
                default:
                    valveStatusStr = "Unknown Valve Status :"+ QString::number(valveStatusInt);
                    break;
            }
            ui->T_ValveHistory_14->setText(valveStatusStr);

    }

    // Tamper Last occurrence date 3
    if(!ui->table_TamperLastclearancedate_14){
        qDebug()<<"Error: table_TamperLastclearancedate_14 is not initialized";
        return;
    }
        if (fileData->FieldParsedValue.size() > 5) {
            QStringList TamperLastclearancedate = QStringList::fromVector(fileData->FieldParsedValue[5]);
            if (ui->table_TamperLastclearancedate_14) {
                for (int i = 0; i < qMin(TamperLastclearancedate.size(), ui->table_TamperLastclearancedate_14->columnCount()); ++i) {
                    QString formattedDate = formatDateTime(TamperLastclearancedate[i]);
                    QTableWidgetItem* item = new QTableWidgetItem(formattedDate);
                    if (item) {
                        ui->table_TamperLastclearancedate_14->setItem(0, i, item);
                    }
                }
            }
        }

}
/**
         * @brief Implementation a function to Displays parsed data for (Configuration Card- Output (From meter to backend)-  (Meter Report) (Multi Objects))  according to File 0015.
         *
         * @param fileData A QSharedPointer to the parsed FileData object.
         */
void WaterMeterParserView::displayFile0015Data(QSharedPointer<FileData> fileData) {
    if(!fileData || fileData->FieldParsedValue.empty()){
        qDebug()<<"Error: fileData or FieldParsedValue is empty.";
        return;
    }
        // LastMeterTransactionDate
        // Process Byte 1:LastMeterTransactionDate(Year) 0
        // Process Byte 2: LastMeterTransactionDate (Month) 1
        // Process Bytes 3-4: LastMeterTransactionDate (Min) 2



        // Extract the values
        int year =  fileData->FieldParsedValue[0].first().toInt()+2000;
        int month =fileData->FieldParsedValue[1].first().toInt();
        int minutes = fileData->FieldParsedValue[2].first().toInt();


        // Combine date and time into a QDateTime object
        QDateTime LastMeterTransactionDate=WaterMeterParserView:: Mintues_Days_Hours_Mintues(year,month,minutes);

        // Format the date and time as "yyyy-MM-dd hh:mm:ss AP"
        QString formattedDateTime = LastMeterTransactionDate.toString("yyyy-MM-dd hh:mm");

        ui->dateTime_LastMeterTransactionDate_15->setDateTime(LastMeterTransactionDate);
        ui->dateTime_LastMeterTransactionDate_15->setDisplayFormat("yyyy-MM-dd hh:mm");

        // Returend Meter Actions 3

        if (!fileData->FieldParsedValue[3].isEmpty()) {
            QVector<QString> MeterActionsFlagVector = fileData->FieldParsedValue[3];
            if (!MeterActionsFlagVector.isEmpty()) {
                QString MeterActionsFlagStr = MeterActionsFlagVector.first();
                int MeterActionsFlag = MeterActionsFlagStr.toInt();

                    for (int i = 7; i >= 0; --i) {
                        if (MeterActionsFlag & (1 << i)) { // Check if the i-th bit is set
                            int MeterNumber = 8 - i; // Calculate the MeterActions
                            switch (MeterNumber) {
                                case 1: ui->ReturnedMeterActions_chBox1_15->setChecked(true); break;
                                case 2: ui->ReturnedMeterActions_chBox2_15->setChecked(true); break;
                                case 3: ui->ReturnedMeterActions_chBox3_15->setChecked(true); break;
                                case 4: ui->ReturnedMeterActions_chBox4_15->setChecked(true); break;
                                case 5: ui->ReturnedMeterActions_chBox5_15->setChecked(true); break;
                                case 6: ui->ReturnedMeterActions_chBox6_15->setChecked(true); break;
                                case 7: ui->ReturnedMeterActions_chBox7_15->setChecked(true); break;
                                case 8: ui->ReturnedMeterActions_chBox8_15->setChecked(true); break;

                                default: break;
                            }
                        }

                }
            }
        }

        //error code  4
        if (fileData->FieldParsedValue[4].first().toInt()==1){
        ui->T_ErrorCode_15 ->setText("Success");}
        else {
            ui->T_ErrorCode_15 ->setText("UnSuccess");

        }

     //used or not 5
            if (fileData->FieldParsedValue[5].first().toInt()==1){
            ui->T_notused_15 ->setText("Used");}
            else {
                ui->T_notused_15 ->setText("Not Used");

            }



}
/**
         * @brief Implementation a function to Displays parsed data for (Retrieval Card -Output (From meter to backend)- 6 (Retrieve Report File 1) (Multi Objects)) according to File 0016.
         *
         * @param fileData A QSharedPointer to the parsed FileData object.
         */
void WaterMeterParserView::displayFile0016Data(QSharedPointer<FileData> fileData){

    if(!fileData || fileData->FieldParsedValue.empty()){
        qDebug()<<"Error: fileData or FieldParsedValue is empty.";
        return;
    }
    //15
    // LastMeterTransactionDate
    // Process Byte 1:LastMeterTransactionDate(Year) 0
    // Process Byte 2: LastMeterTransactionDate (Month) 1
    // Process Bytes 3-4: LastMeterTransactionDate (Min) 2

    // Extract the values
    int year =     fileData->FieldParsedValue[0].first().toInt()+2000;
    int month =   fileData->FieldParsedValue[1].first().toInt();
    int minutes = fileData->FieldParsedValue[2].first().toInt();

    // Combine date and time into a QDateTime object
    QDateTime LastMeterTransactionDate=WaterMeterParserView:: Mintues_Days_Hours_Mintues(year,month,minutes);

    // Format the date and time as "yyyy-MM-dd hh:mm"
    QString formattedDateTime = LastMeterTransactionDate.toString("yyyy-MM-dd hh:mm");

    ui->dateTime_LastMeterTransactionDate_16->setDateTime(LastMeterTransactionDate);
    ui->dateTime_LastMeterTransactionDate_16->setDisplayFormat("yyyy-MM-dd hh:mm");

    // Returend Meter Actions 2

    if (!fileData->FieldParsedValue[3].isEmpty()) {
        QVector<QString> MeterActionsFlagVector = fileData->FieldParsedValue[3];
        if (!MeterActionsFlagVector.isEmpty()) {
            QString MeterActionsFlagStr = MeterActionsFlagVector.first();
            int MeterActionsFlag = MeterActionsFlagStr.toInt();
                for (int i = 7; i >= 0; --i) {
                    if (MeterActionsFlag & (1 << i)) { // Check if the i-th bit is set
                        int MeterNumber = 8 - i; // Calculate the MeterActions
                        switch (MeterNumber) {
                            case 1: ui->ReturnedMeterActions_chBox1_16->setChecked(true); break;
                            case 2: ui->ReturnedMeterActions_chBox2_16->setChecked(true); break;
                            case 3: ui->ReturnedMeterActions_chBox3_16->setChecked(true); break;
                            case 4: ui->ReturnedMeterActions_chBox4_16->setChecked(true); break;
                            case 5: ui->ReturnedMeterActions_chBox5_16->setChecked(true); break;
                            case 6: ui->ReturnedMeterActions_chBox6_16->setChecked(true); break;
                            case 7: ui->ReturnedMeterActions_chBox7_16->setChecked(true); break;
                            case 8: ui->ReturnedMeterActions_chBox8_16->setChecked(true); break;

                            default: break;
                        }
                    }

            }
        }
    }

   // Subscription Number 4
    ui->T_SubscriptionNumber_16->setText(fileData->FieldParsedValue[4].first());

    // Service point category ID 5
      ui->T_Service_point_category_id_16->setText(fileData->FieldParsedValue[5].first());

      // Service point sub-category ID 6
      ui->T_Service_point_activity_id_16->setText(fileData->FieldParsedValue[6].first());
      //  Service cut flag 7
      if (!fileData->FieldParsedValue[7].isEmpty()) {
          int Servicecutflag = fileData->FieldParsedValue[7].first().toInt();
          if(Servicecutflag==0)
          ui->radioB_Service_cut_flag_Pre_Paid_16 ->setChecked(true);
          else {
          ui->radioB_Service_cut_flag_Post_Paid_16->setChecked(true);

          }
      }

    //  Number of units 8
      ui->T_Number_of_Units_16 ->setText(fileData->FieldParsedValue[8].first());
    //  Sewage service flag 9

      if (!fileData->FieldParsedValue[9].isEmpty()) {
          int SewageServiceflag = fileData->FieldParsedValue[9].first().toInt();
          if(SewageServiceflag==1)
          ui->radioB_Sewage_service_flag_service_covered_16 ->setChecked(true);
          else {
          ui->radioB_Sewage_service_flag_service_not_covered_16->setChecked(true);

          }
      }



      //  Charge ID 10
          ui->T_ChargeID_16 ->setText(QStringList::fromVector(fileData->FieldParsedValue[10]).first());
    //  Charge amount 11
          ui->T_ChargeAmount_16->setText(QString::number(fileData->FieldParsedValue[11].first().toDouble()/1000)+" .LE");
      //Replacement Charge ID 12
      ui->T_ReplacementChargeID_16->setText(fileData->FieldParsedValue[12].first());
      //Replacement   Charge amount 13
      ui->T_ReplacementChargeAmount_16->setText(QString::number(fileData->FieldParsedValue[13].first().toDouble()/1000)+" .LE");

      // Low credit limit Value 14
          ui->T_LowCreditLimitValue_16 ->setText(QString::number(fileData->FieldParsedValue[14].first().toDouble()/1000)+" .LE");
      //  Low credit percentage value 15
      ui->T_Lowcreditpercentagevalue_16->setText(fileData->FieldParsedValue[15].first());
     //  Low credit limit option 16
      ui->T_Lowcreditlimitoption_16->setText(fileData->FieldParsedValue[16].first());


      //  Deduction ID 17
      ui->T_DeductionID_16->setText(fileData->FieldParsedValue[17].first());
   //  Number of cycles 18
      ui->T_Numberofcycles_16->setText(fileData->FieldParsedValue[18].first());
   //  Deducted value 19
          ui->T_Deductedvalue_16 ->setText(QString::number(fileData->FieldParsedValue[19].first().toDouble()/1000)+" .LE");
  //  Deduction start cycle 20-21

      // Process Byte : Deduction start cycle Date (Year) 20
      // Process Byte : Deduction start cycle Date (Month) 21

      // Extract the values
           year =  fileData->FieldParsedValue[20].first().toInt()+2000;
           month = fileData->FieldParsedValue[21].first().toInt();

             // Create a QDate object
          QDate date(year, month, 1); // Default to the first day of the month

          // Combine date  into a QDateTime object
          QDateTime DeductionstartcycleDate(date);

          // Format the date and time as "yyyy-MM-dd hh:mm:ss AP"
           formattedDateTime = DeductionstartcycleDate.toString("yyyy-MM-dd");


      ui->dat_DeductionstartcycleDate_16->setDateTime(DeductionstartcycleDate);
      ui->dat_DeductionstartcycleDate_16->setDisplayFormat("yyyy-MM-dd");

     //  Reading 22

      ui->T_Reading_16->setText(fileData->FieldParsedValue[22].first());

      //  Monthly consumption 23

      ui->T_CurrentMonthReading_16->setText(fileData->FieldParsedValue[23].first());

     //  Overdraft consumption 24
      if(!ui->table_Monthlyconsumption_16){
          qDebug()<<"Error: table_Monthlyconsumption_16 is not initialized";
          return;
      }
      QStringList Monthlyconsumption = QStringList::fromVector(fileData->FieldParsedValue[24]);
      for (int i = 0; i < qMin(Monthlyconsumption.size(), ui->table_Monthlyconsumption_16->columnCount()); ++i) {
          ui->table_Monthlyconsumption_16->setItem(0, i,
              new QTableWidgetItem(Monthlyconsumption[i]));
      }

      ui->T_Overdraftconsumption_16->setText(fileData->FieldParsedValue[24].first());

      // Leakage Consumption 25
       ui->T_LeakageConsumption_16->setText(fileData->FieldParsedValue[25].first());

       // Tariff ID 26
           ui->T_TariffID_16->setText(fileData->FieldParsedValue[26].first());
       // Monthly fixed fees 27
           ui->T_Month_Fixed_Fees_16->setText(QString::number( fileData->FieldParsedValue[27].first().toDouble()/100)+".LE");
       //  Monthly fixed fees per unit 28
           ui->T_Month_Fixed_Fees_Per_Unit_16->setText(QString::number(fileData->FieldParsedValue[28].first().toDouble()/100)+" .LE");
       //  Monthly fixed step fees 29
           if(!ui->table_Month_Fixed_Step_Fees_16){
               qDebug()<<"Error: table_Month_Fixed_Step_Fees_16 is not initialized";
               return;
           }
           QStringList stepFees = QStringList::fromVector(fileData->FieldParsedValue[29]);
           for (int i = 0; i < stepFees.size(); ++i) {
               ui->table_Month_Fixed_Step_Fees_16->setItem(0, i,
                   new QTableWidgetItem(QString::number((stepFees[i].toInt()/100)) + " .LE"));
           }

         //  Monthly fixed fees per cubic meter 30
           ui->T_Month_Fixed_Fees_per_cubic_meter_16 ->setText(QString::number(fileData->FieldParsedValue[30].first().toDouble()/100)+" .LE");

        //   Monthly fixed fees options 31

           if (!fileData->FieldParsedValue[31].isEmpty()) {
               QStringList MonthlyfixedfeesoptionsList = QStringList::fromVector(fileData->FieldParsedValue[31]);
               bool ok;
           int binaryNumber = MonthlyfixedfeesoptionsList.first().toInt(&ok);
   qDebug()<<"MonthlyfixedfeesoptionsList"<<MonthlyfixedfeesoptionsList.first();
           if (ok) {

               ui->Month_Fixed_Fees_Chbox_16->setChecked((binaryNumber & 0b10000000)!= 0);
               ui->Month_Fixed_Fees_per_Unit_ChBox_16->setChecked((binaryNumber & 0b01000000)!= 0);
               ui->Month_Fixed_Steps_ChBox_16->setChecked((binaryNumber & 0b00100000)!= 0);
               ui->Month_Fixed_Fees_per_Cubic_Meter_ChBox_16->setChecked((binaryNumber & 0b00010000)!= 0);
               if(binaryNumber!=0) {
                   qDebug()<<"MonthlyfixedfeesoptionsList is out of rang binaryNumber & 0b10000000 : "<<(binaryNumber & 0b10000000);
                   qDebug()<<"MonthlyfixedfeesoptionsList is out of rang it is a : "<<(binaryNumber & 0b01000000);
                   qDebug()<<"MonthlyfixedfeesoptionsList is out of rang it is a : "<<(binaryNumber & 0b00100000);
                   qDebug()<<"MonthlyfixedfeesoptionsList is out of rang it is a : "<<(binaryNumber & 0b00010000);

                   qDebug()<<"MonthlyfixedfeesoptionsList is out of rang it is a : "<<binaryNumber;
               }
                   else{
                       qDebug()<<"MonthlyfixedfeesoptionsList is a : "<<binaryNumber;

                   }


           }
           else{
               qDebug()<<"MonthlyfixedfeesoptionsList is out of range"<<binaryNumber;

           }

       }

         //  Accumulated tier flag 32

           if (!fileData->FieldParsedValue[32].isEmpty()) {
               QVector<QString> tierFlagVector = fileData->FieldParsedValue[32];
               if (!tierFlagVector.isEmpty()) {
                   QString tierFlagStr = tierFlagVector.first();
                   int tierFlag = tierFlagStr.toInt();

                          for (int i = 15; i >= 0; --i) {
                           if (tierFlag & (1 << i)) { // Check if the i-th bit is set
                               int tierNumber = 16 - i; // Calculate the tier number
                               switch (tierNumber) {
                                   case 1: ui->Tier_chBox1_16->setChecked(true); break;
                                   case 2: ui->Tier_chBox2_16->setChecked(true); break;
                                   case 3: ui->Tier_chBox3_16->setChecked(true); break;
                                   case 4: ui->Tier_chBox4_16->setChecked(true); break;
                                   case 5: ui->Tier_chBox5_16->setChecked(true); break;
                                   case 6: ui->Tier_chBox6_16->setChecked(true); break;
                                   case 7: ui->Tier_chBox7_16->setChecked(true); break;
                                   case 8: ui->Tier_chBox8_16->setChecked(true); break;
                                   case 9: ui->Tier_chBox9_16->setChecked(true); break;
                                   case 10: ui->Tier_chBox10_16->setChecked(true); break;
                                   case 11: ui->Tier_chBox11_16->setChecked(true); break;
                                   case 12: ui->Tier_chBox12_16->setChecked(true); break;
                                   case 13: ui->Tier_chBox13_16->setChecked(true); break;
                                   case 14: ui->Tier_chBox14_16->setChecked(true); break;
                                   case 15: ui->Tier_chBox15_16->setChecked(true); break;
                                   case 16: ui->Tier_chBox16_16->setChecked(true); break;
                                   default: break;
                               }

                       }
                   }
               }
           }


         //  Consumption price 33
           if(!ui->table_Consumption_Price_16){
               qDebug()<<"Error: table_Consumption_Price_16 is not initialized";
               return;
           }
           QStringList prices = QStringList::fromVector(fileData->FieldParsedValue[33]);
           for (int i = 0; i < prices.size(); ++i) {
               ui->table_Consumption_Price_16->setItem(0, i,
                   new QTableWidgetItem(QString::number(prices[i].toDouble()/100) + " .LE"));
           }


         //  Max consumption per tier 34
           if(!ui->table_Max_Consumption_Per_Tier_16){
               qDebug()<<"Error: table_Max_Consumption_Per_Tier_16 is not initialized";
               return;
           }
           QStringList pertier = QStringList::fromVector(fileData->FieldParsedValue[34]);
           for (int i = 0; i < pertier.size(); ++i) {
               ui->table_Max_Consumption_Per_Tier_16->setItem(
               0, i,
                   new QTableWidgetItem(QString::number(pertier[i].toDouble()) + " .m³"));
           }

         //  Sewage price Percentage 35
           ui->T_Sewage_Percentage_16 ->setText(fileData->FieldParsedValue[35].first()+" %");

           //  Sewage price  36

           ui->T_Sewage_Price_16 ->setText(QString::number(fileData->FieldParsedValue[36].first().toDouble()/100)+" .LE");

           // Sewage Start Date 38
           // Process Byte 120: Start date (Year) 37
           // Process Byte 121: Start date (Month) 38
           // Process Bytes 122-123: Start date (Min) 39


           // Extract the values
            year =  fileData->FieldParsedValue[37].first().toInt()+2000;
            month = fileData->FieldParsedValue[38].first().toInt();
            minutes =  fileData->FieldParsedValue[39].first().toInt();


            QDateTime datetimex = WaterMeterParserView:: Mintues_Days_Hours_Mintues(year,month,minutes);

           // Format the date and time as "yyyy-MM-dd"
            formattedDateTime = datetimex.toString("yyyy-MM-dd hh:mm");

           ui->DE_Sewage_Start_Date_16->setDateTime(datetimex);
           ui->DE_Sewage_Start_Date_16->setDisplayFormat("yyyy-MM-dd hh:mm");


          //  Units tiers table flag 40

           if (!fileData->FieldParsedValue[40].isEmpty()) {
               int Unitstierstableflag = fileData->FieldParsedValue[40].first().toInt();
               if(Unitstierstableflag==1){
               ui->radioB_Unit_Tiers_Table_flag_on_16->setChecked(true);
               }
               else {
                   ui->radioB_Unit_Tiers_Table_flag_off_16->setChecked(true);
               }

           }
         //  Friendly Hours ID 41
           ui->T_Friendly_Hours_ID_16 ->setText(fileData->FieldParsedValue[41].first());

        //   Workdays 42

           if (!fileData->FieldParsedValue[42].isEmpty()) {
               QString workdaysString = fileData->FieldParsedValue[42].first();
                   // Split the string by comma and trim spaces
                   QStringList workdaysList = workdaysString.split(", ", QString::SkipEmptyParts);

                   for(int i=0;i<workdaysList.size();i++){
                       if(workdaysList[i]=="Sun")
                     ui->Workdays_sunday_chBox_16->setChecked(true);
                       if(workdaysList[i]=="Mon")
                     ui->Workdays_monday_chBox_16->setChecked(true);
                       if(workdaysList[i]=="Thu")
                     ui->Workdays_tuesday_chBox_16->setChecked(true);
                       if(workdaysList[i]=="Wed")
                     ui->Workdays_wensday_chBox_16->setChecked(true);
                       if(workdaysList[i]=="Thu")
                     ui->Workdays_thursday_chBox_16->setChecked(true);
                       if(workdaysList[i]=="Fri")
                     ui->Workdays_frieday_chBox_16->setChecked(true);
                       if(workdaysList[i]=="Sat")
                     ui->Workdays_saturday_chBox_16->setChecked(true);

                 }

                 }

         //  Start working hour 43
           QStringList startHourList = QStringList::fromVector(fileData->FieldParsedValue[43]);
           if (!startHourList.isEmpty()) {

           for(const QString& timeStr : startHourList) {
               // Split the time string by ":" and get the hour
                   QString hour = timeStr.split(":").first();
                   int hourNum = hour.toInt();

                   if(timeStr.contains("AM", Qt::CaseInsensitive)) {
                       // Use a switch statement to set the appropriate radio button
                       switch(hourNum) {
                       case 1:ui->radioB_Start_Working_Hour_AM__1_16->setChecked(true); break;
                       case 2:ui->radioB_Start_Working_Hour_AM__2_16->setChecked(true); break;
                       case 3:ui->radioB_Start_Working_Hour_AM__3_16->setChecked(true); break;
                       case 4:ui->radioB_Start_Working_Hour_AM__4_16->setChecked(true); break;
                       case 5:ui->radioB_Start_Working_Hour_AM__5_16->setChecked(true); break;
                       case 6:ui->radioB_Start_Working_Hour_AM__6_16->setChecked(true); break;
                       case 7:ui->radioB_Start_Working_Hour_AM__7_16->setChecked(true); break;
                       case 8:ui->radioB_Start_Working_Hour_AM__8_16->setChecked(true); break;
                       case 9:ui->radioB_Start_Working_Hour_AM__9_16->setChecked(true); break;
                       case 10:ui->radioB_Start_Working_Hour_AM__10_16->setChecked(true);break;
                       case 11:ui->radioB_Start_Working_Hour_AM__11_16->setChecked(true);break;
                       case 12:ui->radioB_Start_Working_Hour_AM__12_16->setChecked(true);break;

                       }
                   } else if(timeStr.contains("PM", Qt::CaseInsensitive)) {
                       switch(hourNum) {
                       case 1:ui->radioB_Start_Working_Hour_PM__1_16->setChecked(true); break;
                       case 2:ui->radioB_Start_Working_Hour_PM__2_16->setChecked(true); break;
                       case 3:ui->radioB_Start_Working_Hour_PM__3_16->setChecked(true); break;
                       case 4:ui->radioB_Start_Working_Hour_PM__4_16->setChecked(true); break;
                       case 5:ui->radioB_Start_Working_Hour_PM__5_16->setChecked(true); break;
                       case 6:ui->radioB_Start_Working_Hour_PM__6_16->setChecked(true); break;
                       case 7:ui->radioB_Start_Working_Hour_PM__7_16->setChecked(true); break;
                       case 8:ui->radioB_Start_Working_Hour_PM__8_16->setChecked(true); break;
                       case 9:ui->radioB_Start_Working_Hour_PM__9_16->setChecked(true); break;
                       case 10:ui->radioB_Start_Working_Hour_PM__10_16->setChecked(true);break;
                       case 11:ui->radioB_Start_Working_Hour_PM__11_16->setChecked(true);break;
                       case 12:ui->radioB_Start_Working_Hour_PM__12_16->setChecked(true);break;


                   }
                       }



           }
       }


         //  End working hour 44
           QStringList endHourList = QStringList::fromVector(fileData->FieldParsedValue[44]);
           if (!endHourList.isEmpty()) {
               for(const QString& timeStr : endHourList) {
                   // Split the time string by ":" and get the hour
                       QString hour = timeStr.split(":").first();
                       int hourNum = hour.toInt();

                       if(timeStr.contains("AM", Qt::CaseInsensitive)) {
                           // Use a switch statement to set the appropriate radio button
                           switch(hourNum) {
                               case 1:ui->radioB_End_Working_Hour_AM__1_16->setChecked(true); break;
                               case 2:ui->radioB_End_Working_Hour_AM__2_16->setChecked(true); break;
                               case 3:ui->radioB_End_Working_Hour_AM__3_16->setChecked(true); break;
                               case 4:ui->radioB_End_Working_Hour_AM__4_16->setChecked(true); break;
                               case 5:ui->radioB_End_Working_Hour_AM__5_16->setChecked(true); break;
                               case 6:ui->radioB_End_Working_Hour_AM__6_16->setChecked(true); break;
                               case 7:ui->radioB_End_Working_Hour_AM__7_16->setChecked(true); break;
                               case 8:ui->radioB_End_Working_Hour_AM__8_16->setChecked(true); break;
                               case 9:ui->radioB_End_Working_Hour_AM__9_16->setChecked(true); break;
                               case 10:ui->radioB_End_Working_Hour_AM__10_16->setChecked(true);break;
                               case 11:ui->radioB_End_Working_Hour_AM__11_16->setChecked(true);break;
                               case 12:ui->radioB_End_Working_Hour_AM__12_16->setChecked(true);break;
                           }
                       } else if(timeStr.contains("PM", Qt::CaseInsensitive)) {
                           switch(hourNum) {
                           case 1:ui->radioB_End_Working_Hour_PM__1_16->setChecked(true); break;
                           case 2:ui->radioB_End_Working_Hour_PM__2_16->setChecked(true); break;
                           case 3:ui->radioB_End_Working_Hour_PM__3_16->setChecked(true); break;
                           case 4:ui->radioB_End_Working_Hour_PM__4_16->setChecked(true); break;
                           case 5:ui->radioB_End_Working_Hour_PM__5_16->setChecked(true); break;
                           case 6:ui->radioB_End_Working_Hour_PM__6_16->setChecked(true); break;
                           case 7:ui->radioB_End_Working_Hour_PM__7_16->setChecked(true); break;
                           case 8:ui->radioB_End_Working_Hour_PM__8_16->setChecked(true); break;
                           case 9:ui->radioB_End_Working_Hour_PM__9_16->setChecked(true); break;
                           case 10:ui->radioB_End_Working_Hour_PM__10_16->setChecked(true);break;
                           case 11:ui->radioB_End_Working_Hour_PM__11_16->setChecked(true);break;
                           case 12:ui->radioB_End_Working_Hour_PM__12_16->setChecked(true);break;

                           }
                       }


               }

           }
         //  Overdraft amount of time 45
           ui->T_Overdraft_amount_of_Time_16 ->setText(fileData->FieldParsedValue[45].first()+" hours");

         //  Overdraft amount of money 46
           ui->T_Overdraft_amount_of_Money_16 ->setText(QString::number( fileData->FieldParsedValue[46].first().toDouble()/1000.0)+" .LE");


            //  Holiday month 47
            //  Holiday day 48

           QVector<QString> months = fileData->FieldParsedValue[47];
           QVector<QString> days = fileData->FieldParsedValue[48];


              // Array of month names
              QStringList monthNames = {
                  "January", "February", "March", "April", "May", "June",
                  "July", "August", "September", "October", "November", "December"
              };

              // Create formatted holiday list
              QStringList holidayList;
              holidayList.append("Holiday Month : Day");  // Header


              // Process each month-day pair
              for (int i = 0; i < months.size(); ++i) {
                  bool monthOk, dayOk;
                  int monthNumber = months[i].toInt(&monthOk);
                  int dayNumber = days[i].toInt(&dayOk);

                  // Only process valid month-day pairs
                  if (monthOk && dayOk &&
                      monthNumber >= 1 && monthNumber <= 12 &&
                      dayNumber > 0 && dayNumber <= 31) {

                      QString monthName = monthNames[monthNumber - 1];
                      QString formattedHoliday = QString("%1 - %2")
                          .arg(monthName)
                          .arg(dayNumber);

                      // Only add unique entries
                      if (!holidayList.contains(formattedHoliday)) {
                          holidayList.append(formattedHoliday);
                      }
                  }
              }



              // Sort the holidays by month and day
              auto sortByMonthDay = [&monthNames](const QString& a, const QString& b) {
                  if (a == "Holiday Month : Day") return true;  // Keep header at top
                  if (b == "Holiday Month : Day") return false;

                  QStringList partsA = a.split(" - ");
                  QStringList partsB = b.split(" - ");

                  if (partsA.size() != 2 || partsB.size() != 2) return false;

                  int monthIndexA = monthNames.indexOf(partsA[0]);
                  int monthIndexB = monthNames.indexOf(partsB[0]);

                  if (monthIndexA != monthIndexB) {
                      return monthIndexA < monthIndexB;
                  }

                  return partsA[1].toInt() < partsB[1].toInt();
              };

              std::sort(holidayList.begin() + 1, holidayList.end(), sortByMonthDay);

              // Set the formatted text to the UI
              if (ui->T_Holiday_month_day_16) {
                  ui->T_Holiday_month_day_16->setText(holidayList.join("\n"));
              }

              // Credit balance 49
                 ui->T_Creditbalance_16->setText(QString::number(fileData->FieldParsedValue[49].first().toDouble() / 1000) + " LTE");

                // Overdraft credit 50
                 ui->T_Overdraftcredit_16->setText(QString::number(fileData->FieldParsedValue[50].first().toDouble() / 1000) + " LTE");

                // Consumed credit 51
                 ui->T_Consumedcredit_16->setText(QString::number(fileData->FieldParsedValue[51].first().toDouble() / 1000) + " LTE");

                // Consumed credit monthly 52
                 if(!ui->table_Consumedcreditmonthly_16){
                     qDebug()<<"Error: table_Consumedcreditmonthly_16 is not initialized";
                     return;
                 }
                 QStringList ConsumedCreditMonthly = QStringList::fromVector(fileData->FieldParsedValue[52]);
                 for (int i = 0; i < qMin(ConsumedCreditMonthly.size(), ui->table_Consumedcreditmonthly_16->columnCount()); ++i) {
                     ui->table_Consumedcreditmonthly_16->setItem(0, i,
                         new QTableWidgetItem(ConsumedCreditMonthly[i]));
                 }

                // Cumulative charge 53
                 ui->T_Cumulativecharge_16->setText(QString::number(fileData->FieldParsedValue[53].first().toDouble() / 1000) + " LTE");

                // Active Tariff ID 54
                 ui->T_ActiveTariffID_16->setText(fileData->FieldParsedValue[54].first());

                // Active Tariff start date 55-57
                // ActivetariffstartdateYear
                // ActivetariffstartdateMon
               //  ActivetariffstartdateMin


                 // Get the parsed values from the model


                 // Extract the values
                  year =     fileData->FieldParsedValue[55].first().toInt()+2000;
                  month =    fileData->FieldParsedValue[56].first().toInt();
                  minutes =  fileData->FieldParsedValue[57].first().toInt();


                 QDateTime Activetariffstartdate=WaterMeterParserView::Mintues_Days_Hours_Mintues(year,month,minutes);

                 // Format the date and time as "yyyy-MM-dd hh:mm"
                  formattedDateTime = Activetariffstartdate.toString("yyyy-MM-dd hh:mm");

                 ui->dat_Activetariffstartdate_16->setDateTime(Activetariffstartdate);
                 ui->dat_Activetariffstartdate_16->setDisplayFormat("yyyy-MM-dd hh:mm");

                // Other Tariff ID 58
                 ui->T_OtherTariffID_16->setText(fileData->FieldParsedValue[58].first());

                 // Last occurrence date 59
                 if(!ui->table_Lastoccurrencedate_16){
                     qDebug()<<"Error: table_Lastoccurrencedate_16 is not initialized";
                     return;
                 }
                     if (fileData->FieldParsedValue.size() > 0) {
                         QStringList Lastoccurrencedate = QStringList::fromVector(fileData->FieldParsedValue[59]);
                         if (ui->table_Lastoccurrencedate_16) {
                             for (int i = 0; i < Lastoccurrencedate.size(); ++i) {
                                 QString formattedDate = formatDateTime(Lastoccurrencedate[i]);
                                 QTableWidgetItem* item = new QTableWidgetItem(formattedDate);
                                 if (item) {
                                     ui->table_Lastoccurrencedate_16->setItem(0, i, item);
                                 }
                             }
                         }
                     }

                    // Last clearance date 60
                     if(!ui->table_Lastclearancedate_16){
                         qDebug()<<"Error: table_Lastclearancedate_16 is not initialized";
                         return;
                     }
                     if (fileData->FieldParsedValue.size() > 1) {
                         QStringList Lastclearancedate = QStringList::fromVector(fileData->FieldParsedValue[60]);
                         if (ui->table_Lastclearancedate_16) {
                             for (int i = 0; i <Lastclearancedate.size(); ++i) {
                                 QString formattedDate = formatDateTime(Lastclearancedate[i]);
                                 QTableWidgetItem* item = new QTableWidgetItem(formattedDate);
                                 if (item) {
                                     ui->table_Lastclearancedate_16->setItem(0, i, item);
                                 }
                             }
                         }
                     }

                    // Tamper count 61
                     if(!ui->table_Tampercount_16){
                         qDebug()<<"Error: table_Tampercount_16 is not initialized";
                         return;
                     }
                     if (fileData->FieldParsedValue.size() > 2) {
                         QStringList Tampercount = QStringList::fromVector(fileData->FieldParsedValue[61]);
                         if (ui->table_Tampercount_16) {
                             for (int i = 0; i < Tampercount.size(); ++i) {
                                 QTableWidgetItem* item = new QTableWidgetItem(Tampercount[i]);
                                 if (item) {
                                     ui->table_Tampercount_16->setItem(0, i, item);
                                 }
                             }
                         }
                     }

                    // Last occurrence date 62
                     if(!ui->table_TamperLastoccurrencedate_16){
                         qDebug()<<"Error: table_TamperLastoccurrencedate_16 is not initialized";
                         return;
                     }
                     if (fileData->FieldParsedValue.size() > 3) {
                         QStringList TamperLastoccurrencedate = QStringList::fromVector(fileData->FieldParsedValue[62]);
                         if (ui->table_TamperLastoccurrencedate_16) {
                             for (int i = 0; i < qMin(TamperLastoccurrencedate.size(), ui->table_TamperLastoccurrencedate_16->columnCount()); ++i) {
                                 QString formattedDate = formatDateTime(TamperLastoccurrencedate[i]);
                                 QTableWidgetItem* item = new QTableWidgetItem(formattedDate);
                                 if (item) {
                                     ui->table_TamperLastoccurrencedate_16->setItem(0, i, item);
                                 }
                             }
                         }
                     }

                     //Valve history 63
                     if (fileData->FieldParsedValue.size() > 4 && !fileData->FieldParsedValue[63].isEmpty()) {
                         QString valveStatus = fileData->FieldParsedValue[63][0];
                         int valveStatusInt = valveStatus.toInt();

                             QString valveStatusStr;
                             switch (valveStatusInt) {
                                 case 0:
                                     valveStatusStr = "Open Valve";
                                     break;
                                 case 1:
                                     valveStatusStr = "Close Valve";
                                     break;
                                 case 2:
                                     valveStatusStr = "Error Valve";
                                     break;
                                 default:
                                     valveStatusStr = "Unknown Valve Status: "+ QString::number(valveStatusInt);
                                     break;
                             }
                             ui->T_ValveHistory_16->setText(valveStatusStr);

                     }

    //Current Month Reading 64
ui->T_CurrentMonthReading_16->setText(QString::number(fileData->FieldParsedValue[65].first().toDouble()));

    //error code  65
    if (fileData->FieldParsedValue[65].first().toInt()==1){
    ui->T_ErrorCode_16 ->setText("Success");}
    else {
        ui->T_ErrorCode_16 ->setText("UnSuccess");

    }

 //used or not 66
        if (fileData->FieldParsedValue[66].first().toInt()==1){
        ui->T_notused_16 ->setText("Used");}
        else {
            ui->T_notused_16 ->setText("Not Used");

        }
}
