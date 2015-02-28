#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#ifdef Q_OS_WIN
#include <windows.h> // for Sleep
#endif


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void ToggleState();
    void SendData(const QByteArray &data);

public slots:
    void Connect();
    void Refresh();
    void Disconnect();

    void SetName();
    void SetPin();
    void SetBaudRate();

    void ClearName();
    void ClearPin();

    void GetVersion();

    void ReceiveData();
    void OpenArduinoProcess();

private:
    Ui::MainWindow *ui;
    QList<QSerialPortInfo> ComPorts;
    QSerialPortInfo SelectdPort;
    QSerialPort* SerialUse;

};

#endif // MAINWINDOW_H
