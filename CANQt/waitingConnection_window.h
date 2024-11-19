/*
#ifndef WAITINGCONNECTION_WINDOW_H
#define WAITINGCONNECTION_WINDOW_H

#include <QDialog>
#include <QTimer>  // Include QTimer
#include "ui_waitingConnection_window.h"

class WaitingConnectionWindow : public QDialog
{
    Q_OBJECT

public:
    explicit WaitingConnectionWindow(QWidget *parent = nullptr);
    ~WaitingConnectionWindow();

private slots:
    void updateProgress();      // Slot to update the progress bar

private:
    void checkCANConnection();  // Method to check CAN connection status

    Ui::WaitingConnectionWindow *ui;  // Pointer to the UI
    QTimer *timer;                    // Timer for progress updates
    int progressValue;                // Current progress value
    bool isConnected;                 // Flag to store CAN connection status
};

#endif // WAITINGCONNECTION_WINDOW_H
*/
#ifndef WAITINGCONNECTION_WINDOW_H
#define WAITINGCONNECTION_WINDOW_H

#include <QDialog>
#include <QTimer>  // Include QTimer
#include "ui_waitingConnection_window.h"
#include "socketcan_cpp.h"

class WaitingConnectionWindow : public QDialog
{
    Q_OBJECT

public:
    explicit WaitingConnectionWindow(QWidget *parent = nullptr);
    ~WaitingConnectionWindow();
    void checkCANConnection();  // Method to check CAN connection status


private slots:
    void updateProgress();      // Slot to update the progress bar

private:

    Ui::WaitingConnectionWindow *ui;  // Pointer to the UI
    QTimer *timer;                    // Timer for progress updates
    scpp::SocketCan canSocket;
    int progressValue;                // Current progress value
    bool isConnected;                 // Flag to store CAN connection status
    bool isWelcomeScreen;            // Flag to indicate if the welcome screen is displayed
};

#endif // WAITINGCONNECTION_WINDOW_H
