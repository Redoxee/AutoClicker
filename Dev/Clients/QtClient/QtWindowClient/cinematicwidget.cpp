#include "cinematicwidget.h"

CinematicWidget::CinematicWidget(QWidget *parent, GameWindow* gameWindow) : QWidget(parent)
{
    this->gameWindow = gameWindow;

    this->SetupUI();

    this->animationSequence = new QSequentialAnimationGroup();

    QPropertyAnimation* progressBarAnimation = new QPropertyAnimation(this->progressBar,"value");
    progressBarAnimation->setStartValue(0);
    progressBarAnimation->setEndValue(99);
    progressBarAnimation->setDuration(1000);
    this->animationSequence->addAnimation(progressBarAnimation);

    this->windowShakeAnimation = new WindowShakeAnimation(this->gameWindow, 1000, 15.0f, 0.05f);
    this->animationSequence->addAnimation(this->windowShakeAnimation);

    connect(this->animationSequence, SIGNAL(finished()), this, SLOT(MainSequenceFinished()));
    connect(this->bottomButton, SIGNAL(clicked()), this, SLOT(GameStartPressed()));

    this->StartCinematic();
}

void CinematicWidget::SetupUI()
{
    QVBoxLayout* vLayout = new QVBoxLayout(this);
    this->progressBar = new QProgressBar();
    vLayout->addWidget(this->progressBar);

    this->bottomButton = new QPushButton();
    this->bottomButton->setText("Start Manual Installation!");
    this->gameWindow->BottomBox->addButton(this->bottomButton, QDialogButtonBox::ButtonRole::YesRole);
    this->bottomButton->hide();
}

void CinematicWidget::StartCinematic()
{
    this->animationSequence->start();
}

void CinematicWidget::MainSequenceFinished()
{
    this->bottomButton->show();
}

void CinematicWidget::GameStartPressed()
{
    this->gameWindow->BottomBox->removeButton(this->bottomButton);
    delete this->bottomButton;

    this->gameWindow->GotToScreen(Screens::GameScreen);
}
