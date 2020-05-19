#include "maingamewidget.h"

#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QLogValueAxis>
#include <QtCharts/QChartView>

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

    connect(this->clickerButton, SIGNAL (clicked()), this, SLOT (handleClick()));

    scoreValueLabel->setText("0");
    frameValueLabel->setText("0");

    this->RefreshProgressBars(0);

    this->lastRefreshedFrame = -1;

    connect(gameWindow->ServerWorker(), SIGNAL(RefreshGameData(ServerGameplayState*)),this, SLOT(refreshData(ServerGameplayState*)));
    gameWindow->ServerWorker()->RequestOrder(ServerWorker::Order::OrderStartGameplayRefresh);

    connect(this->updateWorker, &UpdateWorker::Update, this, &MainGameWidget::Update);
}

MainGameWidget::~MainGameWidget()
{

    disconnect(this->gameWindow->ServerWorker(), SIGNAL(RefreshGameData(ServerGameplayState*)),this, SLOT(refreshData(ServerGameplayState*)));

    for(auto current = this->UpgradeButtons.begin(); current != this->UpgradeButtons.end(); current++)
    {
        delete (*current);
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

    this->UpgradeLayout = new QGridLayout();
    vBoxLayout->addLayout(this->UpgradeLayout);

    this->clickerButton = new QPushButton("Click", this);
    vBoxLayout->addWidget(this->clickerButton);

    this->finishButton = new QPushButton();
    this->gameWindow->BottomBox->addButton(finishButton, QDialogButtonBox::ButtonRole::YesRole);
    this->finishButton->setVisible(false);
    QSizePolicy sizePolicy = this->finishButton->sizePolicy();
    sizePolicy.setRetainSizeWhenHidden(true);
    this->finishButton->setSizePolicy(sizePolicy);
    this->finishButton->setText("Next >>");

    connect(this->finishButton, &QPushButton::clicked, this, &MainGameWidget::onFinishButtonClicked);
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
    QString scoreLabel = QString("Score %1 / %2").arg(QString::number(serverData->Score), QString::number(serverData->TargetScore));
    scoreLabel += QString("\n(Passive speed : %1 * %2").arg(FormatDownQuantity(serverData->PassiveSpeed), FormatDownQuantity(serverData->GlobalFactor));
    if(serverData->TempBonusDuration > 0 && serverData->TempBonusFactor > 1)
    {
        scoreLabel += QString(" * %1").arg(serverData -> TempBonusFactor);
    }

    scoreLabel += QString(")");

    this->scoreValueLabel->setText(scoreLabel);
    this->frameValueLabel->setText(QString::number(serverData->FrameCount));
    this->clickValueLabel->setText("+" + FormatDownQuantity(serverData->ClickValue) + " bytes");

    this->gameWindow->currentFrame = serverData->FrameCount;

    this->realCurrentScore = serverData->Score;
    if(serverData->NumberOfUpgrades != this->UpgradeButtons.size())
    {
        if(serverData->NumberOfUpgrades > this->UpgradeButtons.size())
        {
            size_t buttonToAdd = serverData->NumberOfUpgrades - this->UpgradeButtons.size();
            for(int i = 0; i < buttonToAdd; ++i)
            {
                QPushButton* btn = new QPushButton;
                this->UpgradeButtons.push_back(btn);
                int index = this->UpgradeButtons.size() - 1;
                int col = index % 3;
                int row = index / 3;
                this->UpgradeLayout->addWidget(btn, row, col);
                connect(btn, &QPushButton::clicked, [=](){ emit this->UpgradeButtonClick(index);});
            }
        }
        else
        {
            size_t buttonToRemove = this->UpgradeButtons.size() - serverData->NumberOfUpgrades;
            for(int i = 0; i < buttonToRemove; ++i)
            {
                QPushButton* btn = this->UpgradeButtons[this->UpgradeButtons.size() - 1];
                delete btn;
                this->UpgradeButtons.pop_back();
            }
        }

        this->isDirty = true;
    }

    if(this->isDirty)
    {
        for(int index = 0; index < serverData->NumberOfUpgrades; ++index)
        {
            this->UpgradeButtons[index]->setText(serverData->Upgrades[index].GetLabel());
            this->UpgradeButtons[index]->setEnabled(serverData->Upgrades[index].FailureFlags == 0);
            this->UpgradeButtons[index]->setToolTip(serverData->Upgrades[index].Description);
        }

        this->isDirty = false;
        this->lastRefreshedFrame = serverData->FrameCount;
    }
    else
    {
        for(int index = 0; index < serverData->NumberOfUpgrades; ++index)
        {
            this->UpgradeButtons[index]->setEnabled(serverData->Upgrades[index].FailureFlags == 0);
        }
    }

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
    this->gameWindow->BottomBox->removeButton(this->finishButton);
    this->gameWindow->GotToScreen(Screens::EndGameScreen);
}
