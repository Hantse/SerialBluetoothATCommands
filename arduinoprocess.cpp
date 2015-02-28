#include "arduinoprocess.h"
#include "ui_arduinoprocess.h"

ArduinoProcess::ArduinoProcess(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::ArduinoProcess)
{
    ui->setupUi(this);
}

ArduinoProcess::~ArduinoProcess()
{
    delete ui;
}
