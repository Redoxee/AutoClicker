#ifndef SERVERWORKER_H
#define SERVERWORKER_H

#include <QObject>

#include <QApplication>

#include <QNetworkConfigurationManager>
#include <QNetworkConfiguration>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>

#include <QThread>

#include <QProcess>
#include <QDir>
#include <QString>
#include "windows.h"

namespace ServerUtils {
    struct ServerGameplayState;
}

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
        NoServerFound,
        WaitingForGame,
        Pause,
    };

    enum Order
    {
        OrderStartNewServer,
        OrderStartGameplayRefresh,
    };

    ServerWorker(QApplication* application);
    virtual ~ServerWorker();

    State CurrentState() {return this->currentState;}
    void RequestOrder(Order order);
signals:
    void InitialServerResponse();
    void ServerStarted();
    void RefreshGameData(ServerUtils::ServerGameplayState* gameData);

    void PostOrderSignal(ServerWorker::Order order);

private:
    State currentState = State::None;

    QApplication* application = nullptr;

    QThread* thread = nullptr;
    QNetworkAccessManager* manager = nullptr;
    QNetworkRequest request;
    QNetworkReply* reply = nullptr;

    QString applicationPath;

    ServerUtils::ServerGameplayState* data = nullptr;

    int attemptCount;
    const int maxAttemptCount = 100;

    QProcess* serverProcess = nullptr;

    void StartWorker();
    void PauseServer();
    void TerminateServer();
    void StartNewServer();

private slots:
    void ThreadStarted();
    void initialServerRequestResponse(QNetworkReply* httpResponse);
    void startServerRequest(QNetworkReply* httpResponse);
    void gameplayServerResponse(QNetworkReply* reply);
    void orderSlot(ServerWorker::Order order);
};

#endif // SERVERWORKER_H
