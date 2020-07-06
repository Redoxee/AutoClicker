#include "endgamewidget.h"

#include <QProgressBar>
#include <QPauseAnimation>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>

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
#include "SWIUtils.h"

EndGameWidget::EndGameWidget(GameWindow* gameWindow) : QWidget(gameWindow)
{
    this->gameWindow = gameWindow;

    this->SetupUI();
    this->updateWorker = new UpdateWorker();

    this->time = 0;
//    connect(this->updateWorker, &UpdateWorker::Update, this, &EndGameWidget::Update);
}

void firstBarSequence(QProgressBar* target, QSequentialAnimationGroup* sequence, float durationFactor, QWidget* parent);

void EndGameWidget::SetupUI()
{
    float animationDurationFactor = 1;

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
    firstBarSequence(firstFakeBar, firstSequence, animationDurationFactor, this);

    this->crissCrossProgressBar = new CrissCrossProgressBar(28, 15, 4, this);
    this->crissCrossProgressBar->SetProgress(0);
    FancyProgressBarWrapper* crissCrossWrapper = new FancyProgressBarWrapper(23000 * animationDurationFactor, this->crissCrossProgressBar);
    crissCrossWrapper->setEasingCurve(QEasingCurve::InSine);
    firstSequence->addAnimation(crissCrossWrapper);

    this->crissCrossProgressBar->FirstLayout()->addWidget(firstFakeBar);

    vboxLayout->insertWidget(0, this->crissCrossProgressBar);

    QPauseAnimation* firstPause = new QPauseAnimation(8000 * animationDurationFactor, this);
    firstSequence->addAnimation(firstPause);

    FancyProgressBarWrapper* downUpWrapper = new FancyProgressBarWrapper(1000 * animationDurationFactor, this->BigBar, QAbstractAnimation::Direction::Forward);
    downUpWrapper->setEasingCurve(QEasingCurve::InQuad);
    firstSequence->addAnimation(downUpWrapper);

    QPauseAnimation* secondPause = new QPauseAnimation(300 * animationDurationFactor, this);
    firstSequence->addAnimation(secondPause);

    FancyProgressBarWrapper* downUpBackWrapper = new FancyProgressBarWrapper(1000 * animationDurationFactor, this->BigBar, QAbstractAnimation::Direction::Backward);
    downUpWrapper->setEasingCurve(QEasingCurve::OutQuad);
    firstSequence->addAnimation(downUpBackWrapper);

    this->spiralProgressBar = new SpiralProgressBar(this);
    this->spiralProgressBar->SetProgress(0);
    this->spiralProgressBar->setVisible(false);
    QWidget* placeHolderWidget = new QWidget(this->spiralProgressBar);
    this->spiralProgressBar->CentralLayout->addWidget(placeHolderWidget);
    FancyProgressBarWrapper* spiralWrapper = new FancyProgressBarWrapper(20000 * animationDurationFactor, this->spiralProgressBar);
    spiralWrapper->setEasingCurve(QEasingCurve::InSine);
    firstSequence->addAnimation(spiralWrapper);

    QPauseAnimation* thirdPause = new QPauseAnimation(9000 * animationDurationFactor, this);
    firstSequence->addAnimation(thirdPause);

    FancyProgressBarWrapper* rightLeftWrapper = new FancyProgressBarWrapper(800 * animationDurationFactor, this->BigBar, QAbstractAnimation::Direction::Forward);
    rightLeftWrapper->setEasingCurve(QEasingCurve::InExpo);
    firstSequence->addAnimation(rightLeftWrapper);

    QPauseAnimation* fourthPause = new QPauseAnimation(100 * animationDurationFactor, this);
    firstSequence->addAnimation(fourthPause);

    FancyProgressBarWrapper* rightLeftBackWrapper = new FancyProgressBarWrapper(3000 * animationDurationFactor, this->BigBar, QAbstractAnimation::Direction::Backward);
    rightLeftWrapper->setEasingCurve(QEasingCurve::InQuad);
    firstSequence->addAnimation(rightLeftBackWrapper);

    this->tiledProgressBar = new TiledProgressBar(this);
    this->tiledProgressBar->SetProgress(0);
    this->tiledProgressBar->setVisible(false);
    FancyProgressBarWrapper* tiledWrapper = new FancyProgressBarWrapper(25000 * animationDurationFactor, this->tiledProgressBar);
    tiledWrapper->setEasingCurve(QEasingCurve::InSine);
    firstSequence->addAnimation(tiledWrapper);

    QPauseAnimation* fithPause = new QPauseAnimation(8000 * animationDurationFactor, this);
    firstSequence->addAnimation(fithPause);

    FancyProgressBarWrapper* leftRightWrapper = new FancyProgressBarWrapper(800 * animationDurationFactor, this->BigBar);
    firstSequence->addAnimation(leftRightWrapper);

    this->doorStyleProgressBar = new DoorStyleProgressBar(40, this);
    this->doorStyleProgressBar->SetProgress(0);
    gameWindow->LeftGLayout->addWidget(this->doorStyleProgressBar, 0, 0);

    FancyProgressBarWrapper* leftDoorWrapper = new FancyProgressBarWrapper(6000 * animationDurationFactor, this->doorStyleProgressBar);
    leftDoorWrapper->setEasingCurve(QEasingCurve::OutSine);
    firstSequence->addAnimation(leftDoorWrapper);

    FancyProgressBarWrapper* leftRightBackWrapper = new FancyProgressBarWrapper(800 * animationDurationFactor, this->BigBar, QAbstractAnimation::Direction::Backward);
    firstSequence->addAnimation(leftRightBackWrapper);

    FancyProgressBarWrapper* leftDoorWrapperBack = new FancyProgressBarWrapper(6000 * animationDurationFactor, this->doorStyleProgressBar, QAbstractAnimation::Direction::Backward);
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
       QPixmap* logo = new QPixmap(":/RcRessources/ThankYouForPlaying.png");
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
        this->endScoreWidget->raise();
        this->endGameModule->GetRevealAnimation()->start();
    });

    this->endScoreWidget = new EndScoreWidget(this);
    vboxLayout->addWidget(this->endScoreWidget);

    this->endGameModule = new EndGameModuleWidget(this, gameWindow);
    this->endScoreWidget->centralLayout->addWidget(this->endGameModule);


    this->endScoreWidget->Update(0);

    this->endScoreWidget->setVisible(false);

    firstSequence->start(QAbstractAnimation::DeletionPolicy::DeleteWhenStopped);
}

void firstBarSequence(QProgressBar* target, QSequentialAnimationGroup* sequence, float durationFactor, QWidget* parent)
{
    {
        QPropertyAnimation* pA0 = new QPropertyAnimation(target, "value", parent);
        pA0->setStartValue(0);
        pA0->setEndValue(5);
        pA0->setDuration(5000 * durationFactor);
        sequence->addAnimation(pA0);
    }

    sequence->addPause(500 * durationFactor);

    {
        QPropertyAnimation* pA1 = new QPropertyAnimation(target, "value", parent);
        pA1->setStartValue(5);
        pA1->setEndValue(17);
        pA1->setDuration(2000 * durationFactor);
        sequence->addAnimation(pA1);
    }

    sequence->addPause(900 * durationFactor);

    {
        QPropertyAnimation* pA1 = new QPropertyAnimation(target, "value", parent);
        pA1->setStartValue(17);
        pA1->setEndValue(31);
        pA1->setDuration(1500 * durationFactor);
        sequence->addAnimation(pA1);
    }

    sequence->addPause(400 * durationFactor);

    {
        QPropertyAnimation* pA1 = new QPropertyAnimation(target, "value", parent);
        pA1->setStartValue(31);
        pA1->setEndValue(42);
        pA1->setDuration(2000 * durationFactor);
        sequence->addAnimation(pA1);
    }

    sequence->addPause(800 * durationFactor);

    {
        QPropertyAnimation* pA1 = new QPropertyAnimation(target, "value", parent);
        pA1->setStartValue(42);
        pA1->setEndValue(69);
        pA1->setDuration(4500 * durationFactor);
        sequence->addAnimation(pA1);
    }

    sequence->addPause(3500 * durationFactor);

    {
        QPropertyAnimation* pA1 = new QPropertyAnimation(target, "value", parent);
        pA1->setStartValue(69);
        pA1->setEndValue(95);
        pA1->setDuration(1100 * durationFactor);
        sequence->addAnimation(pA1);
    }

    sequence->addPause(2000 * durationFactor);

    {
        QPropertyAnimation* pA1 = new QPropertyAnimation(target, "value", parent);
        pA1->setStartValue(95);
        pA1->setEndValue(98);
        pA1->setDuration(4000 * durationFactor);
        sequence->addAnimation(pA1);
    }

    sequence->addPause(2000 * durationFactor);

    {
        QPropertyAnimation* pA1 = new QPropertyAnimation(target, "value", parent);
        pA1->setStartValue(98);
        pA1->setEndValue(99);
        pA1->setDuration(4000 * durationFactor);
        sequence->addAnimation(pA1);
    }

    sequence->addPause(3000 * durationFactor);
}

void EndGameWidget::Update(float dt)
{
    this->time += dt;
    this->endScoreWidget->Update(this->time);
}


EndGameModuleWidget::EndGameModuleWidget(QWidget* parent, GameWindow* gameWindow) : QFrame(parent)
{
    this->gamewindow = gameWindow;

    QSizePolicy sizePolicy;
    sizePolicy.setRetainSizeWhenHidden(true);

    this->setFrameStyle(QFrame::Box | QFrame::Sunken);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout* vLayout = new QVBoxLayout(this);
    vLayout->setMargin(0);

    vLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Fixed, QSizePolicy::Expanding));

    this->titleLabel = new QLabel("Congratulation!", this);
    this->titleLabel->setAlignment(Qt::AlignCenter);
    vLayout->addWidget(this->titleLabel);
    sizePolicy = this->titleLabel->sizePolicy();
    sizePolicy.setRetainSizeWhenHidden(true);
    this->titleLabel->setSizePolicy(sizePolicy);
    this->titleLabel->setVisible(false);

    this->subtitleLabel = new QLabel("You have not installed the game.", this);
    this->subtitleLabel->setAlignment(Qt::AlignCenter);
    vLayout->addWidget(this->subtitleLabel);
    vLayout->addSpacing(15);
    sizePolicy = this->subtitleLabel->sizePolicy();
    sizePolicy.setRetainSizeWhenHidden(true);
    this->subtitleLabel->setSizePolicy(sizePolicy);
    this->subtitleLabel->setVisible(false);

    QVBoxLayout* vvLayout = new QVBoxLayout();
    vvLayout->setSpacing(2);
    vvLayout->setMargin(0);

    this->finalScoreLabel = new QLabel("Your finalScore is");
    this->finalScoreLabel->setAlignment(Qt::AlignCenter);
    sizePolicy = this->finalScoreLabel->sizePolicy();
    sizePolicy.setRetainSizeWhenHidden(true);
    vvLayout->addWidget(this->finalScoreLabel);
    this->finalScoreLabel->setSizePolicy(sizePolicy);
    this->finalScoreLabel->setVisible(false);

    this->scoreBar = new QProgressBar(this);
    this->scoreBar->setTextVisible(false);
    QSizePolicy barPolicy = this->scoreBar->sizePolicy();
    barPolicy.setRetainSizeWhenHidden(true);
    this->scoreBar->setSizePolicy(barPolicy);
    vvLayout->addWidget(this->scoreBar);
    this->scoreBar->setVisible(false);

    this->scoreLabel = new QLabel(this);
    this->scoreLabel->setText("0");
    this->scoreLabel->setAlignment(Qt::AlignCenter);
    sizePolicy = this->scoreLabel->sizePolicy();
    sizePolicy.setRetainSizeWhenHidden(true);
    this->scoreLabel->setSizePolicy(sizePolicy);
    vvLayout->addWidget(this->scoreLabel);
    this->scoreLabel->setVisible(false);

    vLayout->addLayout(vvLayout);

    vLayout->addSpacing(15);

    QHBoxLayout* hLayout = new QHBoxLayout();
    vLayout->addItem(hLayout);
    this->sendMessage = new QPushButton(this);
    this->sendMessage->setText("Tell me your Score");
    sizePolicy = this->sendMessage->sizePolicy();
    sizePolicy.setRetainSizeWhenHidden(true);
    this->sendMessage->setSizePolicy(sizePolicy);
    this->sendMessage->setVisible(false);

    hLayout->addWidget(this->sendMessage);

    this->exit = new QPushButton(this);
    this->exit->setText("Exit");
    this->exit->setVisible(false);
    sizePolicy = this->exit->sizePolicy();
    sizePolicy.setRetainSizeWhenHidden(true);
    this->exit->setSizePolicy(sizePolicy);
    hLayout->addWidget(this->exit);

    vLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Fixed, QSizePolicy::Expanding));
    int finalScore = AutoClicker::ScoreFormula(this->gamewindow->currentFrame);
    QString scoreString = SWIUtils::FormatScore(finalScore);

    connect(this->sendMessage, &QPushButton::clicked, this, [scoreString](){
        QString twitter = QString("https://twitter.com/intent/tweet?text=@Redoxee\n I've scored %1 on the SWInstaller.").arg(scoreString);
        QDesktopServices::openUrl(QUrl(twitter));
    });

    connect(this->exit, &QPushButton::clicked, this, [this](){
        this->gamewindow->close();
    });
}

QAbstractAnimation* EndGameModuleWidget::GetRevealAnimation()
{
    int pauseDuration = 2000;

    QSequentialAnimationGroup* sequence = new QSequentialAnimationGroup(this);
    QPauseAnimation* pauseAnimation = sequence->addPause(pauseDuration);
    connect(pauseAnimation, &QAbstractAnimation::finished, this, [this](){
        this->titleLabel->setVisible(true);
    });

    pauseAnimation = sequence->addPause(pauseDuration);
    connect(pauseAnimation, &QAbstractAnimation::finished, this, [this](){
        this->subtitleLabel->setVisible(true);
    });


    pauseAnimation = sequence->addPause(pauseDuration);
    connect(pauseAnimation, &QAbstractAnimation::finished, this, [this](){
        this->finalScoreLabel->setVisible(true);
    });

    pauseAnimation = sequence->addPause(pauseDuration);
    connect(pauseAnimation, &QAbstractAnimation::finished, this, [this](){
        this->scoreLabel->setVisible(true);
        this->scoreBar->setVisible(true);
    });

    int finalScore = AutoClicker::ScoreFormula(this->gamewindow->currentFrame);
    int maxScore = AutoClicker::ScoreFormula(0);
    int percentage = (finalScore * 100) / maxScore;

    PropertyTextAnimation* progressAnimation = new PropertyTextAnimation(this->scoreBar, this->scoreLabel, finalScore, this);
    progressAnimation->setDuration(12000);
    progressAnimation->setStartValue(0);
    progressAnimation->setEndValue(percentage);
    progressAnimation->setEasingCurve(QEasingCurve::InOutQuart);
    sequence->addAnimation(progressAnimation);

    pauseAnimation = sequence->addPause(pauseDuration * 2);
    connect(pauseAnimation, &QAbstractAnimation::finished, this, [this](){
        this->sendMessage->setVisible(true);
        this->exit->setVisible(true);
    });


    return sequence;
}

PropertyTextAnimation::PropertyTextAnimation(QProgressBar* progressBar, QLabel* targetLabel, int endValue, QWidget* parent): QPropertyAnimation(progressBar, "value", parent)
{
    this->label = targetLabel;
    this->endValue = endValue;
}

void PropertyTextAnimation::updateCurrentValue(const QVariant &value)
{
    QPropertyAnimation::updateCurrentValue(value);
    int p = value.toInt();
    int score = this->endValue * p / 100;
    this->label->setText(QString::number(score));
}

void PropertyTextAnimation::updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    QPropertyAnimation::updateState(newState, oldState);

    if(oldState == QAbstractAnimation::Running && newState == QAbstractAnimation::State::Stopped)
    {
        this->label->setText(QString::number(this->endValue));
    }
}
