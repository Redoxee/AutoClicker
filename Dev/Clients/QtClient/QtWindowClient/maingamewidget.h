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

namespace ServerUtils
{
    struct ServerGameplayState;
    struct Upgrade;
}

class UpdateWorker;
class ScaledProgressBar;
class GameWindow;
class UpgradeButton;

class UpgradeSlot : public QFrame
{
public:
    UpgradeSlot(QWidget* parrent = nullptr);

    void SetMainLabelValue(int64_t value);
    void SetSubLabelValue(int64_t value);
    void RefreshDisplay(ServerUtils::Upgrade* mainUpgrade, int mainImpactFactor, ServerUtils::Upgrade* Improve);

    QBoxLayout* MainLayout = nullptr;
    QLabel* InstanceBought = nullptr;
    QLabel* MainLabel = nullptr;
    QLabel* SubLabel = nullptr;
    UpgradeButton* UpgradeButtons = nullptr;

    QString MainPattern;
    QString SecondPattern;
    QString TooltipPattern;
};

class ScoreSlot : public QFrame
{
public:
    ScoreSlot(QWidget* parent = nullptr);

    QLabel* ScoreLabel = nullptr;
    QLabel* FactorLabel = nullptr;
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

    void refreshData(ServerUtils::ServerGameplayState* serverData);

private:
    void SetupUI();

    ScoreSlot* scoreSlot = nullptr;

    QLabel* frameValueLabel = nullptr;

    UpgradeButton* clickerButton = nullptr;

    QPushButton* finishButton = nullptr;
    UpdateWorker* updateWorker = nullptr;

    QNetworkAccessManager *manager = nullptr;
    QNetworkRequest request;

    ScaledProgressBar* ProgressBar[4];

    UpgradeSlot* clickUpgradeSlot = nullptr;
    UpgradeSlot* firstGeneratorSlot = nullptr;
    UpgradeSlot* secondGeneratorSlot = nullptr;
    UpgradeSlot* prestigeSlot = nullptr;

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
    void resetHistory();

    void Update(int dt);

    void onFinishButtonClicked();
};

#endif // MAINGAMEWIDGET_H
