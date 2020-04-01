#ifndef SERVERWORKER_H
#define SERVERWORKER_H

#include <QObject>

#include <QApplication>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>

#include <QThread>

#include <QProcess>
#include <QDir>
#include <QString>
#include "windows.h"

#include "autoclickerconfig.h"

class ServerWorker : public QObject
{   
    Q_OBJECT
public:
    enum State
    {
        None,
        Initializing,
        SearchingPreexistingServerInstance,
        StartingNewServerInstance,
        ActiveRefresh,
        Paused,
        NoServerFound,
        WaitingForGame,
        Stopping,
    };

    enum Order
    {
        OrderStartNewServer,
        OrderStartGameplayRefresh,
    };

    ServerWorker(QApplication* application);
    State CurrentState() {return this->currentState;}
    void RequestOrder(Order order);

signals:
    void InitialServerResponse();
    void ServerStarted();
    void RefreshGameData(QJsonObject gameData);

    void PostOrderSignal(ServerWorker::Order order);

private:
    State currentState = State::None;

    QNetworkAccessManager* manager;
    QNetworkRequest request;

    QThread* workerThread;

    QString applicationPath;

    int attemptCount;
    const int maxAttemptCount = 100;

    void StartNewServer();

private slots:
    void onThreadStart();
    void initialServerRequestResponse(QNetworkReply* httpResponse);
    void startServerRequest(QNetworkReply* httpResponse);
    void gameplayServerResponse(QNetworkReply* reply);
    void orderSlot(ServerWorker::Order order);
    void aboutToQuit();
};

#endif // SERVERWORKER_H