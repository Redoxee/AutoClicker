#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>

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
    for(auto current = this->UpgradeButtons.begin(); current != this->UpgradeButtons.end(); current++)
    {
        delete (*current)->Upgrade;
    }
    this->UpgradeButtons.clear();

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
    if(static_cast<int>(this->UpgradeButtons.size()) != jsonUpgrades.size())
    {
        this->UpgradeButtons.clear();
        for(int index = 0; index < jsonUpgrades.size(); ++index)
        {
            QJsonObject jsonUpgrade = jsonUpgrades[index].toObject();

            QString name = jsonUpgrade["Name"].toString();
            int price = jsonUpgrade["Price"].toInt();
            int instanceBought = jsonUpgrade["NumberOfInstanceBought"].toInt();

            Upgrade* upgrade = new Upgrade(index , name, price, instanceBought);

            UpgradeButton* upgradeButton = new UpgradeButton(upgrade, this);
            this->UpgradeLayout->addWidget(upgradeButton);
            connect(upgradeButton, &QPushButton::clicked, [=](){ emit this->UpgradeButtonClick(upgradeButton);});

            this->UpgradeButtons.push_back(upgradeButton);
        }

        this->isDirty = false;
    }
    else if(this->isDirty)
    {
        for(int index = 0; index < jsonUpgrades.size(); ++index)
        {
            UpgradeButton* upgradeButton = this->UpgradeButtons[static_cast<unsigned long long>(index)];
            if(upgradeButton->Upgrade->IsDirty)
            {
                QJsonObject jsonUpgrade = jsonUpgrades[index].toObject();

                QString name = jsonUpgrade["Name"].toString();
                int price = jsonUpgrade["Price"].toInt();
                int instanceBought = jsonUpgrade["NumberOfInstanceBought"].toInt();

                upgradeButton->Upgrade->Name = name;
                upgradeButton->Upgrade->Price = price;
                upgradeButton->Upgrade->InstanceBought = instanceBought;
                upgradeButton->Upgrade->IsDirty = false;
                upgradeButton->RefreshLabel();
            }
        }

        this->isDirty = false;
    }
}

void MainWindow::UpgradeButtonClick(UpgradeButton* upgradeButton)
{
    qDebug() << upgradeButton->Upgrade->Name;
    upgradeButton->Upgrade->IsDirty = true;
    this->isDirty = true;
    this->request.setUrl(QString::fromStdString(AutoClicker::BaseURI() + "upgrade=" + std::to_string((upgradeButton->Upgrade->Index))));
    this->manager->get(this->request);
}

void MainWindow::aboutToQuit()
{
    this->request.setUrl(QUrl("http://localhost:1234/AutoClicker?set_update_pause=true"));
    manager->get(request);
    qDebug() << "About to quit called.";
    // TODO : find a way to enable one last request before quitting without waiting an arbitrary length of time.
    QThread::sleep(1);
}
