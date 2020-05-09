#include "endgamewidget.h"

#include <QProgressBar>
#include <QPauseAnimation>

#include "gamewindow.h"
#include "updateworker.h"
#include "gridprogressbar.h"
#include "spiralprogressbar.h"
#include "tiledprogressbar.h"
#include "crisscrossprogressbar.h"
#include "doorstyleprogressbar.h"
#include "endscorewidget.h"
#include "autoclickerconfig.h"
#include "fancyprogressbarwrapper.h"
#include "scaledprogressbar.h"

EndGameWidget::EndGameWidget(GameWindow* gameWindow) : QWidget(gameWindow)
{
    this->gameWindow = gameWindow;

    this->SetupUI();
    this->updateWorker = new UpdateWorker();

    this->time = 0;
//    connect(this->updateWorker, &UpdateWorker::Update, this, &EndGameWidget::Update);
}

void EndGameWidget::SetupUI()
{
    QGridLayout* gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(0);
    gridLayout->setMargin(0);

    QVBoxLayout* vboxLayout = new QVBoxLayout();
    vboxLayout->setMargin(0);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    gridLayout->addLayout(vboxLayout, 0, 0);

    this->spiralProgressBar = new SpiralProgressBar(this);
    vboxLayout->addWidget(this->spiralProgressBar);
    this->spiralProgressBar->SetProgress(0);
    QWidget* placeHolderWidget = new QWidget(this->spiralProgressBar);
    this->spiralProgressBar->CentralLayout->addWidget(placeHolderWidget);

    QSequentialAnimationGroup* firstSequence = new QSequentialAnimationGroup(this);
    FancyProgressBarWrapper* spiralWrapper = new FancyProgressBarWrapper(2000, this->spiralProgressBar);
//    spiralWrapper->setEasingCurve(QEasingCurve::InExpo);
    firstSequence->addAnimation(spiralWrapper);

    QPauseAnimation* firstPause = new QPauseAnimation(2000, this);
    firstSequence->addAnimation(firstPause);

    this->BigBar = new ScaledProgressBar(101, this);
    this->BigBar->setStyleSheet("border : 0px;margin : -1px;background:transparent;");
    this->BigBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->BigBar->setOrientation(Qt::Orientation::Vertical);
    this->BigBar->setValue(0);
    this->BigBar->setTextVisible(false);

    FancyProgressBarWrapper* downUpWrapper = new FancyProgressBarWrapper(3000, this->BigBar, QAbstractAnimation::Direction::Forward);
    firstSequence->addAnimation(downUpWrapper);


    QPauseAnimation* secondPause = new QPauseAnimation(800, this);
    firstSequence->addAnimation(secondPause);

    FancyProgressBarWrapper* downUpBackWrapper = new FancyProgressBarWrapper(3000, this->BigBar, QAbstractAnimation::Direction::Backward);
    firstSequence->addAnimation(downUpBackWrapper);

    this->tiledProgressBar = new TiledProgressBar(this);
    this->tiledProgressBar->SetProgress(0);
    this->tiledProgressBar->setVisible(false);
    FancyProgressBarWrapper* tiledWrapper = new FancyProgressBarWrapper(3000, this->tiledProgressBar);
    firstSequence->addAnimation(tiledWrapper);

    QPauseAnimation* thirdPause = new QPauseAnimation(3000, this);
    firstSequence->addAnimation(thirdPause);

    FancyProgressBarWrapper* rightLeftWrapper = new FancyProgressBarWrapper(3000, this->BigBar, QAbstractAnimation::Direction::Forward);
    firstSequence->addAnimation(rightLeftWrapper);

    QPauseAnimation* fourthPause = new QPauseAnimation(800, this);
    firstSequence->addAnimation(fourthPause);

    FancyProgressBarWrapper* rightLeftBackWrapper = new FancyProgressBarWrapper(3000, this->BigBar, QAbstractAnimation::Direction::Backward);
    firstSequence->addAnimation(rightLeftBackWrapper);

    gridLayout->addWidget(this->BigBar, 0, 0);

    connect(downUpWrapper, &QAbstractAnimation::finished, this, [this, vboxLayout]() {
        delete this->spiralProgressBar;

        this->tiledProgressBar->setVisible(true);
        vboxLayout->addWidget(this->tiledProgressBar);
    });

    connect(secondPause, &QPauseAnimation::finished, this, [this]() {
        this->BigBar->setInvertedAppearance(true);
    });

    connect(rightLeftWrapper, &QAbstractAnimation::stateChanged, this, [this](QAbstractAnimation::State newState){
        if(newState == QAbstractAnimation::State::Running)
        {
            this->BigBar->setOrientation(Qt::Orientation::Horizontal);
            this->BigBar->setInvertedAppearance(true);
            this->BigBar->SetProgress(0.0f);
            this->BigBar->raise();
        }
    });

    connect(rightLeftWrapper, &QAbstractAnimation::finished, this, [this](){
        delete this->tiledProgressBar;
    });

    connect(rightLeftBackWrapper, &QAbstractAnimation::stateChanged, this, [this](QAbstractAnimation::State newState){
        if(newState == QAbstractAnimation::State::Running)
        {
            this->BigBar->setInvertedAppearance(false);
        }
    });

    connect(firstSequence, &QSequentialAnimationGroup::finished, this, &EndGameWidget::FirstSequenceFinished);

/*
    this->gridProgressBar = new GridProgressBar(this,8, 12);
    this->gridProgressBar->hide();
*/

    //this->doorStyleProgressBar = new DoorStyleProgressBar(40, this);
    //vboxLayout->addWidget(this->doorStyleProgressBar);

    //this->crissCrossProgressBar = new CrissCrossProgressBar(30, 15, 8, this);
    //this->gameWindow->LeftGLayout->addWidget(this->crissCrossProgressBar, 0,0);
    //vboxLayout->addWidget(this->crissCrossProgressBar);
    //this->crissCrossProgressBar->SetValue(0);


    this->endScoreWidget = new EndScoreWidget(this);
    vboxLayout->addWidget(this->endScoreWidget);

    QSpacerItem* spacer;
    spacer = new QSpacerItem(0,0, QSizePolicy::Fixed, QSizePolicy::Expanding);
    this->endScoreWidget->centralLayout->addItem(spacer);

    QLabel* endMessage = new QLabel();
    endMessage->setAlignment(Qt::AlignCenter);
    endMessage->setText("Thank you\nfor playing");
    this->endScoreWidget->centralLayout->addWidget(endMessage);

    QLabel* scoreMessage = new QLabel();
    scoreMessage->setAlignment(Qt::AlignCenter);
    this->endScoreWidget->centralLayout->addWidget(scoreMessage);
    QString scoreStr = QString("your final score is \n%0");
    if(gameWindow->currentFrame>0)
    {
        int finalScore = AutoClicker::ScoreFormula(gameWindow->currentFrame);
        scoreStr = scoreStr.arg(finalScore);
    }
    else
    {
        scoreStr = scoreStr.arg(000000000);
    }

    scoreMessage->setText(scoreStr);

    spacer = new QSpacerItem(0,0, QSizePolicy::Fixed, QSizePolicy::Expanding);
    this->endScoreWidget->centralLayout->addItem(spacer);
    this->endScoreWidget->Update(0);

    this->endScoreWidget->setVisible(false);

    firstSequence->start(QAbstractAnimation::DeletionPolicy::DeleteWhenStopped);
}

void EndGameWidget::Update(float dt)
{
    this->time += dt;
    this->endScoreWidget->Update(this->time);
}

void EndGameWidget::AnimationFinishedDelete(QWidget *target)
{
    delete target;
}

void EndGameWidget::FirstSequenceFinished()
{
//    this->spiralProgressBar->setEnabled(false);
//    delete this->spiralProgressBar;

    this->endScoreWidget->setVisible(true);
    this->time = 0;
    connect(this->updateWorker, &UpdateWorker::Update, this, &EndGameWidget::Update);
}
