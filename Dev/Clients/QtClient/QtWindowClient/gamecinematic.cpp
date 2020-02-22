#include "gamecinematic.h"

GameCinematic::GameCinematic(QWidget *parent, QMainWindow* mainWindow) : QWidget(parent)
{
    this->SetupUI();

    this->animationSequence = new QSequentialAnimationGroup();

    QPropertyAnimation* progressBarAnimation = new QPropertyAnimation(this->progressBar,"value");
    progressBarAnimation->setStartValue(0);
    progressBarAnimation->setEndValue(100);
    progressBarAnimation->setDuration(1000);
    this->animationSequence->addAnimation(progressBarAnimation);

    this->windowShakeAnimation = new WindowShakeAnimation(mainWindow, 1000, 15.0f, 0.05f);
    this->animationSequence->addAnimation(this->windowShakeAnimation);
}

void GameCinematic::SetupUI()
{
    QVBoxLayout* vLayout = new QVBoxLayout(this);
    this->progressBar = new QProgressBar();
    vLayout->addWidget(this->progressBar);
}

void GameCinematic::StartCinematic()
{
    this->animationSequence->start();
}
