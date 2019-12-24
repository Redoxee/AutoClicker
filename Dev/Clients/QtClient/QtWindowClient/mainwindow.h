#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QPushButton>
#include <QLabel>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>

#include <QThread>

#include <vector>

#include "refresherworker.h"
#include "upgrade.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void handleClick();
    void handleHttpRequest(QNetworkReply* reply);

    void refreshData(QJsonObject jsonData);

private:
    Ui::MainWindow *ui;

    QLabel* scoreValueLabel;
    QLabel* tickValueLabel;
    QLabel* clickValueLabel;
    QLabel* targetScoreLabel;

    QLayout* UpgradeLayout;

    QNetworkAccessManager *manager;
    QNetworkRequest request;

    RefresherWorker* refreshWorker;
    QThread* workerThread;

    std::vector<Upgrade> Upgrades;

    QPushButton CreateUpgradeButton(QString Label);
    QString BuildUpgradeLabel(Upgrade* upgrade);
};
#endif // MAINWINDOW_H
