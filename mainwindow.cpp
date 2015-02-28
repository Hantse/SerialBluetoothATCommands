#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>
#include "arduinoprocess.h"
#include <QtCore/QTime>
#include <QtDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set Baude Rate For Arduino Communication
    ui->cbBaudRate->addItem("1200", QSerialPort::Baud1200);
    ui->cbBaudRate->addItem("2400", QSerialPort::Baud2400);
    ui->cbBaudRate->addItem("4800", QSerialPort::Baud4800);
    ui->cbBaudRate->addItem("9600", QSerialPort::Baud9600);
    ui->cbBaudRate->addItem("19200", QSerialPort::Baud19200);
    ui->cbBaudRate->addItem("38400", QSerialPort::Baud38400);
    ui->cbBaudRate->addItem("57600", QSerialPort::Baud57600);
    ui->cbBaudRate->addItem("115200", QSerialPort::Baud115200);
    ui->cbBaudRate->setCurrentIndex(3);

    ui->cbBaudRateSerial->addItem("1200", 1);
    ui->cbBaudRateSerial->addItem("2400", 2);
    ui->cbBaudRateSerial->addItem("4800", 3);
    ui->cbBaudRateSerial->addItem("9600", 4);
    ui->cbBaudRateSerial->addItem("19200", 5);
    ui->cbBaudRateSerial->addItem("38400", 6);
    ui->cbBaudRateSerial->addItem("57600", 7);
    ui->cbBaudRateSerial->addItem("115200", 8);
    ui->cbBaudRateSerial->addItem("230400", 9);
    ui->cbBaudRateSerial->addItem("460800", 'A');
    ui->cbBaudRateSerial->addItem("921600", 'B');
    ui->cbBaudRateSerial->addItem("1382400", 'C');

    ui->cbBaudRateSerial->setCurrentIndex(3);

    // Get all available port
    Refresh();

    // Main ui slot & signal
    QObject::connect(ui->btRefresh, SIGNAL(clicked()), this, SLOT(Refresh()));
    QObject::connect(ui->btConnect, SIGNAL(clicked()), this, SLOT(Connect()));
    QObject::connect(ui->btDisconnect, SIGNAL(clicked()), this, SLOT(Disconnect()));
    QObject::connect(ui->actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
    QObject::connect(ui->actionArduino, SIGNAL(triggered()), this, SLOT(OpenArduinoProcess()));

    // Name slot & signal
    QObject::connect(ui->btSetName, SIGNAL(clicked()), this, SLOT(SetName()));
    QObject::connect(ui->btClearName, SIGNAL(clicked()), this, SLOT(ClearName()));
    QObject::connect(ui->btSetPin, SIGNAL(clicked()), this, SLOT(SetPin()));
    QObject::connect(ui->btClearPin, SIGNAL(clicked()), this, SLOT(ClearPin()));
    QObject::connect(ui->btSetBaudRate, SIGNAL(clicked()), this, SLOT(SetBaudRate()));
    QObject::connect(ui->btVersion, SIGNAL(clicked()), this, SLOT(GetVersion()));

    ui->txtPin->setValidator(new QIntValidator(0, 9999, this));
}

void MainWindow::Refresh()
{
    // Get all available port
    ComPorts = QSerialPortInfo::availablePorts();

    // Suppress all items on combobox
    ui->cbPortsCom->clear();

    if(ComPorts.count() > 0){
        // Set all ports in Combobox
        foreach(SelectdPort, ComPorts)
        {
            ui->cbPortsCom->addItem(SelectdPort.portName() + " " + SelectdPort.manufacturer() + "(" + SelectdPort.isBusy() + ")");
        }

        ui->btConnect->setEnabled(true);
    }else{
        ui->btConnect->setEnabled(false);
    }
}

void MainWindow::Connect()
{
    SerialUse = new QSerialPort(this);
    QObject::connect(SerialUse, SIGNAL(readyRead()), this, SLOT(ReceiveData()));
    SerialUse->setPortName((ComPorts[ui->cbPortsCom->currentIndex()].portName()));
    SerialUse->setBaudRate(ui->cbBaudRate->itemData(ui->cbBaudRate->currentIndex()).toInt());
    SerialUse->setParity(QSerialPort::UnknownParity);
    SerialUse->setDataBits(QSerialPort::Data8);
    SerialUse->setStopBits(QSerialPort::OneStop);
    SerialUse->setFlowControl(QSerialPort::NoFlowControl);

    if(SerialUse->open(QIODevice::ReadWrite)){
        ToggleState();
        ui->statusBar->showMessage("Connected On Serial " + (ComPorts[ui->cbPortsCom->currentIndex()].portName()));
    }else {
        QMessageBox::critical(this, tr("Error"), SerialUse->errorString());
    }
}

void MainWindow::Disconnect()
{
    SerialUse->close();
    ui->taReception->clear();
    ToggleState();
    ui->statusBar->showMessage("Disconnected On Serial " + (ComPorts[ui->cbPortsCom->currentIndex()].portName()));
    QMessageBox::information(this, tr("Successfull !"), "Successfull disconnected !");
}

// Set HC method
void MainWindow::SetName(){
    QString Command = "AT+NAME" + ui->txtName->text();
    SendData(Command.toLatin1());
}

void MainWindow::SetPin(){
    QString Command = "AT+PIN" + ui->txtPin->text();
    SendData(Command.toLatin1());
}

void MainWindow::SetBaudRate(){
    QString Command = "AT+BAUD" + ui->cbBaudRateSerial->itemData(ui->cbBaudRateSerial->currentIndex()).toString();
    SendData(Command.toLatin1());
}

// Clear HC method
void MainWindow::ClearName(){
    ui->txtName->clear();
}

void MainWindow::ClearPin(){
    ui->txtPin->clear();
}

void MainWindow::GetVersion(){
    QString Command = "AT+VERSION";
    SendData(Command.toLatin1());
}


void MainWindow::SendData(const QByteArray &data)
{
    SerialUse->write(data);
}

void MainWindow::ReceiveData()
{
    if(SerialUse->isReadable()){

        QString Result(SerialUse->readAll());
        qDebug() << Result;

        if(Result == "OKsetname"){
            ui->taReception->append("New name : " + ui->txtName->text());
        }else if(Result == "OKsetPIN"){
            ui->taReception->append("New pin : " + ui->txtPin->text());
        }else{}

    }else{
        QMessageBox::critical(this, tr("Error"), SerialUse->errorString());
    }
}

void MainWindow::ToggleState(){
    ui->gbMainContent->setEnabled(!ui->gbMainContent->isEnabled());
    ui->btConnect->setEnabled(!ui->btConnect->isEnabled());
    ui->cbBaudRate->setEnabled(!ui->cbBaudRate->isEnabled());
    ui->cbPortsCom->setEnabled(!ui->cbPortsCom->isEnabled());
    ui->btRefresh->setEnabled(!ui->btRefresh->isEnabled());   
}

void MainWindow::OpenArduinoProcess(){
    ArduinoProcess ArduinoProcessWizard;
    ArduinoProcessWizard.exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}
