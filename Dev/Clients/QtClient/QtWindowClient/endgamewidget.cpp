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

    //this->doorStyleProgressBar = new DoorStyleProgressBar(40, this);
    //vboxLayout->addWidget(this->doorStyleProgressBar);

    //this->crissCrossProgressBar = new CrissCrossProgressBar(30, 15, 8, this);
    //this->gameWindow->LeftGLayout->addWidget(this->crissCrossProgressBar, 0,0);
    //vboxLayout->addWidget(this->crissCrossProgressBar);
    //this->crissCrossProgressBar->SetValue(0);

    /*
    QGraphicsScene* grScene = new QGraphicsScene(this);
    QProgressBar* rotatedPB = new QProgressBar();
    QGraphicsView* grView = new QGraphicsView(grScene);
    QGraphicsProxyWidget* proxyWidget = grScene->addWidget(rotatedPB);
    grView->setBackgroundBrush(Qt::BrushStyle::CrossPattern);
    QWidget* vp = grView->viewport();
    proxyWidget->setRotation(0);
    grScene->setSceneRect(0,0,vp->width(), vp->height());
    proxyWidget->setPos(200, 300);
    vboxLayout->addWidget(vp);
    */

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
    scoreStr = scoreStr.arg(8888888);
    scoreMessage->setText(scoreStr);

    spacer = new QSpacerItem(0,0, QSizePolicy::Fixed, QSizePolicy::Expanding);
    this->endScoreWidget->centralLayout->addItem(spacer);
}

void EndGameWidget::Update(float dt)
{
    this->time += dt;

    float animTime = pow(this->time * .00005, 2.);
    //this->spiralProgressBar->SetValue(animTime);
    //this->tiledProgressBar->SetValue(animTime);
    //this->crissCrossProgressBar->SetValue(animTime);
    //this->doorStyleProgressBar->SetValue(animTime);
    this->endScoreWidget->Update(this->time);
}
