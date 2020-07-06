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

namespace ServerUtils
{
    struct ServerGameplayState;
    struct Upgrade;
}

class UpdateWorker;
class ScaledProgressBar;
class GameWindow;
class UpgradeButton;
class EventLogger;
class HistoryChart;

class UpgradeSlot : public QFrame
{
public:
    UpgradeSlot(QWidget* parent = nullptr);

    void SetMainLabelValue(int64_t value);
    void SetSubLabelValue(int64_t value);
    void SetTooltipValue(int64_t value);

    void RefreshDisplay(ServerUtils::Upgrade* mainUpgrade, int mainImpactFactor, ServerUtils::Upgrade* Improve);
    void RefreshComponentVisibility(bool mainVisible, bool mainActive, bool secondVisible, bool secondActive);

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

    EventLogger* eventLogger = nullptr;

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

    HistoryChart* historyChart = nullptr;
    int skipNextHistoryUpdates = 0;

    void Update(int dt);

    void onFinishButtonClicked();
};

#endif // MAINGAMEWIDGET_H
