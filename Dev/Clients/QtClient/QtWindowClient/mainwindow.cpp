#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->ui->setupUi(this);
    this->setWindowTitle("Auto Clicker");
    this->scoreValueLabel = findChild<QLabel*>("ScoreValue");
    this->targetScoreLabel = findChild<QLabel*>("ScoreTarget");
    this->tickValueLabel = findChild<QLabel*>("TickValue");
    this->clickValueLabel = findChild<QLabel*>("ClickValue");

    this->manager = new QNetworkAccessManager();
    QObject::connect(this->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleHttpRequest(QNetworkReply*)));

    QPushButton* clickerButton = findChild<QPushButton*>("ClickerButton");
    QObject::connect(clickerButton, SIGNAL (released()), this, SLOT (handleClick()));

    scoreValueLabel->setText("0");
    tickValueLabel->setText("0");

    this->refreshWorker = new RefresherWorker();
    QObject::connect(this->refreshWorker, SIGNAL(refreshData(QJsonObject)), this, SLOT(refreshData(QJsonObject)));
    this->workerThread = new QThread();
    this->refreshWorker->moveToThread(this->workerThread);

    connect(this->workerThread, SIGNAL(started()), this->refreshWorker, SLOT(run()));
    this->workerThread->start();
}

MainWindow::~MainWindow()
{
    this->refreshWorker->stop();
    delete ui;
}

void MainWindow::handleClick()
{
    request.setUrl(QUrl("http://localhost:1234/AutoClicker?Click=True"));
    manager->get(request);
}

void MainWindow::handleHttpRequest(QNetworkReply* reply)
{
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }
}

void MainWindow::refreshData(QJsonObject jsonData)
{
    int score = jsonData["Score"].toInt();
    int tickCount = jsonData["TickCount"].toInt();
    int targetScore = jsonData["TargetScore"].toInt();
    int clickValue = jsonData["ClickValue"].toInt();

    this->scoreValueLabel->setText(QString::number(score));
    this->tickValueLabel->setText(QString::number(tickCount));
    this->targetScoreLabel->setText("/" + QString::number(targetScore));
    this->clickValueLabel->setText("+"+QString::number(clickValue) + " coins");
}

