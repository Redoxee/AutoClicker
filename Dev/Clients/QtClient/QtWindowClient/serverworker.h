#ifndef SERVERWORKER_H
#define SERVERWORKER_H

#include <QObject>

#include <QApplication>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

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
    };

    ServerWorker(QApplication* application, QThread* thread);

    State CurrentState() {return this->currentState;}

public slots:
    void RequestOrder(Order order);

signals:
    void InitialServerResponse();


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
};

#endif // SERVERWORKER_H
