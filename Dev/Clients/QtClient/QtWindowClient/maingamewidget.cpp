#include "maingamewidget.h"

MainGameWidget::MainGameWidget(QWidget* parent, QApplication* application) : QWidget(parent)
{
    this->SetupUI();

    this->manager = new QNetworkAccessManager();
    QObject::connect(this->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleHttpRequest(QNetworkReply*)));

    QObject::connect(this->clickerButton, SIGNAL (clicked()), this, SLOT (handleClick()));

    scoreValueLabel->setText("0");
    frameValueLabel->setText("0");

    this->RefreshProgressBars(0);

    this->lastRefreshedFrame = -1;

    this->refreshWorker = new RefresherWorker();
    QObject::connect(this->refreshWorker, SIGNAL(refreshData(QJsonObject)), this, SLOT(refreshData(QJsonObject)));
    this->workerThread = new QThread();
    this->refreshWorker->moveToThread(this->workerThread);

    connect(this->workerThread, SIGNAL(started()), this->refreshWorker, SLOT(run()));
    this->workerThread->start();

    connect(application, SIGNAL(aboutToQuit()), this, SLOT(aboutToQuit()));

}

MainGameWidget::~MainGameWidget()
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
}

void MainGameWidget::SetupUI()
{
    QVBoxLayout* vBoxLayout = new QVBoxLayout(this);
    vBoxLayout->setMargin(15);

    this->scoreValueLabel = new QLabel(this);
    vBoxLayout->addWidget(this->scoreValueLabel);

    this->frameValueLabel = new QLabel(this);
    vBoxLayout->addWidget(this->frameValueLabel);

    this->clickValueLabel = new QLabel(this);
    vBoxLayout->addWidget(this->clickValueLabel);

    for(int index = 0; index < 4; ++index)
    {
        this->ProgressBar[index] = new QProgressBar(this);
        vBoxLayout->addWidget(this->ProgressBar[index]);
    }
    this->UpgradeLayout = new QGridLayout();
    vBoxLayout->addLayout(this->UpgradeLayout);

    this->clickerButton = new QPushButton("Click", this);
    vBoxLayout->addWidget(this->clickerButton);
}

void MainGameWidget::handleClick()
{
    request.setUrl(
                QUrl(
                    QString::fromStdString(AutoClicker::BaseURI() + "click=true")
                    )
                );
    manager->get(request);
}

void MainGameWidget::handleHttpRequest(QNetworkReply* reply)
{
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }
}

void MainGameWidget::refreshData(QJsonObject jsonData)
{
    int score = jsonData["Score"].toInt();
    int frameCount = jsonData["FrameCount"].toInt();
    int targetScore = jsonData["TargetScore"].toInt();
    int clickValue = jsonData["ClickValue"].toInt();
    int passiveSpeed = jsonData["PassiveSpeed"].toInt();
    int globalFactor = jsonData["GlobalFactor"].toInt();

    QString scoreLabel = QString("Score %1 / %2 (Passive speed : %3 * %4)").arg(QString::number(score), QString::number(targetScore), QString::number(passiveSpeed), QString::number(globalFactor));
    this->scoreValueLabel->setText(scoreLabel);
    this->frameValueLabel->setText(QString::number(frameCount));
    this->clickValueLabel->setText("+" + QString::number(clickValue) + " bytes");

    int progress = static_cast<int>((static_cast<double>(score) / static_cast<double>(targetScore)) * 100.0);
    if(progress > 100)
    {
        progress = 100;
    }

    this->RefreshProgressBars(score);

    QJsonArray jsonUpgrades = jsonData["Upgrades"].toArray();
    if(static_cast<int>(this->UpgradeButtons.size()) != jsonUpgrades.size())
    {
        this->UpgradeButtons.clear();
        for(int index = 0; index < jsonUpgrades.size(); ++index)
        {
            QJsonObject jsonUpgrade = jsonUpgrades[index].toObject();

            QString name = jsonUpgrade["Name"].toString();
            QString description = jsonUpgrade["Description"].toString();
            int price = jsonUpgrade["Price"].toInt();
            int instanceBought = jsonUpgrade["NumberOfInstanceBought"].toInt();

            Upgrade* upgrade = new Upgrade(index , name, price, instanceBought);

            UpgradeButton* upgradeButton = new UpgradeButton(upgrade, this);
            upgradeButton->setToolTip(description);

            int NumberColumn = 3;
            int col = index / NumberColumn;
            int row = index % NumberColumn;
            this->UpgradeLayout->addWidget(upgradeButton, col, row);

            connect(upgradeButton, &QPushButton::clicked, [=](){ emit this->UpgradeButtonClick(upgradeButton);});

            this->UpgradeButtons.push_back(upgradeButton);
        }

        this->isDirty = false;
    }
    else if(this->isDirty && frameCount > this->lastRefreshedFrame)
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
        this->lastRefreshedFrame = frameCount;
    }
    else
    {
        for(int index = 0; index < jsonUpgrades.size(); ++index)
        {
            UpgradeButton* upgradeButton = this->UpgradeButtons[static_cast<unsigned long long>(index)];
            int failureFlags = jsonUpgrades[index].toObject()["FailureFlags"].toInt();
            upgradeButton->setEnabled(failureFlags == 0);
        }
    }
}

void MainGameWidget::UpgradeButtonClick(UpgradeButton* upgradeButton)
{
    qDebug() << upgradeButton->Upgrade->Name;
    upgradeButton->Upgrade->IsDirty = true;
    this->isDirty = true;
    this->request.setUrl(QString::fromStdString(AutoClicker::BaseURI() + "upgrade=" + std::to_string((upgradeButton->Upgrade->Index))));
    this->manager->get(this->request);
}

void MainGameWidget::aboutToQuit()
{
    this->request.setUrl(QUrl(QString::fromStdString( AutoClicker::BaseURI() + "set_update_pause=true")));
    manager->get(request);
    qDebug() << "About to quit called.";
    // TODO : find a way to enable one last request before quitting without waiting an arbitrary length of time.
    QThread::sleep(1);
}

void MainGameWidget::RefreshProgressBars(int score)
{
    int numberOfbars = 4;
    if(score >= 1000000000)
    {
        for(int index = 0; index < numberOfbars; ++index)
        {
            this->ProgressBar[index]->setValue(100);
        }

        return;
    }

    int firstBarScore = (score % 1000) / 10;
    this->ProgressBar[0]->setValue(firstBarScore);
    for(int index = 1; index < numberOfbars; ++index)
    {
        double order = pow(10,3 + 2*index);
        double denominator = pow(10,1 + 2 * index);
        int barScore = score % static_cast<int>(order) / static_cast<int>(denominator);

        this->ProgressBar[index]->setValue(barScore);
    }
}
