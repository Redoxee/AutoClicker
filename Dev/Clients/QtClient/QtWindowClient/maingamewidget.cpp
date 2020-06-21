#include "maingamewidget.h"

#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QLogValueAxis>
#include <QtCharts/QChartView>
#include <QGraphicsLayout>
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
#include "SWIUtils.h"

using namespace SWIUtils;
using namespace ServerUtils;

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

    connect(gameWindow->ServerWorker(), SIGNAL(RefreshGameData(ServerUtils::ServerGameplayState*)),this, SLOT(refreshData(ServerUtils::ServerGameplayState*)));
    gameWindow->ServerWorker()->RequestOrder(ServerWorker::Order::OrderStartGameplayRefresh);

    connect(this->updateWorker, &UpdateWorker::Update, this, &MainGameWidget::Update);
}

MainGameWidget::~MainGameWidget()
{
    ServerWorker* worker = this->gameWindow->ServerWorker();
    if(worker!= nullptr)
    {
        disconnect(this->gameWindow->ServerWorker(), SIGNAL(RefreshGameData(ServerUtils::ServerGameplayState*)),this, SLOT(refreshData(ServerGameplayState*)));
    }
}

void MainGameWidget::SetupUI()
{
    QVBoxLayout* vBoxLayout = new QVBoxLayout(this);
    vBoxLayout->setMargin(0);

    this->historySeries = new QtCharts::QLineSeries();
    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->legend()->hide();
    chart->addSeries(this->historySeries);
    chart->layout()->setContentsMargins(0, 0, 0, 0);
    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->setBackgroundRoundness(0);

    this->historyYAxis = new QtCharts::QLogValueAxis();
    this->historyYAxis->setMin(0);
    this->historyYAxis->setLabelFormat("%g bits");
    QFont font = this->historyYAxis->labelsFont();
    font.setPointSize(6);
    this->historyYAxis->setLabelsFont(font);
    this->historyYAxis->setBase(10.0);
    this->historyYAxis->setMinorTickCount(0);
    chart->addAxis(this->historyYAxis, Qt::AlignLeft);
    this->historySeries->attachAxis(this->historyYAxis);
    this->historyChartView = new QtCharts::QChartView(chart);
    this->historyChartView->setRenderHint(QPainter::Antialiasing, true);
    this->historyChartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->historyChartView->setMinimumHeight(100);
    this->historyChartView->setMaximumHeight(100);

    vBoxLayout->addWidget(this->historyChartView);

    this->scoreSlot = new ScoreSlot(this);
    vBoxLayout->addWidget(this->scoreSlot);

    this->prestigeSlot = new UpgradeSlot(this);
    vBoxLayout->addWidget(this->prestigeSlot);

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

    vBoxLayout->addLayout(progressLayout);

    QBoxLayout* upgradeLayout = new QVBoxLayout();
    upgradeLayout->setSpacing(0);
    upgradeLayout->setMargin(0);

    this->clickUpgradeSlot = new UpgradeSlot(this);
    this->clickUpgradeSlot->MainPattern = "Faster manual install";
    this->clickUpgradeSlot->SecondPattern = "+%1 bits per click.";
    this->clickUpgradeSlot->InstanceBought->setText("0");
    this->clickUpgradeSlot->UpgradeButtons->mainButtonPattern = "Aquire\n%1 bits";
    this->clickUpgradeSlot->UpgradeButtons->secondButtonPattern = "Improve each unit by a factor of %1  |  %2 bits";
    this->clickUpgradeSlot->UpgradeButtons->secondButtonTooltipPattern = "Needs at least %1 unit aquiered.";
    this->clickUpgradeSlot->TooltipPattern = "Currently improving the manual installer by %1 bits per click";

    connect(this->clickUpgradeSlot->UpgradeButtons->MainButton, &QPushButton::clicked, this, [this](){this->UpgradeButtonClick(ServerGameplayState::ClickUpgradeIndex);});
    this->clickUpgradeSlot->SetMainLabelValue(0);
    this->clickUpgradeSlot->SetSubLabelValue(0);
    connect(this->clickUpgradeSlot->UpgradeButtons->secondaryAction, &QAction::triggered, this, [this](){this->UpgradeButtonClick(ServerGameplayState::ClickUpgradeImproveIndex);});

    this->firstGeneratorSlot = new UpgradeSlot(this);
    this->firstGeneratorSlot->MainPattern = "Auto installer";
    this->firstGeneratorSlot->SecondPattern = "Install %1 bits/s!";
    this->firstGeneratorSlot->TooltipPattern = "Currently installing %1 bits per seconds";

    connect(this->firstGeneratorSlot->UpgradeButtons->MainButton, &QPushButton::clicked, this, [this](){this->UpgradeButtonClick(ServerGameplayState::FirstGeneratorIndex);});
    this->firstGeneratorSlot->UpgradeButtons->mainButtonPattern = "Aquire\n%1 bits";
    this->firstGeneratorSlot->UpgradeButtons->secondButtonPattern = "Improve each unit by a factor of %1  |  %2 bits";
    this->firstGeneratorSlot->UpgradeButtons->secondButtonTooltipPattern = "Needs at least %1 unit aquiered.";
    this->firstGeneratorSlot->SetMainLabelValue(0);
    this->firstGeneratorSlot->SetSubLabelValue(0);
    connect(this->firstGeneratorSlot->UpgradeButtons->secondaryAction, &QAction::triggered, this, [this](){this->UpgradeButtonClick(ServerGameplayState::FirstGeneratorImproveIndex);});


    this->secondGeneratorSlot = new UpgradeSlot(this);
    this->secondGeneratorSlot->MainPattern = "Super installer";
    this->secondGeneratorSlot->SecondPattern = "Install %1 bits/s.";
    connect(this->secondGeneratorSlot->UpgradeButtons->MainButton, &QPushButton::clicked, this, [this](){this->UpgradeButtonClick(ServerGameplayState::SecondGeneratorIndex);});
    this->secondGeneratorSlot->UpgradeButtons->mainButtonPattern = "Aquire\n%1 bits";
    this->secondGeneratorSlot->UpgradeButtons->secondButtonPattern = "Improve each unit by a factor of %1  |  %2 bits";
    this->secondGeneratorSlot->UpgradeButtons->secondButtonTooltipPattern = "Needs at least %1 unit aquiered.";
    this->secondGeneratorSlot->SetMainLabelValue(0);
    this->secondGeneratorSlot->SetSubLabelValue(0);
    connect(this->secondGeneratorSlot->UpgradeButtons->secondaryAction, &QAction::triggered, this, [this](){this->UpgradeButtonClick(ServerGameplayState::SecondGeneratorImproveIndex);});
    this->secondGeneratorSlot->TooltipPattern = "Currently installing %1 bits per seconds";

    upgradeLayout->addWidget(this->secondGeneratorSlot);
    upgradeLayout->addWidget(this->firstGeneratorSlot);
    upgradeLayout->addWidget(this->clickUpgradeSlot);

    vBoxLayout->addLayout(upgradeLayout);

    this->clickerButton = new UpgradeButton(this);
    this->clickerButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->clickerButton->mainButtonPattern = "Manual install (+%1 bits)";
    this->clickerButton->secondButtonPattern = "Boost your auto installer temporarely by using your manual installer %1 bits  |  %2 bits";
    this->clickerButton->MainButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->clickerButton->MainButton->setMaximumWidth(3000);
    this->clickerButton->setFixedHeight(50);
    this->clickerButton->secondButtonTooltipPattern = "Locked if the manual installer is over %1 improvements";
    vBoxLayout->addWidget(this->clickerButton);
    connect(this->clickerButton->MainButton, &QPushButton::clicked, this, &MainGameWidget::handleClick);
    connect(this->clickerButton->secondaryAction, &QAction::triggered, this, [this](){this->UpgradeButtonClick(ServerGameplayState::ClickBonusFactorIndex);});

    this->prestigeSlot->MainPattern = "Defragment installation";
    this->prestigeSlot->SecondPattern = "Reset installation but speedup the process by a factor of %1";
    this->prestigeSlot->UpgradeButtons->mainButtonPattern = "Aquire\n%1 bits";
    this->prestigeSlot->UpgradeButtons->secondButtonPattern = "Boost the defragmentation by %1  |  %2 bits";
    this->prestigeSlot->SetMainLabelValue(0);
    this->prestigeSlot->TooltipPattern = "Currently multiplying every install by %1";

    connect(this->prestigeSlot->UpgradeButtons->MainButton, &QPushButton::clicked, this, [this](){
        this->UpgradeButtonClick(ServerGameplayState::PrestigeIndex);
    });
    connect(this->prestigeSlot->UpgradeButtons->secondaryAction, &QAction::triggered, this, [this](){this->UpgradeButtonClick(ServerGameplayState::PrestigeImproveIndex);});

    this->finishButton = new QPushButton();
    this->gameWindow->BottomBox->addWidget(finishButton);
    this->finishButton->setVisible(false);
    QSizePolicy sizePolicy = this->finishButton->sizePolicy();
    sizePolicy.setRetainSizeWhenHidden(true);
    this->finishButton->setSizePolicy(sizePolicy);
    this->finishButton->setText("Next");

    connect(this->finishButton, &QPushButton::clicked, this, &MainGameWidget::onFinishButtonClicked);

    if(!GameSettings::Instance()->SkipUpgradeReveal)
    {
        this->clickerButton->SecondaryButton->setVisible(false);
        this->clickUpgradeSlot->setVisible(false);
        this->clickUpgradeSlot->UpgradeButtons->SecondaryButton->setVisible(false);
        this->firstGeneratorSlot->setVisible(false);
        this->firstGeneratorSlot->UpgradeButtons->SecondaryButton->setVisible(false);
        this->secondGeneratorSlot->setVisible(false);
        this->secondGeneratorSlot->UpgradeButtons->SecondaryButton->setVisible(false);
        this->prestigeSlot->setVisible(false);
        this->prestigeSlot->UpgradeButtons->SecondaryButton->setVisible(false);

        for(int index = 1; index < 4; ++index)
        {
            this->ProgressBar[index]->setVisible(false);
        }

        this->historyChartView->setVisible(false);
    }
}

void MainGameWidget::handleClick()
{
    this->isDirty = true;
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
    int64_t passiveSpeed = serverData->PassiveSpeed * serverData->GlobalFactor;
    if(serverData->TempBonusDuration > 0 && serverData->TempBonusFactor > 1)
    {
        passiveSpeed *= serverData->TempBonusFactor;
    }

    int framePerSecond = 1000 / AutoClicker::FrameLength();
    passiveSpeed *= framePerSecond;

    QString passiveSpeedMessage = QString("Autoinstall %1 bits per second").arg(SWIUtils::FormatDownQuantity(passiveSpeed));

    QString scoreMessage = QString("%1 bits installed").arg(QString::number(serverData->Score));
    this->scoreSlot->ScoreLabel->setText(scoreMessage);
    this->scoreSlot->FactorLabel->setText(passiveSpeedMessage);

    this->frameValueLabel->setText(QString::number(serverData->FrameCount));

    int clickValue = serverData->ClickValue;
    int bonusFactor = 1;
    int globalFactor = 1;

    if(serverData->GlobalFactor > 1)
    {
        globalFactor = serverData->GlobalFactor;
    }

    bonusFactor *= globalFactor;

    if(serverData->TempBonusDuration > 0)
    {
        bonusFactor *= serverData->TempBonusFactor;
    }

    clickValue *= bonusFactor;

    this->clickUpgradeSlot->RefreshDisplay(serverData->clickUpgrade, globalFactor, serverData->clickUpgradeImprove);
    this->firstGeneratorSlot->RefreshDisplay(serverData->firstGenerator, framePerSecond * globalFactor, serverData->firstGeneratorImprove);
    this->secondGeneratorSlot->RefreshDisplay(serverData->secondGenerator, framePerSecond * globalFactor, serverData->secondGeneratorImprove);
    this->prestigeSlot->RefreshDisplay(serverData->prestige, 1, serverData->prestigeImprove);

    this->clickUpgradeSlot->SetTooltipValue(serverData->clickUpgrade->InstanceBought * serverData->clickUpgrade->ImpactValue * bonusFactor);
    this->firstGeneratorSlot->SetTooltipValue(serverData->firstGenerator->InstanceBought * serverData->firstGenerator->ImpactValue * bonusFactor * framePerSecond);
    this->secondGeneratorSlot->SetTooltipValue(serverData->secondGenerator->InstanceBought * serverData->secondGenerator->ImpactValue * bonusFactor * framePerSecond);
    this->prestigeSlot->SetTooltipValue(globalFactor);

    if(!this->clickerButton->SecondaryButton->isVisible())
    {
        if((serverData->clickFactor->FailureFlags & ~FailureFlags::NotEnoughMoney) == FailureFlags::None)
        {
            if(serverData->Score >= (serverData->clickFactor->Price / 2))
            {
                this->clickerButton->SecondaryButton->setVisible(true);
            }
        }
    }

    if(!this->clickUpgradeSlot->isVisible())
    {
        if((serverData->clickUpgrade->FailureFlags & ~FailureFlags::NotEnoughMoney) == FailureFlags::None)
        {
            if(serverData->Score >= (serverData->clickUpgrade->Price / 2))
            {
                this->clickUpgradeSlot->setVisible(true);
            }
        }
    }

    if((serverData->clickUpgradeImprove->FailureFlags & ~FailureFlags::NotEnoughMoney) == FailureFlags::None && !this->clickUpgradeSlot->UpgradeButtons->SecondaryButton->isVisible())
    {
        this->clickUpgradeSlot->UpgradeButtons->SecondaryButton->setVisible(true);
    }

    if(!this->firstGeneratorSlot->isVisible())
    {
        if((serverData->firstGenerator->FailureFlags & ~FailureFlags::NotEnoughMoney) == FailureFlags::None)
        {
            if(serverData->Score >= (serverData->firstGenerator->Price / 2))
            {
                this->firstGeneratorSlot->setVisible(true);
            }
        }
    }

    if((serverData->firstGeneratorImprove->FailureFlags & ~FailureFlags::NotEnoughMoney) == FailureFlags::None && !this->firstGeneratorSlot->UpgradeButtons->SecondaryButton->isVisible())
    {
        this->firstGeneratorSlot->UpgradeButtons->SecondaryButton->setVisible(true);
    }

    if(!this->secondGeneratorSlot->isVisible())
    {
        if((serverData->secondGenerator->FailureFlags & ~FailureFlags::NotEnoughMoney) == FailureFlags::None)
        {
            if(serverData->Score >= (serverData->secondGenerator->Price / 2))
            {
                this->secondGeneratorSlot->setVisible(true);
            }
        }
    }

    if((serverData->secondGeneratorImprove->FailureFlags & ~FailureFlags::NotEnoughMoney) == FailureFlags::None && !this->secondGeneratorSlot->UpgradeButtons->SecondaryButton->isVisible())
    {
        this->secondGeneratorSlot->UpgradeButtons->SecondaryButton->setVisible(true);
    }

    if(!this->prestigeSlot->isVisible())
    {
        if((serverData->prestige->FailureFlags & ~FailureFlags::NotEnoughMoney) == FailureFlags::None)
        {
            if(serverData->Score >= (serverData->prestige->Price / 2))
            {
                this->prestigeSlot->setVisible(true);
            }
        }
    }

    if((serverData->prestigeImprove->FailureFlags & ~FailureFlags::NotEnoughMoney) == FailureFlags::None && !this->prestigeSlot->UpgradeButtons->SecondaryButton->isVisible())
    {
        this->prestigeSlot->UpgradeButtons->SecondaryButton->setVisible(true);
    }

    this->clickerButton->SetMainButtonValue(clickValue);
    this->clickerButton->SetSecondaryButtonValue(serverData->clickFactor->ImpactValue, serverData->clickFactor->Price);
    this->clickerButton->SetSecondaryTooltipValue(serverData->clickFactor->Lock.Value);
    if (serverData->clickFactor->FailureFlags != FailureFlags::None)
    {
        this->clickerButton->secondaryAction->setEnabled(false);
    }
    else
    {
        this->clickerButton->secondaryAction->setEnabled(true);
    }

    this->gameWindow->currentFrame = serverData->FrameCount;

    this->realCurrentScore = serverData->Score;

    this->pushToScoreHistory(serverData->Score);

    int displayGap = abs(this->displayedScore - this->realCurrentScore);

    if(!this->historyChartView->isVisible() && displayGap > 15000)
    {
        this->historyChartView->setVisible(true);
    }

    if(this->historyChartView->isVisible())
    {
        this->refreshHistory();
    }

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
        if(!this->ProgressBar[index]->isVisible() && this->ProgressBar[index]->value() > 0)
        {
            this->ProgressBar[index]->setVisible(true);
        }
    }
}

double damping(double pos, double target, double k,double dt)
{
    return pos + ((target - pos) / k * dt);
}

void MainGameWidget::Update(int)
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

void MainGameWidget::resetHistory()
{
    for(int index = 0; index < this->historySize; ++index)
    {
        this->scoreHistory[index] = 0;
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
    hLayout-> setMargin(0);
    this->setFixedHeight(50);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    this->InstanceBought = new QLabel(this);
    this->InstanceBought->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    this->InstanceBought->setFixedWidth(33);
    this->InstanceBought->setText("999");
    this->InstanceBought->setAlignment(Qt::AlignmentFlag::AlignCenter);
    QFont instanceFont = this->InstanceBought->font();
    instanceFont.setPixelSize(15);
    this->InstanceBought->setFont(instanceFont);
    hLayout->addWidget(this->InstanceBought);

    hLayout->addSpacing(5);
    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->setMargin(0);
    vLayout->setSpacing(0);
    QSpacerItem* spacer = new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding);
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
    QFont subFont = this->SubLabel->font();
    subFont.setPointSize(8);
    this->SubLabel->setFont(subFont);

    spacer = new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding);
    vLayout->addSpacerItem(spacer);

    this->UpgradeButtons = new UpgradeButton(this);
    hLayout->addWidget(this->UpgradeButtons);

    QSplitter* splitter = new QSplitter(this);
    vLayout->addWidget(splitter);

    this->MainLayout = hLayout;
    this->setFrameStyle(QFrame::Box | QFrame::Sunken);

    this->InstanceBought->setVisible(false);
}

void UpgradeSlot::SetMainLabelValue(int64_t value)
{
    this->MainLabel->setText(this->MainPattern.arg(value));
}

void UpgradeSlot::SetSubLabelValue(int64_t value)
{
    this->SubLabel->setText(this->SecondPattern.arg(value));
}

void UpgradeSlot::SetTooltipValue(int64_t value)
{
    if(value > 0)
    {
        this->setToolTip(this->TooltipPattern.arg(value));
    }
    else
    {
        this->setToolTip("");
    }
}

void UpgradeSlot::RefreshDisplay(Upgrade *mainUpgrade, int mainImpactFactor, Upgrade *improve)
{
    this->InstanceBought->setText(QString::number(mainUpgrade->InstanceBought));
    this->SetSubLabelValue(mainUpgrade->ImpactValue * mainImpactFactor);
    this->UpgradeButtons->SetMainButtonValue(mainUpgrade->Price);
    this->UpgradeButtons->SetSecondaryButtonValue(improve->ImpactValue, improve->Price);

    if(improve->Lock.Target != "None" && !this->UpgradeButtons->secondButtonTooltipPattern.isEmpty())
    {
        this->UpgradeButtons->SetSecondaryTooltipValue(improve->Lock.Value);
    }

    if(mainUpgrade->FailureFlags != FailureFlags::None)
    {
        this->UpgradeButtons->MainButton->setEnabled(false);
        this->UpgradeButtons->SecondaryButton->setEnabled(false);
    }
    else
    {
        this->UpgradeButtons->MainButton->setEnabled(true);
        this->UpgradeButtons->SecondaryButton->setEnabled(true);

        if(improve->FailureFlags != FailureFlags::None)
        {
            this->UpgradeButtons->secondaryAction->setEnabled(false);
        }
        else
        {
            this->UpgradeButtons->secondaryAction->setEnabled(true);
        }
    }

    if(mainUpgrade->InstanceBought > 1 && !this->InstanceBought->isVisible())
    {
        this->InstanceBought->setVisible(true);
    }
}

ScoreSlot::ScoreSlot(QWidget* parent): QFrame(parent)
{
    QHBoxLayout* hLayout = new QHBoxLayout(this);
    QVBoxLayout* vLayout = new QVBoxLayout();
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

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setFrameStyle(QFrame::Box | QFrame::Sunken);
    this->setFixedHeight(50);
}
