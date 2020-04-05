#include "maingamewidget.h"

MainGameWidget::MainGameWidget(GameWindow* gameWindow) : QWidget(gameWindow)
{
    this->gameWindow = gameWindow;
    this->updateWorker = new UpdateWorker();

    this->SetupUI();

    this->manager = new QNetworkAccessManager();
    connect(this->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleHttpRequest(QNetworkReply*)));

    connect(this->clickerButton, SIGNAL (clicked()), this, SLOT (handleClick()));

    scoreValueLabel->setText("0");
    frameValueLabel->setText("0");

    this->RefreshProgressBars(0);

    this->lastRefreshedFrame = -1;

    connect(gameWindow->ServerWorker(), SIGNAL(RefreshGameData(QJsonObject)),this, SLOT(refreshData(QJsonObject)));
    gameWindow->ServerWorker()->RequestOrder(ServerWorker::Order::OrderStartGameplayRefresh);

    connect(this->updateWorker, &UpdateWorker::Update, this, &MainGameWidget::Update);
}

MainGameWidget::~MainGameWidget()
{

    disconnect(this->gameWindow->ServerWorker(), SIGNAL(RefreshGameData(QJsonObject)),this, SLOT(refreshData(QJsonObject)));

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


    QSpacerItem* spacer = new QSpacerItem(1,1,QSizePolicy::Fixed, QSizePolicy::Expanding);
    this->gameWindow->LeftLayout->addItem(spacer);

    this->frameValueLabel = new QLabel(this);
    this->gameWindow->LeftLayout->addWidget(this->frameValueLabel);

    this->clickValueLabel = new QLabel(this);
    vBoxLayout->addWidget(this->clickValueLabel);

    this->ProgressBar[0] = new ScaledProgressBar(1000, this);
    vBoxLayout->addWidget(this->ProgressBar[0]);

    for(int index = 1; index < 4; ++index)
    {
        float scale = pow(10, 3 + 2 * index);

        this->ProgressBar[index] = new ScaledProgressBar(scale, this);
       // this->ProgressBar[index]->setTextVisible(false);

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

    this->realCurrentScore = score;

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

    for(int index = 0; index < numberOfbars; ++index)
    {
        this->ProgressBar[index]->SetScaledValue(displayedScore);
    }
}

double damping(double pos, double target, double k,double dt)
{
    return pos + ((target - pos) / k * dt);
}

void MainGameWidget::Update(int dt)
{
    if(this->displayedScore == this->realCurrentScore)
    {
        return;
    }

    else if(abs(this->displayedScore - this->realCurrentScore) <= 10)
    {
        this->displayedScore = this->realCurrentScore;
    }
    else
    {
        double damp = 8;
        if(this->displayedScore > this->realCurrentScore)
        {
            damp = 4;
        }

        this->displayedScore = damping(this->displayedScore, this->realCurrentScore, damp, 1);
    }

    this->RefreshProgressBars(this->displayedScore);
}
