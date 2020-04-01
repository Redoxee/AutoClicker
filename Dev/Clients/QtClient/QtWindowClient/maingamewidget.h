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


    QNetworkAccessManager *manager;
    QNetworkRequest request;

    QProgressBar* ProgressBar[4];
    std::vector<UpgradeButton*> UpgradeButtons;

    void RefreshProgressBars(int score);
    QPushButton CreateUpgradeButton(QString Label);

    int lastRefreshedFrame;
    bool isDirty;

    GameWindow* gameWindow;
};

#endif // MAINGAMEWIDGET_H
