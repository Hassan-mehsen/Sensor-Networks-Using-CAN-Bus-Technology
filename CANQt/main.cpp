#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "waitingConnection_window.h"
#include <QApplication>
#include <QDialog>

int main(int argc, char *argv[])
{

/*
    QApplication a(argc, argv);
    MainWindow w;
    w.openCANPort();
    WaitingConnectionWindow waitingWindow;
    waitingWindow.checkCANConnection() ;
    waitingWindow.exec();
    w.receiveCANMessage();
    w.show();
    return a.exec();
*/


    QApplication a(argc, argv);


    WaitingConnectionWindow waitingWindow;
    int result = waitingWindow.exec();

    if (result == QDialog::Accepted)
    {

        MainWindow w;
        w.show();
        return a.exec();
    }
    else
    {
        // If the connection failed, exit the application
        return 0;
    }



}
