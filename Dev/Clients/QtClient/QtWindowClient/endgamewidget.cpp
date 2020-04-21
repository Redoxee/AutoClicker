#include "endgamewidget.h"

EndGameWidget::EndGameWidget(GameWindow* gameWindow) : QWidget(gameWindow)
{
    this->gameWindow = gameWindow;

    this->SetupUI();
    this->updateWorker = new UpdateWorker();

    this->time = 0;

    connect(this->updateWorker, &UpdateWorker::Update, this, &EndGameWidget::Update);
}

void EndGameWidget::SetupUI()
{
    QVBoxLayout* vboxLayout = new QVBoxLayout(this);
    vboxLayout->setMargin(0);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->gridProgressBar = new GridProgressBar(this,8, 12);
    this->gridProgressBar->hide();

    /*
    this->spiralProgressBar = new SpiralProgressBar(this);
    vboxLayout->addWidget(this->spiralProgressBar);
    this->spiralProgressBar->SetValue(0);
    QWidget* placeHolderWidget = new QWidget(this->spiralProgressBar);
    this->spiralProgressBar->CentralLayout->addWidget(placeHolderWidget);
    */

    /*
    this->tiledProgressBar = new TiledProgressBar(this);
    vboxLayout->addWidget(this->tiledProgressBar);
    this->tiledProgressBar->SetValue(0);
    */

    this->crissCrossProgressBar = new CrissCrossProgressBar(30, 15, 8, this);
    vboxLayout->addWidget(this->crissCrossProgressBar);
    this->crissCrossProgressBar->SetValue(0);
}

void EndGameWidget::Update(float dt)
{
    this->time += dt;

    float animTime = pow(this->time * .00005, 2.);
    //this->spiralProgressBar->SetValue(animTime);
    //this->tiledProgressBar->SetValue(animTime);
    this->crissCrossProgressBar->SetValue(animTime);
}
