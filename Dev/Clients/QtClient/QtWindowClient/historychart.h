#ifndef HISTORYCHART_H
#define HISTORYCHART_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QFrame>
#include <QPushButton>
#include <vector>

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
public:
    HistoryChart(QWidget* parent);

    void PushToScoreHistory(int currentScore);
    void RefreshHistory();
    void ResetHistory();

private:

    const int historySize = 100;
    int historyCursor;
    int* scoreHistory = nullptr;
    int* scoreDeltaHistory = nullptr;
    QtCharts::QLineSeries* historySeries = nullptr;
    QtCharts::QChartView* historyChartView = nullptr;
    QtCharts::QLogValueAxis* historyYAxis = nullptr;
    QtCharts::QChart* chart = nullptr;

    QPushButton* scoreButton = nullptr;
    QPushButton* deltaButton = nullptr;
};

#endif // HISTORYCHART_H
