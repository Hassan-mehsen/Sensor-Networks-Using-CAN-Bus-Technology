#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QDebug>
#include <QGroupBox>
#include <QGraphicsView>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QTimer>
#include <QGridLayout>
#include <QString>
#include <iostream>
#include <QtCore>
#include <QtOpenGL>
#include <QProgressBar>

#include <socketcan_cpp.h>
#include <objectgl.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void openCANPort();
    void sendCANMessage();
    void receiveCANMessage();

public slots:
    void mot_button();
    void db_button();
    void onButSendClicked();
    void onTimer_Tick();
    void onTimer_TickSend();
    void updateHumidityProgress(int humidity);

private:
    scpp::SocketCan socket_can;  // Socket CAN

    Ui::MainWindow *ui;           // UI pointer

    // Layouts
    QGridLayout *temp_layout;
    QGridLayout *spw_layout;
    QGridLayout *hum_layout;
    QGridLayout *press_layout;
    QGridLayout *mot_layout;
    QGridLayout *db_layout;
    QGridLayout *principal_layout;
    QGridLayout *angle0_layout;
    QGridLayout *angle1_layout;
    QGridLayout *angle2_layout;

    // Group boxes
    QGroupBox *hum_box;
    QGroupBox *temp_box;
    QGroupBox *spw_box;
    QGroupBox *press_box;
    QGroupBox *mot_box;
    QGroupBox *db_box;
    QGroupBox *angle0_box;
    QGroupBox *angle1_box;
    QGroupBox *angle2_box;

    // Input fields
    QLineEdit *temp_line;
    QLineEdit *spw_line;
    QLineEdit *press_line;
    QLineEdit *db_line;
    QLineEdit *angle0_line;
    QLineEdit *angle1_line;
    QLineEdit *angle2_line;

    // Radio buttons
    QRadioButton *d_RadioButton;
    QRadioButton *b_RadioButton;

    // Buttons
    QPushButton *reset;
    QPushButton *start;
    QPushButton *onOff;
    QPushButton *but_send;

    // Labels
    QLabel *percentage;
    QLabel *lc;
    QLabel *MPascal;
    QLabel *deg;
    QLabel *speed;

    // Progress Bar
    QProgressBar *hum_progress ;

    // Graphics view
    QGraphicsView *graph;

    // Timer
    QTimer *timer_tick;
    QTimer *timer_tickSend;

    // Central widget
    QWidget *centralWidget;

    // OpenGL object
    ObjectOpenGL *Object_GL;
};

#endif // MAINWINDOW_H
