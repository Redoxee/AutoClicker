#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QJsonArray>

MainWindow::MainWindow(QApplication* application, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->ui->setupUi(this);
    this->setWindowTitle("Auto Clicker");
    this->scoreValueLabel = this->findChild<QLabel*>("ScoreValue");
    this->targetScoreLabel = this->findChild<QLabel*>("ScoreTarget");
    this->tickValueLabel = this->findChild<QLabel*>("TickValue");
    this->clickValueLabel = this->findChild<QLabel*>("ClickValue");
    this->UpgradeLayout = this->findChild<QLayout*>("UpgradeButtonHolder");
    this->manager = new QNetworkAccessManager();
    QObject::connect(this->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleHttpRequest(QNetworkReply*)));

    QPushButton* clickerButton = findChild<QPushButton*>("ClickerButton");
    QObject::connect(clickerButton, SIGNAL (clicked()), this, SLOT (handleClick()));

    scoreValueLabel->setText("0");
    tickValueLabel->setText("0");

    this->refreshWorker = new RefresherWorker();
    QObject::connect(this->refreshWorker, SIGNAL(refreshData(QJsonObject)), this, SLOT(refreshData(QJsonObject)));
    this->workerThread = new QThread();
    this->refreshWorker->moveToThread(this->workerThread);

    connect(this->workerThread, SIGNAL(started()), this->refreshWorker, SLOT(run()));
    this->workerThread->start();

    connect(application, SIGNAL(aboutToQuit()), this, SLOT(aboutToQuit()));
}

MainWindow::~MainWindow()
{
    this->refreshWorker->stop();
    for(auto current = this->Upgrades.begin(); current != this->Upgrades.end(); current++)
    {
        delete *current;
    }
    this->Upgrades.clear();

    QLayoutItem *child;
    while ((child = this->UpgradeLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    delete ui;
}

void MainWindow::handleClick()
{
    request.setUrl(QUrl("http://localhost:1234/AutoClicker?click=true"));
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

    QJsonArray jsonUpgrades = jsonData["Upgrades"].toArray();
    if(static_cast<int>(this->Upgrades.size()) != jsonUpgrades.size())
    {
        this->Upgrades.clear();
        for(int index = 0; index < jsonUpgrades.size(); ++index)
        {
            QJsonObject jsonUpgrade = jsonUpgrades[index].toObject();

            QString name = jsonUpgrade["Name"].toString();
            int price = jsonUpgrade["Price"].toInt();

            Upgrade* upgrade = new Upgrade(index , name, price);

            this->Upgrades.push_back(upgrade);

            UpgradeButton* upgradeButton = new UpgradeButton(upgrade, this);
            this->UpgradeLayout->addWidget(upgradeButton);
            connect(upgradeButton, &QPushButton::clicked, [=](){ emit this->UpgradeButtonClick(upgradeButton);});
        }
    }
}

void MainWindow::UpgradeButtonClick(UpgradeButton* upgradeButton)
{
    qDebug() << upgradeButton->Upgrade->Name;
}

void MainWindow::aboutToQuit()
{
    this->request.setUrl(QUrl("http://localhost:1234/AutoClicker?set_update_pause=true"));
    manager->get(request);
    qDebug() << "About to quit called.";
    // TODO : find a way to enable one last request before quitting without waiting an arbitrary length of time.
    QThread::sleep(1);
}
