#include "maingamewidget.h"

#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QLogValueAxis>
#include <QtCharts/QChartView>
#include <QSplitter>

#include <QMenu>
#include <QAction>

#include "autoclickerconfig.h"
#include "SWIUtils.h"
#include "gamewindow.h"
#include "upgradebutton.h"
#include "updateworker.h"
#include "scaledprogressbar.h"
#include "servergameplaystate.h"
#include "serverworker.h"

using namespace SWIUtils;

MainGameWidget::MainGameWidget(GameWindow* gameWindow) : QWidget(gameWindow)
{
    this->gameWindow = gameWindow;
    this->updateWorker = new UpdateWorker();

    this->isFinished = false;
    this->isSleeping = false;

    this->scoreHistory = new int[this->historySize];
    for(int i = 0; i < this->historySize; ++i)
    {
        this->scoreHistory[i] = 0;
    }

    this->historyCursor = 0;

    this->SetupUI();

    this->manager = new QNetworkAccessManager();
    connect(this->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleHttpRequest(QNetworkReply*)));

    this->RefreshProgressBars(0);

    this->lastRefreshedFrame = -1;

    connect(gameWindow->ServerWorker(), SIGNAL(RefreshGameData(ServerGameplayState*)),this, SLOT(refreshData(ServerGameplayState*)));
    gameWindow->ServerWorker()->RequestOrder(ServerWorker::Order::OrderStartGameplayRefresh);

    connect(this->updateWorker, &UpdateWorker::Update, this, &MainGameWidget::Update);
}

MainGameWidget::~MainGameWidget()
{
    disconnect(this->gameWindow->ServerWorker(), SIGNAL(RefreshGameData(ServerGameplayState*)),this, SLOT(refreshData(ServerGameplayState*)));
}

void MainGameWidget::SetupUI()
{
    QVBoxLayout* vBoxLayout = new QVBoxLayout(this);
    vBoxLayout->setMargin(0);

    this->scoreSlot = new ScoreSlot(this);
    vBoxLayout->addWidget(this->scoreSlot);

    QSpacerItem* verticalSpacer = new QSpacerItem(1,1, QSizePolicy::Fixed, QSizePolicy::Expanding);
    vBoxLayout->addSpacerItem(verticalSpacer);

    this->frameValueLabel = new QLabel(this);
    this->gameWindow->LeftLayout->addWidget(this->frameValueLabel);

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
    this->historyYAxis->setMin(0);
    this->historyYAxis->setLabelFormat("%g");
    QFont font = this->historyYAxis->labelsFont();
    font.setPointSize(6);
    this->historyYAxis->setLabelsFont(font);
    this->historyYAxis->setBase(10.0);
    this->historyYAxis->setMinorTickCount(0);

    chart->addAxis(this->historyYAxis, Qt::AlignLeft);
    this->historySeries->attachAxis(this->historyYAxis);
    this->historyChartView = new QtCharts::QChartView(chart);
    this->historyChartView->setRenderHint(QPainter::Antialiasing, true);
    this->historyChartView->setMinimumHeight(90);
    this->historyChartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    vBoxLayout->addWidget(this->historyChartView);

    vBoxLayout->addLayout(progressLayout);

    QBoxLayout* upgradeLayout = new QVBoxLayout();
    upgradeLayout->setSpacing(0);
    upgradeLayout->setMargin(0);

    this->clickUpgradeSlot = new UpgradeSlot(this);
    this->clickUpgradeSlot->MainLabel->setText("Faster manual install");
    this->clickUpgradeSlot->SubLabel->setText("Use the current installed bits to improve the installer.");
    this->clickUpgradeSlot->InstanceBought->setText("0");
    connect(this->clickUpgradeSlot->UpgradeButtons->MainButton, &QPushButton::clicked, this, [this](){this->UpgradeButtonClick(ServerGameplayState::ClickUpgradeIndex);});

    this->firstGeneratorSlot = new UpgradeSlot(this);
    connect(this->firstGeneratorSlot->UpgradeButtons->MainButton, &QPushButton::clicked, this, [this](){this->UpgradeButtonClick(ServerGameplayState::FirstGeneratorIndex);});

    this->secondGeneratorSlot = new UpgradeSlot(this);
    connect(this->secondGeneratorSlot->UpgradeButtons->MainButton, &QPushButton::clicked, this, [this](){this->UpgradeButtonClick(ServerGameplayState::SecondGeneratorIndex);});

    upgradeLayout->addWidget(this->secondGeneratorSlot);
    upgradeLayout->addWidget(this->firstGeneratorSlot);
    upgradeLayout->addWidget(this->clickUpgradeSlot);

    vBoxLayout->addLayout(upgradeLayout);

    QHBoxLayout* clickerButtonLayout = new QHBoxLayout();
    vBoxLayout->addLayout(clickerButtonLayout);

    clickerButtonLayout->setMargin(0);
    clickerButtonLayout->setSpacing(0);

    this->clickerButton = new QPushButton("Install (+1 Bit)", this);
    clickerButtonLayout->addWidget(this->clickerButton);
    this->clickerButton->setFixedHeight(50);
    this->clickerButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    connect(this->clickerButton, &QPushButton::clicked, this, &MainGameWidget::handleClick);

    this->clickMenuButton = new QPushButton("+");
    clickerButtonLayout->addWidget(this->clickMenuButton);
    this->clickMenuButton->setFixedWidth(25);
    this->clickMenuButton->setFixedHeight(50);
    this->clickMenuButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    connect(this->clickMenuButton, &QPushButton::clicked, this,[this]()
    {
        QMenu menu;
        menu.setToolTipsVisible(true);
        QAction *a = new QAction();
        a->setText("Buy upgrade for 8888888 bits\n Test");
        a->setToolTip("Upgrade by x precent \n youhou.");
        menu.addAction(a);
        QPoint pos = this->clickMenuButton->pos() + this->gameWindow->pos();

        menu.exec(pos);
    });

    this->finishButton = new QPushButton();
    this->gameWindow->BottomBox->addWidget(finishButton);
    this->finishButton->setVisible(false);
    QSizePolicy sizePolicy = this->finishButton->sizePolicy();
    sizePolicy.setRetainSizeWhenHidden(true);
    this->finishButton->setSizePolicy(sizePolicy);
    this->finishButton->setText("Next");

    connect(this->finishButton, &QPushButton::clicked, this, &MainGameWidget::onFinishButtonClicked);
}

void MainGameWidget::handleClick()
{
    this->isDirty = true;
    qDebug("Hello");
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
    QString scoreMessage = QString("%1 bits installed").arg(QString::number(serverData->Score));

    int64_t passiveSpeed = serverData->PassiveSpeed * serverData->GlobalFactor;
    if(serverData->TempBonusDuration > 0 && serverData->TempBonusFactor > 1)
    {
        passiveSpeed *= serverData->TempBonusFactor;
    }

    QString passiveSpeedMessage = QString("Autoinstall %1 bits per second").arg(QString::number(passiveSpeed));

    this->scoreSlot->ScoreLabel->setText(scoreMessage);
    this->scoreSlot->FactorLabel->setText(passiveSpeedMessage);

    this->frameValueLabel->setText(QString::number(serverData->FrameCount));

    int clickValue = serverData->ClickValue;
    if(serverData->GlobalFactor > 1)
    {
        clickValue *= serverData->GlobalFactor;
    }

    if(serverData->TempBonusDuration > 0)
    {
        clickValue *= serverData->TempBonusFactor;
    }

    this->clickUpgradeSlot->InstanceBought->setText(QString::number(serverData->clickUpgrade->InstanceBought));
    this->clickUpgradeSlot->UpgradeButtons->MainButton->setText(serverData->clickUpgrade->GetPriceLabel());

    this->firstGeneratorSlot->InstanceBought->setText(QString::number(serverData->firstGenerator->InstanceBought));
    this->firstGeneratorSlot->UpgradeButtons->MainButton->setText(serverData->firstGenerator->GetPriceLabel());

    this->secondGeneratorSlot->InstanceBought->setText(QString::number(serverData->secondGenerator->InstanceBought));
    this->secondGeneratorSlot->UpgradeButtons->MainButton->setText(serverData->secondGenerator->GetPriceLabel());

    this->clickerButton->setText(QString("Install (+%1 Bits)").arg(clickValue));

    this->gameWindow->currentFrame = serverData->FrameCount;

    this->realCurrentScore = serverData->Score;

    this->pushToScoreHistory(serverData->Score);
    this->refreshHistory();

    if(!this->isSleeping && serverData->IsSleeping())
    {
        // TODO Display a message here to explain why the game isnt advancing.
        // Maybe a dialog box.

        this->isSleeping = true;
    }

    if(!this->isFinished && serverData->IsFinished())
    {
        this->finishButton->setVisible(true);
        this->isFinished = true;
    }
}

void MainGameWidget::UpgradeButtonClick(int buttonIndex)
{
    this->isDirty = true;
    this->request.setUrl(QString::fromStdString(AutoClicker::BaseURI() + "upgrade=" + std::to_string(buttonIndex)));
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
        if(score > maxValue)
        {
            maxValue = score;
        }
    }

    maxValue *= 1.1;
    int minValue = 1;
    if(maxValue > 1000)
    {
        minValue = maxValue / 1000;
    }

    this->historyYAxis->setMax(maxValue);
    this->historyYAxis->setMin(minValue);

    for(int i = 0; i < this->historySize; ++i)
    {
        int index = (this->historyCursor + i) % this->historySize;
        int score = this->scoreHistory[index];

        if(i > 0 && i < (this->historySize - 1))
        {
            if(score == this->scoreHistory[index - 1] && score == this->scoreHistory[index + 1])
            {
                continue;
            }
        }

        if(score < 1)
        {
            score = 1;
        }

        float xPos = static_cast<float>(i) / static_cast<float>(this->historySize);
        this->historySeries->append(xPos, score);
    }
}

void MainGameWidget::onFinishButtonClicked()
{
    this->gameWindow->BottomBox->removeWidget(this->finishButton);
    delete this->finishButton;

    this->gameWindow->GotToScreen(Screens::EndGameScreen);
}

UpgradeSlot::UpgradeSlot(QWidget* parent) : QFrame(parent)
{
    QHBoxLayout* hLayout = new QHBoxLayout(this);
    hLayout->setSpacing(0);
    hLayout->setMargin(0);
    this->setFixedHeight(50);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    this->InstanceBought = new QLabel(this);
    this->InstanceBought->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    this->InstanceBought->setFixedWidth(40);
    this->InstanceBought->setText("999");
    this->InstanceBought->setAlignment(Qt::AlignmentFlag::AlignCenter);
    QFont instanceFont = this->InstanceBought->font();
    instanceFont.setPixelSize(15);
    this->InstanceBought->setFont(instanceFont);
    hLayout->addWidget(this->InstanceBought);

    QVBoxLayout* vLayout = new QVBoxLayout();
    QSpacerItem* spacer = new QSpacerItem(0,0, QSizePolicy::Fixed, QSizePolicy::Expanding);
    vLayout->addSpacerItem(spacer);
    hLayout->addLayout(vLayout);
    this->MainLabel = new QLabel(this);
    QFont mainFont = this->MainLabel->font();
    mainFont.setPointSize(10);
    this->MainLabel->setFont(mainFont);
    vLayout->addWidget(this->MainLabel);
    this->MainLabel->setText("[TBW] Placeholder");

    this->SubLabel = new QLabel(this);
    vLayout->addWidget(this->SubLabel);
    this->SubLabel->setWordWrap(true);
    this->SubLabel->setText("[TBW] PlaceHolder");

    spacer = new QSpacerItem(0,0,QSizePolicy::Fixed, QSizePolicy::Expanding);
    vLayout->addSpacerItem(spacer);

    this->UpgradeButtons = new UpgradeButton("Aquire\n8888 bits", this);
    hLayout->addWidget(this->UpgradeButtons);

    QSplitter* splitter = new QSplitter(this);
    vLayout->addWidget(splitter);

    this->MainLayout = hLayout;
    this->setFrameStyle(QFrame::Box | QFrame::Sunken);
}


ScoreSlot::ScoreSlot(QWidget* parent): QFrame(parent)
{
    QHBoxLayout* hLayout = new QHBoxLayout(this);
    QVBoxLayout* vLayout = new QVBoxLayout(this);
    hLayout->setSpacing(0);
    hLayout->setMargin(0);

    this->ScoreLabel = new QLabel("0", this);
    this->FactorLabel = new QLabel("1", this);
    this->ScoreLabel->setAlignment(Qt::AlignRight);
    this->FactorLabel->setAlignment(Qt::AlignRight);

    QSpacerItem* horizontalSpacer = new QSpacerItem(0,0, QSizePolicy::Expanding, QSizePolicy::Fixed);
    hLayout->addSpacerItem(horizontalSpacer);

    QSpacerItem* spacer = new QSpacerItem(0,0, QSizePolicy::Fixed, QSizePolicy::Expanding);
    vLayout->addSpacerItem(spacer);
    hLayout->addLayout(vLayout);
    hLayout->addSpacing(5);
    QFont scoreFont = this->ScoreLabel->font();
    scoreFont.setPointSize(10);
    this->ScoreLabel->setFont(scoreFont);
    vLayout->addWidget(this->ScoreLabel);
    vLayout->addWidget(this->FactorLabel);
    spacer = new QSpacerItem(0,0,QSizePolicy::Fixed, QSizePolicy::Expanding);
    vLayout->addSpacerItem(spacer);

    hLayout->addLayout(vLayout);

    this->UpgradeButtons = new UpgradeButton("Optimize", this);
    hLayout->addWidget(this->UpgradeButtons);
    this->UpgradeButtons->setFixedHeight(50);

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setFrameStyle(QFrame::Box | QFrame::Sunken);
}
