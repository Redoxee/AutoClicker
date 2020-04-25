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

    QtCharts::QLineSeries *series = new QtCharts::QLineSeries();
    series->append(0, 5);
    series->append(2, 50);
    series->append(3, 75);
    series->append(7, 100);
    series->append(10, 1000);
    *series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);
    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->legend()->hide();
    chart->addSeries(series);

    QtCharts::QValueAxis *axisX = new QtCharts::QValueAxis();
    axisX->setLabelFormat("%i");
    axisX->setTickCount(series->count());
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QtCharts::QLogValueAxis *axisY = new QtCharts::QLogValueAxis();
    axisY->setLabelFormat("%g");
    axisY->setBase(10.0);
    axisY->setMinorTickCount(-1);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    vboxLayout->addWidget(chartView);
}

void EndGameWidget::Update(float dt)
{
    this->time += dt;

    float animTime = pow(this->time * .00005, 2.);
    //this->spiralProgressBar->SetValue(animTime);
    //this->tiledProgressBar->SetValue(animTime);
    //this->crissCrossProgressBar->SetValue(animTime);
    //this->doorStyleProgressBar->SetValue(animTime);
}
