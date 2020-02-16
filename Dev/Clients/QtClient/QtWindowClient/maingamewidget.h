#ifndef MAINGAMEWIDGET_H
#define MAINGAMEWIDGET_H

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

#include <QThread>

#include <vector>

#include "refresherworker.h"
#include "upgrade.h"
#include "upgradebutton.h"


class MainGameWidget : public QWidget
{
public:
    MainGameWidget(QWidget* parent);
    ~MainGameWidget();

public slots:
    void handleClick();
    void UpgradeButtonClick(UpgradeButton* button);
    void handleHttpRequest(QNetworkReply* reply);

    void refreshData(QJsonObject jsonData);
    void aboutToQuit();

private:

    QLabel* scoreValueLabel;
    QLabel* frameValueLabel;
    QLabel* clickValueLabel;

    QGridLayout *  UpgradeLayout;


    QNetworkAccessManager *manager;
    QNetworkRequest request;

    RefresherWorker* refreshWorker;
    QThread* workerThread;

    QProgressBar* ProgressBar;
    std::vector<UpgradeButton*> UpgradeButtons;

    QPushButton CreateUpgradeButton(QString Label);

    int lastRefreshedFrame;
    bool isDirty;

};

#endif // MAINGAMEWIDGET_H
