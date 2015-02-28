#ifndef ARDUINOPROCESS_H
#define ARDUINOPROCESS_H

#include <QWizard>

namespace Ui {
class ArduinoProcess;
}

class ArduinoProcess : public QWizard
{
    Q_OBJECT

public:
    explicit ArduinoProcess(QWidget *parent = 0);
    ~ArduinoProcess();

private:
    Ui::ArduinoProcess *ui;
};

#endif // ARDUINOPROCESS_H
