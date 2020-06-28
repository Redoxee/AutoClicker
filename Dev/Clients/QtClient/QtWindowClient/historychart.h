#ifndef HISTORYCHART_H
#define HISTORYCHART_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QFrame>
#include <QRadioButton>
#include <QGroupBox>
#include <QPen>

namespace QtCharts
{
    class QChart;
    class QLineSeries;
    class QChartView;
    class QLogValueAxis;
};

class HistoryChart : public QFrame
{
    Q_OBJECT
    const int historySize = 100;

    enum class DisplayMode
    {
        Score,
        Delta,
    };

public:
    HistoryChart(QWidget* parent);

    void PushToScoreHistory(int currentScore);
    void RefreshHistory();
    void ResetHistory();

private slots:
    void DisplayModeChangePressed(DisplayMode displayMode);

private:

    DisplayMode mode = DisplayMode::Score;
    int historyCursor;
    int* scoreHistory = nullptr;
    int* scoreDeltaHistory = nullptr;
    QtCharts::QLineSeries* historySeries = nullptr;
    QtCharts::QChartView* historyChartView = nullptr;
    QtCharts::QLogValueAxis* historyYAxis = nullptr;
    QtCharts::QChart* chart = nullptr;

    QPen* scorePen = nullptr;
    QPen* deltaPen = nullptr;

    QRadioButton* scoreButton = nullptr;
    QRadioButton* deltaButton = nullptr;
};

#endif // HISTORYCHART_H
