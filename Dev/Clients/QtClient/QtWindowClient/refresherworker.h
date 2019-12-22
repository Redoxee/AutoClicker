#ifndef REFRESHERWORKER_H
#define REFRESHERWORKER_H

#include <QObject>
#include <QRunnable>
#include <QThread>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>

#include "AutoClickerConfig.h"

class RefresherWorker : public QObject
{
    Q_OBJECT

public:
    RefresherWorker(){
        this->waitingForQuery = false;
        this->running = false;
    }

    bool isRunning() const{
        return this->running;
    }

    void stop(){
        this->running = false;
    }

signals:
    void refreshData(QJsonObject jsonData);

public slots:

    void run();
    void handleHttpRequest(QNetworkReply* reply);

private:
    bool running;
    bool waitingForQuery;

    QNetworkAccessManager* manager;
    QNetworkRequest request;

    void ProcessReply(QNetworkReply* reply);
    void TryRequestRefresh();

    ulong tickLength = 100;
};

#endif // REFRESHERWORKER_H
