#include "cinematicwidget.h"

CinematicWidget::CinematicWidget(QWidget *parent, QMainWindow* mainWindow) : QWidget(parent)
{
    this->SetupUI();

    this->animationSequence = new QSequentialAnimationGroup();

    QPropertyAnimation* progressBarAnimation = new QPropertyAnimation(this->progressBar,"value");
    progressBarAnimation->setStartValue(0);
    progressBarAnimation->setEndValue(99);
    progressBarAnimation->setDuration(1000);
    this->animationSequence->addAnimation(progressBarAnimation);

    this->windowShakeAnimation = new WindowShakeAnimation(mainWindow, 1000, 15.0f, 0.05f);
    this->animationSequence->addAnimation(this->windowShakeAnimation);

    this->StartCinematic();
}

void CinematicWidget::SetupUI()
{
    QVBoxLayout* vLayout = new QVBoxLayout(this);
    this->progressBar = new QProgressBar();
    vLayout->addWidget(this->progressBar);
}

void CinematicWidget::StartCinematic()
{
    this->animationSequence->start();
}
