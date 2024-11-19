#include "waitingConnection_window.h"
#include "socketcan_cpp.h"
#include <QPixmap>

// Constructeur
WaitingConnectionWindow::WaitingConnectionWindow(QWidget *parent) : QDialog(parent), ui(new Ui::WaitingConnectionWindow), progressValue(0), isConnected(false), isWelcomeScreen(true)
{
    ui->setupUi(this);  // Configurer l'interface utilisateur
    setWindowTitle("Waiting for CAN Connection");
    ui->top_label->setText("Welcome There !");
    QPixmap welcomeIcon(":/icons/hi.png");
    ui->icon_label->setPixmap(welcomeIcon.scaled(120,120, Qt::KeepAspectRatio));
    ui->icon_label->setScaledContents(true);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &WaitingConnectionWindow::updateProgress);
    timer->start(500); // en ms
}

// Destructeur
WaitingConnectionWindow::~WaitingConnectionWindow()
{
    delete timer;
    delete ui;
}

void WaitingConnectionWindow::checkCANConnection()
{
    // Ouverture du port CAN avec les mêmes paramètres que dans openCANPort
    if (canSocket.open("can0") == scpp::STATUS_OK)
    {
        // CAN connecté avec succès
        printf("CAN socket opened\n");

        isConnected = true;
        ui->top_label->setText("Connected");

        // Mise à jour de l'icône de succès
        QPixmap successIcon(":/icons/success.png");
        ui->icon_label->setPixmap(successIcon.scaled(60, 60, Qt::KeepAspectRatio));
        ui->icon_label->setScaledContents(true);
    }
    else
    {
        // Échec de la connexion CAN
        printf("Cannot open CAN socket!\n");

        isConnected = false;
        ui->top_label->setText("Not Connected");

        // Mise à jour de l'icône d'échec
        QPixmap failureIcon(":/icons/failure.png");
        ui->icon_label->setPixmap(failureIcon.scaled(60, 60, Qt::KeepAspectRatio));
        ui->icon_label->setScaledContents(true);
    }
}

void WaitingConnectionWindow::updateProgress()
{
    if (isWelcomeScreen) {
        // Attend 3 à 5 secondes dans cette état
        if (progressValue < 20) {
            progressValue += 2;
        }

        // Passer à la phase de connexion après 3-5 secondes
        if (progressValue >= 20) {
            isWelcomeScreen = false;  // Passer à la phase de connexion
            progressValue = 20;       // Fixer la progression à 20%
            checkCANConnection();
        }
    } else {
        // Toujours vérifier la connexion si non connecté
        if (!isConnected)
        {
            checkCANConnection();  // Vérifier la connexion à chaque appel
        }
        else {
            // Accélérer la progression si connecté
            if (progressValue < 100) {
                progressValue += 5;  // Incrémenter plus rapidement si connecté
            }
            if (progressValue >= 100) {
                timer->stop();  // Arrêter le timer
                accept();       // Fermer le dialogue
            }
        }
    }

    // Limiter la valeur de progression à 100%
    if (progressValue > 100) {
        progressValue = 100;
    }

    // Mettre à jour la barre de progression
    ui->progressBar->setValue(progressValue);
}

