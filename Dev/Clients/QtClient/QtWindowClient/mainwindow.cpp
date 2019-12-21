#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    manager = new QNetworkAccessManager();
        QObject::connect(manager, SIGNAL(finished(QNetworkReply*)),
            this,SLOT(handleHttpRequest(QNetworkReply*))
        );

    QPushButton* clickerButton = findChild<QPushButton*>("ClickerButton");
    QObject::connect(clickerButton, SIGNAL (released()), this, SLOT (handleClick()));

    scoreValueLabel = findChild<QLabel*>("ScoreValue");
    tickValueLabel = findChild<QLabel*>("TickValue");

    scoreValueLabel->setText("0");
    tickValueLabel->setText("0");

    this->refreshWorker = new RefresherWorker();
    QObject::connect(this->refreshWorker, SIGNAL(refreshData(int)), this, SLOT(refreshData(int)));
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

void MainWindow::handleHttpRequest(QNetworkReply *reply)
{
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }

    QString answer = reply->readAll();

    qDebug() << answer;

    QJsonDocument jsonDocumnet = QJsonDocument::fromJson(answer.toUtf8());
    QJsonObject jsonObject = jsonDocumnet.object();
    int score = jsonObject["Score"].toInt();
    int tickCount = jsonObject["TickCount"].toInt();

    scoreValueLabel->setText(QString::number(score));
    tickValueLabel->setText(QString::number(tickCount));
}

void MainWindow::refreshData(int data)
{
    scoreValueLabel->setText(QString::number(data));
}

