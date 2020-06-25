#include "cinematicwidget.h"
#include "gamewindow.h"
#include "windowshakeanimation.h"
#include "windowslideanimation.h"
#include "eventlogger.h"

CinematicWidget::CinematicWidget(QWidget *parent, GameWindow* gameWindow) : QWidget(parent)
{
    this->gameWindow = gameWindow;

    this->SetupUI();

    this->animationSequence = new QSequentialAnimationGroup();

    QPropertyAnimation* progressBarAnimation = new QPropertyAnimation(this->progressBar,"value");
    progressBarAnimation->setStartValue(0);
    progressBarAnimation->setEndValue(34);
    progressBarAnimation->setDuration(4000);
    this->animationSequence->addAnimation(progressBarAnimation);

    this->animationSequence->addPause(3000);

    progressBarAnimation = new QPropertyAnimation(this->progressBar,"value");
    progressBarAnimation->setStartValue(34);
    progressBarAnimation->setEndValue(42);
    progressBarAnimation->setDuration(6000);
    this->animationSequence->addAnimation(progressBarAnimation);

    this->animationSequence->addPause(4000);

    progressBarAnimation = new QPropertyAnimation(this->progressBar,"value");
    progressBarAnimation->setStartValue(42);
    progressBarAnimation->setEndValue(45);
    progressBarAnimation->setDuration(3000);
    this->animationSequence->addAnimation(progressBarAnimation);

    this->animationSequence->addPause(8000);

    progressBarAnimation = new QPropertyAnimation(this->progressBar,"value");
    progressBarAnimation->setStartValue(45);
    progressBarAnimation->setEndValue(99);
    progressBarAnimation->setDuration(350);
    this->animationSequence->addAnimation(progressBarAnimation);

    WindowShakeAnimation* windowShakeAnimation = new WindowShakeAnimation(this->gameWindow, 750, 15.0f, 0.05f);
    this->animationSequence->addAnimation(windowShakeAnimation);

    this->animationSequence->addPause(4300);

    progressBarAnimation = new QPropertyAnimation(this->progressBar,"value");
    progressBarAnimation->setStartValue(99);
    progressBarAnimation->setEndValue(50);
    progressBarAnimation->setDuration(1000);
    progressBarAnimation->setEasingCurve(QEasingCurve::OutCubic);
    this->animationSequence->addAnimation(progressBarAnimation);

    this->animationSequence->addPause(1000);

    progressBarAnimation = new QPropertyAnimation(this->progressBar,"value");
    progressBarAnimation->setStartValue(50);
    progressBarAnimation->setEndValue(40);
    progressBarAnimation->setDuration(500);
    progressBarAnimation->setEasingCurve(QEasingCurve::InOutSine);
    this->animationSequence->addAnimation(progressBarAnimation);

    progressBarAnimation = new QPropertyAnimation(this->progressBar,"value");
    progressBarAnimation->setStartValue(40);
    progressBarAnimation->setEndValue(99);
    progressBarAnimation->setDuration(300);
    progressBarAnimation->setEasingCurve(QEasingCurve::InExpo);
    this->animationSequence->addAnimation(progressBarAnimation);

    WindowSlideAnimation* slideAnimation = new WindowSlideAnimation(this, this->gameWindow, 450, QEasingCurve::OutQuad, QPoint(100, 0));
    this->animationSequence->addAnimation(slideAnimation);

    windowShakeAnimation = new WindowShakeAnimation(this->gameWindow, 750, 15.0f, 0.05f);
    this->animationSequence->addAnimation(windowShakeAnimation);

    this->animationSequence->addPause(1200);

    progressBarAnimation = new QPropertyAnimation(this->progressBar,"value");
    progressBarAnimation->setStartValue(99);
    progressBarAnimation->setEndValue(30);
    progressBarAnimation->setEasingCurve(QEasingCurve::OutCubic);
    progressBarAnimation->setDuration(1000);
    this->animationSequence->addAnimation(progressBarAnimation);

    this->animationSequence->addPause(1000);

    progressBarAnimation = new QPropertyAnimation(this->progressBar,"value");
    progressBarAnimation->setStartValue(30);
    progressBarAnimation->setEndValue(20);
    progressBarAnimation->setDuration(500);
    progressBarAnimation->setEasingCurve(QEasingCurve::InOutSine);
    this->animationSequence->addAnimation(progressBarAnimation);

    progressBarAnimation = new QPropertyAnimation(this->progressBar,"value");
    progressBarAnimation->setStartValue(20);
    progressBarAnimation->setEndValue(99);
    progressBarAnimation->setDuration(300);
    progressBarAnimation->setEasingCurve(QEasingCurve::InExpo);
    this->animationSequence->addAnimation(progressBarAnimation);

    slideAnimation = new WindowSlideAnimation(this, this->gameWindow, 450, QEasingCurve::OutQuad, QPoint(200, 0));
    this->animationSequence->addAnimation(slideAnimation);

    windowShakeAnimation = new WindowShakeAnimation(this->gameWindow, 750, 15.0f, 0.05f);
    this->animationSequence->addAnimation(windowShakeAnimation);

    this->animationSequence->addPause(1400);

    progressBarAnimation = new QPropertyAnimation(this->progressBar,"value");
    progressBarAnimation->setStartValue(99);
    progressBarAnimation->setEndValue(1);
    progressBarAnimation->setDuration(350);
    this->animationSequence->addAnimation(progressBarAnimation);

    slideAnimation = new WindowSlideAnimation(this, this->gameWindow, 400, QEasingCurve::OutQuad, QPoint(-300, 0));
    this->animationSequence->addAnimation(slideAnimation);

    windowShakeAnimation = new WindowShakeAnimation(this->gameWindow, 500, 15.0f, 0.05f);
    this->animationSequence->addAnimation(windowShakeAnimation);

    this->animationSequence->addPause(600);

    connect(this->animationSequence, SIGNAL(finished()), this, SLOT(MainSequenceFinished()));
    connect(this->bottomButton, SIGNAL(clicked()), this, SLOT(GameStartPressed()));

    this->StartCinematic();
}

void CinematicWidget::SetupUI()
{
    QVBoxLayout* vLayout = new QVBoxLayout(this);

    QLabel* tempLabel = new QLabel();
    tempLabel->setText("<Insert Here> UI To fake a installer that fail at the end.");
    vLayout->addWidget(tempLabel);

    this->eventLogger = new EventLogger(this);
    vLayout->addWidget(this->eventLogger);

    this->progressBar = new QProgressBar();
    vLayout->addWidget(this->progressBar);

    this->bottomButton = new QPushButton();
    this->bottomButton->setText("Start Manual Installation!");
    this->gameWindow->BottomBox->addWidget(this->bottomButton);
    this->bottomButton->hide();
}

void CinematicWidget::StartCinematic()
{
    this->animationSequence->start();
}

void CinematicWidget::MainSequenceFinished()
{
    QMessageBox* messageBox = new QMessageBox(QMessageBox::Warning,
    "Whoops",
    "Something went wrong.\n\nPlease try the manual installation mode",
    QMessageBox::NoButton, this);

    QPushButton* manualInstall = messageBox->addButton("Manual installation mode", QMessageBox::ButtonRole::YesRole);
    messageBox->exec();

    if(messageBox->clickedButton() == manualInstall)
    {
        this->gameWindow->BottomBox->removeWidget(this->bottomButton);
        delete this->bottomButton;

        this->gameWindow->GotToScreen(Screens::GameScreen);
    }
    else
    {
        this->bottomButton->setVisible(true);
    }
}

void CinematicWidget::GameStartPressed()
{
    this->gameWindow->BottomBox->removeWidget(this->bottomButton);
    delete this->bottomButton;

    this->gameWindow->GotToScreen(Screens::GameScreen);
}
