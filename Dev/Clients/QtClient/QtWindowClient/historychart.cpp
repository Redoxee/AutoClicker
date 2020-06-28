#include "historychart.h"

#include <QHBoxLayout>
#include <QSpacerItem>
#include <QColor>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QLogValueAxis>
#include <QtCharts/QChartView>
#include <QGraphicsLayout>
#include <QDebug>

HistoryChart::HistoryChart(QWidget* parent) : QFrame(parent)
{
    this->setFrameStyle(QFrame::Box | QFrame::Sunken);

    this->setFrameStyle(QFrame::Box | QFrame::Sunken);
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::white);
    this->setPalette(pal);
    this->setAutoFillBackground(true);


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

    this->mode = DisplayMode::Score;
    this->scorePen = new QPen(Qt::SolidLine);
    this->deltaPen = new QPen(Qt::SolidLine);

    this->scorePen->setWidth(2);
    this->deltaPen->setWidth(2);

    this->scorePen->setColor(QColorConstants::Blue);
    this->deltaPen->setColor(QColorConstants::Green);

    QHBoxLayout* hLayout = new QHBoxLayout();
    QGroupBox* groupBox = new QGroupBox(this);
    hLayout->setMargin(0);
    hLayout->setSpacing(0);

    QSpacerItem* spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed);
    hLayout->addSpacerItem(spacer);

    groupBox->setLayout(hLayout);
    this->scoreButton = new QRadioButton(this);
    this->scoreButton->setText("Installed bits");
    this->scoreButton->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
    this->deltaButton = new QRadioButton(this);
    this->deltaButton->setText("Installation speed");
    this->deltaButton->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
    hLayout->addWidget(this->scoreButton);
    hLayout->addWidget(this->deltaButton);

    spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed);
    hLayout->addSpacerItem(spacer);

    vBoxLayout->addWidget(groupBox);

    connect(this->scoreButton, &QRadioButton::pressed, this, [=]{ this->DisplayModeChangePressed(DisplayMode::Score); });
    connect(this->deltaButton, &QRadioButton::pressed, this, [=]{ this->DisplayModeChangePressed(DisplayMode::Delta); });
    this->DisplayModeChangePressed(DisplayMode::Score);
    this->scoreButton->setChecked(true);
}

void HistoryChart::PushToScoreHistory(int currentScore)
{
    this->scoreHistory[this->historyCursor] = currentScore;

    int rollingAverageSize = 4;
    int cursor = this->historyCursor;
    int accumulator = 0;
    for(int index = 0; index < rollingAverageSize; ++index)
    {
        int prevCursor = (cursor - 1);
        if(prevCursor < 0)
        {
            prevCursor += this->historySize;
        }

        int delta = this->scoreHistory[cursor] - this->scoreHistory[prevCursor];
        accumulator += delta;
        cursor = prevCursor;
    }

    this->scoreDeltaHistory[this->historyCursor] = fmax(accumulator / rollingAverageSize, 1);

    this->historyCursor = (this->historyCursor + 1) % this->historySize;
}

void HistoryChart::RefreshHistory()
{
    this->historySeries->clear();
    int maxValue = 100;
    int* displayedData = this->scoreHistory;
    if(this->mode == DisplayMode::Delta)
    {
        displayedData = this->scoreDeltaHistory;
    }

    for(int i = 0; i < this->historySize; ++i)
    {
        int index = (this->historyCursor + i) % this->historySize;
        int score = displayedData[index];
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
        int score = displayedData[index];

        if(i > 0 && i < (this->historySize - 1))
        {
            if(score == displayedData[index - 1] && score == displayedData[index + 1])
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

void HistoryChart::DisplayModeChangePressed(HistoryChart::DisplayMode displayMode)
{
    this->mode = displayMode;
    if(displayMode == DisplayMode::Score)
    {
        this->historySeries->setPen(*this->scorePen);
    }
    else if(displayMode == DisplayMode::Delta)
    {
        this->historySeries->setPen(*this->deltaPen);
    }

    this->RefreshHistory();
}
