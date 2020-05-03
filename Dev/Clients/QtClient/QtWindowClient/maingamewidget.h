#ifndef MAINGAMEWIDGET_H
#define MAINGAMEWIDGET_H

#include <QWidget>
#include <QCloseEvent>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QGridLayout>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include <QtCharts>

#include <vector>

struct ServerGameplayState;

class UpdateWorker;
class ScaledProgressBar;

class GameWindow;

class MainGameWidget : public QWidget
{
    Q_OBJECT
public:
    MainGameWidget(GameWindow* gameWindow);
    ~MainGameWidget();

public slots:
    void handleClick();
    void UpgradeButtonClick(int buttonIndex);
    void handleHttpRequest(QNetworkReply* reply);

    void refreshData(ServerGameplayState* serverData);

private:
    void SetupUI();

    QLabel* scoreValueLabel = nullptr;
    QLabel* frameValueLabel = nullptr;
    QLabel* clickValueLabel = nullptr;
    QPushButton* clickerButton = nullptr;

    QPushButton* finishButton = nullptr;

    QGridLayout*  UpgradeLayout = nullptr;

    UpdateWorker* updateWorker = nullptr;

    QNetworkAccessManager *manager = nullptr;
    QNetworkRequest request;

    ScaledProgressBar* ProgressBar[4];
    std::vector<QPushButton*> UpgradeButtons;

    void RefreshProgressBars(int score);
    QPushButton CreateUpgradeButton(QString Label);

    int lastRefreshedFrame;
    bool isDirty;
    bool isFinished;
    bool isSleeping;

    GameWindow* gameWindow;

    int realCurrentScore = 0;
    int displayedScore = 0;

    int timeAccumulator = 0;

    const int historySize = 100;
    int historyCursor;
    int* scoreHistory = nullptr;
    QtCharts::QLineSeries* historySeries = nullptr;
    QtCharts::QChartView* historyChartView = nullptr;
    QtCharts::QLogValueAxis* historyYAxis = nullptr;

    void pushToScoreHistory(int score);
    void refreshHistory();

    void Update(int dt);

    void onFinishButtonClicked();
};

#endif // MAINGAMEWIDGET_H
