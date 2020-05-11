#include "endgamewidget.h"

#include <QProgressBar>
#include <QPropertyAnimation>
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

void firstBarSequence(QProgressBar* target, QSequentialAnimationGroup* sequence, QWidget* parent);

void EndGameWidget::SetupUI()
{
    QSequentialAnimationGroup* firstSequence = new QSequentialAnimationGroup(this);

    QGridLayout* gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(0);
    gridLayout->setMargin(0);

    QVBoxLayout* vboxLayout = new QVBoxLayout();
    vboxLayout->setMargin(0);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    gridLayout->addLayout(vboxLayout, 0, 0);

    this->BigBar = new ScaledProgressBar(101, this);
    this->BigBar->setStyleSheet("border : 0px;margin : -1px;background:transparent;");
    this->BigBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->BigBar->setOrientation(Qt::Orientation::Vertical);
    this->BigBar->setValue(0);
    this->BigBar->setTextVisible(false);

    QProgressBar* firstFakeBar = new QProgressBar(this);
    firstBarSequence(firstFakeBar, firstSequence, this);

    this->crissCrossProgressBar = new CrissCrossProgressBar(28, 15, 4, this);
    this->crissCrossProgressBar->SetProgress(0);
    FancyProgressBarWrapper* crissCrossWrapper = new FancyProgressBarWrapper(23000, this->crissCrossProgressBar);
    crissCrossWrapper->setEasingCurve(QEasingCurve::InSine);
    firstSequence->addAnimation(crissCrossWrapper);

    this->crissCrossProgressBar->FirstLayout()->addWidget(firstFakeBar);

    vboxLayout->insertWidget(0, this->crissCrossProgressBar);

    QPauseAnimation* firstPause = new QPauseAnimation(8000, this);
    firstSequence->addAnimation(firstPause);

    FancyProgressBarWrapper* downUpWrapper = new FancyProgressBarWrapper(1000, this->BigBar, QAbstractAnimation::Direction::Forward);
    downUpWrapper->setEasingCurve(QEasingCurve::InQuad);
    firstSequence->addAnimation(downUpWrapper);

    QPauseAnimation* secondPause = new QPauseAnimation(300, this);
    firstSequence->addAnimation(secondPause);

    FancyProgressBarWrapper* downUpBackWrapper = new FancyProgressBarWrapper(1000, this->BigBar, QAbstractAnimation::Direction::Backward);
    downUpWrapper->setEasingCurve(QEasingCurve::OutQuad);
    firstSequence->addAnimation(downUpBackWrapper);

    this->spiralProgressBar = new SpiralProgressBar(this);
    this->spiralProgressBar->SetProgress(0);
    this->spiralProgressBar->setVisible(false);
    QWidget* placeHolderWidget = new QWidget(this->spiralProgressBar);
    this->spiralProgressBar->CentralLayout->addWidget(placeHolderWidget);
    FancyProgressBarWrapper* spiralWrapper = new FancyProgressBarWrapper(20000, this->spiralProgressBar);
    spiralWrapper->setEasingCurve(QEasingCurve::InSine);
    firstSequence->addAnimation(spiralWrapper);

    QPauseAnimation* thirdPause = new QPauseAnimation(9000, this);
    firstSequence->addAnimation(thirdPause);

    FancyProgressBarWrapper* rightLeftWrapper = new FancyProgressBarWrapper(800, this->BigBar, QAbstractAnimation::Direction::Forward);
    rightLeftWrapper->setEasingCurve(QEasingCurve::InExpo);
    firstSequence->addAnimation(rightLeftWrapper);

    QPauseAnimation* fourthPause = new QPauseAnimation(100, this);
    firstSequence->addAnimation(fourthPause);

    FancyProgressBarWrapper* rightLeftBackWrapper = new FancyProgressBarWrapper(3000, this->BigBar, QAbstractAnimation::Direction::Backward);
    rightLeftWrapper->setEasingCurve(QEasingCurve::InQuad);
    firstSequence->addAnimation(rightLeftBackWrapper);

    this->tiledProgressBar = new TiledProgressBar(this);
    this->tiledProgressBar->SetProgress(0);
    this->tiledProgressBar->setVisible(false);
    FancyProgressBarWrapper* tiledWrapper = new FancyProgressBarWrapper(25000, this->tiledProgressBar);
    tiledWrapper->setEasingCurve(QEasingCurve::InSine);
    firstSequence->addAnimation(tiledWrapper);

    QPauseAnimation* fithPause = new QPauseAnimation(8000, this);
    firstSequence->addAnimation(fithPause);

    FancyProgressBarWrapper* leftRightWrapper = new FancyProgressBarWrapper(800, this->BigBar);
    firstSequence->addAnimation(leftRightWrapper);

    this->doorStyleProgressBar = new DoorStyleProgressBar(40, this);
    this->doorStyleProgressBar->SetProgress(0);
    gameWindow->LeftGLayout->addWidget(this->doorStyleProgressBar, 0, 0);

    FancyProgressBarWrapper* leftDoorWrapper = new FancyProgressBarWrapper(6000, this->doorStyleProgressBar);
    leftDoorWrapper->setEasingCurve(QEasingCurve::OutSine);
    firstSequence->addAnimation(leftDoorWrapper);

    FancyProgressBarWrapper* leftRightBackWrapper = new FancyProgressBarWrapper(800, this->BigBar, QAbstractAnimation::Direction::Backward);
    firstSequence->addAnimation(leftRightBackWrapper);

    FancyProgressBarWrapper* leftDoorWrapperBack = new FancyProgressBarWrapper(6000, this->doorStyleProgressBar, QAbstractAnimation::Direction::Backward);
    leftDoorWrapperBack->setEasingCurve(QEasingCurve::InSine);
    firstSequence->addAnimation(leftDoorWrapperBack);

    gridLayout->addWidget(this->BigBar, 0, 0);

    this->BigBar->raise();

    connect(downUpWrapper, &QAbstractAnimation::finished, this, [this, vboxLayout, firstFakeBar]() {
        delete this->crissCrossProgressBar;
        delete firstFakeBar;

        this->spiralProgressBar->setVisible(true);
        vboxLayout->addWidget(this->spiralProgressBar);
        this->BigBar->raise();
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
        delete this->spiralProgressBar;
    });

    connect(rightLeftBackWrapper, &QAbstractAnimation::stateChanged, this, [this](QAbstractAnimation::State newState){
        if(newState == QAbstractAnimation::State::Running)
        {
            this->BigBar->setInvertedAppearance(false);
        }
    });

    connect(tiledWrapper, &QAbstractAnimation::stateChanged, this, [this, gridLayout](QAbstractAnimation::State newState){
        if(newState == QAbstractAnimation::State::Running)
        {
            gridLayout->addWidget(this->tiledProgressBar, 0, 0);
            this->tiledProgressBar->setVisible(true);
            this->BigBar->raise();
        }
    });

    connect(leftDoorWrapper, &QAbstractAnimation::finished, this, [this]()
    {
       QPixmap* logo = new QPixmap("Ressources/ThankYouForPlaying.png");
       QLabel* logoHolder = this->gameWindow->LogoHolder();
       logoHolder->setPixmap(*logo);

       delete this->tiledProgressBar;

       this->endScoreWidget->setVisible(true);
       this->time = 0;
       this->endScoreWidget->Update(0);

       this->doorStyleProgressBar->SetBackward(true);
    });

    connect(leftDoorWrapperBack, &QAbstractAnimation::finished, this, [this](){
        delete this->doorStyleProgressBar;
    });


    connect(firstSequence, &QAbstractAnimation::finished, this, [this]()
    {
        connect(this->updateWorker, &UpdateWorker::Update, this, &EndGameWidget::Update);
    });

    this->endScoreWidget = new EndScoreWidget(this);
    vboxLayout->addWidget(this->endScoreWidget);

    QSpacerItem* spacer;
    spacer = new QSpacerItem(0,0, QSizePolicy::Fixed, QSizePolicy::Expanding);
    this->endScoreWidget->centralLayout->addItem(spacer);

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

void firstBarSequence(QProgressBar* target, QSequentialAnimationGroup* sequence, QWidget* parent)
{
    {
        QPropertyAnimation* pA0 = new QPropertyAnimation(target, "value", parent);
        pA0->setStartValue(0);
        pA0->setEndValue(5);
        pA0->setDuration(5000);
        sequence->addAnimation(pA0);
    }

    sequence->addPause(500);

    {
        QPropertyAnimation* pA1 = new QPropertyAnimation(target, "value", parent);
        pA1->setStartValue(5);
        pA1->setEndValue(17);
        pA1->setDuration(2000);
        sequence->addAnimation(pA1);
    }

    sequence->addPause(900);

    {
        QPropertyAnimation* pA1 = new QPropertyAnimation(target, "value", parent);
        pA1->setStartValue(17);
        pA1->setEndValue(31);
        pA1->setDuration(1500);
        sequence->addAnimation(pA1);
    }

    sequence->addPause(400);

    {
        QPropertyAnimation* pA1 = new QPropertyAnimation(target, "value", parent);
        pA1->setStartValue(31);
        pA1->setEndValue(42);
        pA1->setDuration(2000);
        sequence->addAnimation(pA1);
    }

    sequence->addPause(800);

    {
        QPropertyAnimation* pA1 = new QPropertyAnimation(target, "value", parent);
        pA1->setStartValue(42);
        pA1->setEndValue(69);
        pA1->setDuration(4500);
        sequence->addAnimation(pA1);
    }

    sequence->addPause(3500);

    {
        QPropertyAnimation* pA1 = new QPropertyAnimation(target, "value", parent);
        pA1->setStartValue(69);
        pA1->setEndValue(95);
        pA1->setDuration(1100);
        sequence->addAnimation(pA1);
    }

    sequence->addPause(2000);

    {
        QPropertyAnimation* pA1 = new QPropertyAnimation(target, "value", parent);
        pA1->setStartValue(95);
        pA1->setEndValue(98);
        pA1->setDuration(4000);
        sequence->addAnimation(pA1);
    }

    sequence->addPause(2000);

    {
        QPropertyAnimation* pA1 = new QPropertyAnimation(target, "value", parent);
        pA1->setStartValue(98);
        pA1->setEndValue(99);
        pA1->setDuration(4000);
        sequence->addAnimation(pA1);
    }

    sequence->addPause(3000);
}

void EndGameWidget::Update(float dt)
{
    this->time += dt;
    this->endScoreWidget->Update(this->time);
}
