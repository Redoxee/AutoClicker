#ifndef MAINGAMEWIDGET_H
#define MAINGAMEWIDGET_H

#include <QWidget>
#include <QCloseEvent>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QGridLayout>
#include <QFrame>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include <vector>

namespace QtCharts
{
    class QLineSeries;
    class QChartView;
    class QLogValueAxis;
};

struct ServerGameplayState;
class UpdateWorker;
class ScaledProgressBar;
class GameWindow;
class UpgradeButton;

class UpgradeSlot : public QFrame
{
public:
    UpgradeSlot(QWidget* parrent = nullptr);

    QBoxLayout* MainLayout = nullptr;
    QLabel* InstanceBought = nullptr;
    QLabel* MainLabel = nullptr;
    QLabel* SubLabel = nullptr;
    UpgradeButton* UpgradeButtons = nullptr;
};

class ScoreSlot : public QFrame
{
public:
    ScoreSlot(QWidget* parent = nullptr);

    QLabel* ScoreLabel = nullptr;
    QLabel* FactorLabel = nullptr;

    UpgradeButton* UpgradeButtons = nullptr;
};

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

    ScoreSlot* scoreSlot = nullptr;

    QLabel* frameValueLabel = nullptr;

    QPushButton* clickerButton = nullptr;
    QPushButton* clickMenuButton = nullptr;

    QPushButton* finishButton = nullptr;
    UpdateWorker* updateWorker = nullptr;

    QNetworkAccessManager *manager = nullptr;
    QNetworkRequest request;

    ScaledProgressBar* ProgressBar[4];

    UpgradeSlot* clickUpgradeSlot = nullptr;

    UpgradeSlot* firstGeneratorSlot = nullptr;

    UpgradeSlot* secondGeneratorSlot = nullptr;

    void RefreshProgressBars(int score);

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
