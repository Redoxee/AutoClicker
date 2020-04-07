#ifndef MAINGAMEWIDGET_H
#define MAINGAMEWIDGET_H

#include <QApplication>
#include <QObject>
#include <QWidget>
#include <QCloseEvent>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QGridLayout>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QThread>

#include <vector>

#include "gamewindow.h"
#include "upgrade.h"
#include "upgradebutton.h"
#include "updateworker.h"
#include "scaledprogressbar.h"

class MainGameWidget : public QWidget
{
    Q_OBJECT
public:
    MainGameWidget(GameWindow* gameWindow);
    ~MainGameWidget();

public slots:
    void handleClick();
    void UpgradeButtonClick(UpgradeButton* button);
    void handleHttpRequest(QNetworkReply* reply);

    void refreshData(QJsonObject jsonData);

private:
    void SetupUI();

    QLabel* scoreValueLabel;
    QLabel* frameValueLabel;
    QLabel* clickValueLabel;
    QPushButton* clickerButton;

    QGridLayout*  UpgradeLayout;

    UpdateWorker* updateWorker;

    QNetworkAccessManager *manager;
    QNetworkRequest request;

    ScaledProgressBar* ProgressBar[4];
    std::vector<UpgradeButton*> UpgradeButtons;

    void RefreshProgressBars(int score);
    QPushButton CreateUpgradeButton(QString Label);

    int lastRefreshedFrame;
    bool isDirty;

    GameWindow* gameWindow;

    int realCurrentScore = 0;
    int displayedScore = 0;

    int timeAccumulator = 0;

private slots:
    void Update(int dt);
};

#endif // MAINGAMEWIDGET_H
