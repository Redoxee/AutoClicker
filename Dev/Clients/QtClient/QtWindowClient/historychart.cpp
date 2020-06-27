#include "historychart.h"

#include <QHBoxLayout>
#include <QSpacerItem>

#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QLogValueAxis>
#include <QtCharts/QChartView>
#include <QGraphicsLayout>

HistoryChart::HistoryChart(QWidget* parent) : QFrame(parent)
{

    this->scoreHistory = new int[this->historySize];
    this->scoreDeltaHistory = new int[this->historySize];
    for(int i = 0; i < this->historySize; ++i)
    {
        this->scoreHistory[i] = 0;
        this->scoreDeltaHistory[i] = 0;
    }

    this->historyCursor = 0;

    QVBoxLayout* vBoxLayout = new QVBoxLayout(this);
    vBoxLayout->setMargin(0);
    vBoxLayout->setSpacing(0);

    QHBoxLayout* hLayout = new QHBoxLayout();
    QSpacerItem* spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed);
    hLayout->addSpacerItem(spacer);
    hLayout->setMargin(0);
    hLayout->setSpacing(0);
    this->scoreButton = new QPushButton(this);
    this->scoreButton->setText("Score");
    this->deltaButton = new QPushButton(this);
    this->deltaButton->setText("Speed");
    hLayout->addWidget(this->scoreButton);
    hLayout->addWidget(this->deltaButton);
    this->scoreButton->setFixedSize(50, 20);
    this->deltaButton->setFixedSize(50, 20);
    vBoxLayout->addLayout(hLayout);

    this->historySeries = new QtCharts::QLineSeries(this);
    this->chart = new QtCharts::QChart();
    this->chart->legend()->hide();
    this->chart->addSeries(this->historySeries);
    this->chart->layout()->setContentsMargins(0, 0, 0, 0);
    this->chart->setMargins(QMargins(0, 0, 0, 0));
    this->chart->setBackgroundRoundness(0);

    this->historyYAxis = new QtCharts::QLogValueAxis(this);
    this->historyYAxis->setMin(0);
    this->historyYAxis->setLabelFormat("%g bits");
    QFont font = this->historyYAxis->labelsFont();
    font.setPointSize(6);
    this->historyYAxis->setLabelsFont(font);
    this->historyYAxis->setBase(10.0);
    this->historyYAxis->setMinorTickCount(0);
    this->chart->addAxis(this->historyYAxis, Qt::AlignLeft);
    this->historySeries->attachAxis(this->historyYAxis);
    this->historyChartView = new QtCharts::QChartView(this->chart, this);
    this->historyChartView->setRenderHint(QPainter::Antialiasing, true);
    this->historyChartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->historyChartView->setMinimumHeight(100);
    this->historyChartView->setMaximumHeight(100);

    vBoxLayout->addWidget(this->historyChartView);
}

void HistoryChart::PushToScoreHistory(int currentScore)
{
    this->scoreHistory[this->historyCursor] = currentScore;

    int rollingAverageSize = 2;
    int cursor = this->historyCursor;
    int accumulator = 0;
    for(int index = 0; index < rollingAverageSize; ++index)
    {
        int prevCursor = (cursor - 1) % this->historySize;
        int delta = this->scoreHistory[cursor] - this->scoreHistory[prevCursor];
        accumulator += delta;
        cursor = prevCursor;
    }

    this->scoreDeltaHistory[this->historyCursor] = accumulator / rollingAverageSize;

    this->historyCursor = (this->historyCursor + 1) % this->historySize;
}

void HistoryChart::RefreshHistory()
{
    this->historySeries->clear();
    int maxValue = 100;

    for(int i = 0; i < this->historySize; ++i)
    {
        int index = (this->historyCursor + i) % this->historySize;
        int score = this->scoreHistory[index];
        if(score > maxValue)
        {
            maxValue = score;
        }
    }

    maxValue *= 1.1;
    int minValue = 1;
    if(maxValue > 1000)
    {
        minValue = maxValue / 1000;
    }

    this->historyYAxis->setMax(maxValue);
    this->historyYAxis->setMin(minValue);

    for(int i = 0; i < this->historySize; ++i)
    {
        int index = (this->historyCursor + i) % this->historySize;
        int score = this->scoreHistory[index];

        if(i > 0 && i < (this->historySize - 1))
        {
            if(score == this->scoreHistory[index - 1] && score == this->scoreHistory[index + 1])
            {
                continue;
            }
        }

        if(score < 1)
        {
            score = 1;
        }

        float xPos = static_cast<float>(i) / static_cast<float>(this->historySize);
        this->historySeries->append(xPos, score);
    }
}

void HistoryChart::ResetHistory()
{
    for(int index = 0; index < this->historySize; ++index)
    {
        this->scoreHistory[index] = 0;
    }
}
