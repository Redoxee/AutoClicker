#include "maingamewidget.h"
#include "SWIUtils.h"

MainGameWidget::MainGameWidget(GameWindow* gameWindow) : QWidget(gameWindow)
{
    this->gameWindow = gameWindow;
    this->updateWorker = new UpdateWorker();

    this->scoreHistory = new int[this->historySize];
    for(int i = 0; i < this->historySize; ++i)
    {
        this->scoreHistory[i] = 0;
    }

    this->historyCursor = 0;

    this->SetupUI();

    this->manager = new QNetworkAccessManager();
    connect(this->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleHttpRequest(ServerGameplayState*)));

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

    this->frameValueLabel = new QLabel(this);
    this->gameWindow->LeftLayout->addWidget(this->frameValueLabel);

    this->clickValueLabel = new QLabel(this);
    vBoxLayout->addWidget(this->clickValueLabel);


    QVBoxLayout* progressLayout = new QVBoxLayout();
    progressLayout->setMargin(0);

    this->ProgressBar[0] = new ScaledProgressBar(1000, this);
    progressLayout->addWidget(this->ProgressBar[0]);

    for(int index = 1; index < 4; ++index)
    {
        float scale = pow(10, 3 + 2 * index);

        this->ProgressBar[index] = new ScaledProgressBar(scale, this);

        progressLayout->addWidget(this->ProgressBar[index]);
    }

    this->historySeries = new QtCharts::QLineSeries();
    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->legend()->hide();
    chart->addSeries(this->historySeries);
    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    this->historyYAxis = new QtCharts::QLogValueAxis();
    this->historyYAxis->setMin(1);
    this->historyYAxis->setLabelFormat("%g");
    this->historyYAxis->setBase(10.0);
    this->historyYAxis->setMinorTickCount(0);

    chart->addAxis(this->historyYAxis, Qt::AlignLeft);
    this->historySeries->attachAxis(this->historyYAxis);
    this->historyChartView = new QtCharts::QChartView(chart);
    this->historyChartView->setRenderHint(QPainter::Antialiasing);
    this->historyChartView->setMinimumHeight(220);
    this->historyChartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    vBoxLayout->addWidget(this->historyChartView);

    vBoxLayout->addLayout(progressLayout);

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

void MainGameWidget::refreshData(ServerGameplayState* serverData)
{
    QString scoreLabel = QString("Score %1 / %2 \n(Passive speed : %3 * %4)").arg(QString::number(serverData->Score), QString::number(serverData->TargetScore), FormatDownQuantity(serverData->PassiveSpeed), FormatDownQuantity(serverData->GlobalFactor));
    this->scoreValueLabel->setText(scoreLabel);
    this->frameValueLabel->setText(QString::number(serverData->FrameCount));
    this->clickValueLabel->setText("+" + FormatDownQuantity(serverData->ClickValue) + " bytes");

    this->realCurrentScore = serverData->Score;
    if(serverData->NumberOfUpgrades != this->UpgradeButtons.size())
    {
        if(serverData->NumberOfUpgrades > this->UpgradeButtons.size())
        {
            int buttonToAdd = serverData->NumberOfUpgrades -this->UpgradeButtons.size();
            for(int i = 0; i < buttonToAdd; ++i)
            {
                UpgradeButton* btn = new UpgradeButton();
                this->UpgradeButtons.push_back(btn);
            }
        }
        else
        {
            serverData
        }
    }

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

    this->pushToScoreHistory(score);
    this->refreshHistory();
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

void MainGameWidget::pushToScoreHistory(int score)
{
    this->scoreHistory[this->historyCursor++] = score;
    this->historyCursor = this->historyCursor % this->historySize;
}

void MainGameWidget::refreshHistory()
{
    this->historySeries->clear();
    int maxValue = 100;

    for(int i = 0; i < this->historySize; ++i)
    {
        int index = (this->historyCursor + i) % this->historySize;
        int score = this->scoreHistory[index];
        if(score < 1)
        {
            score = 1;
        }

        float xPos = static_cast<float>(i) / static_cast<float>(this->historySize);
        this->historySeries->append(xPos, score);
        if(score > maxValue)
        {
            maxValue = score;
        }
    }

    this->historyYAxis->setMax(maxValue * 1.1);
}
