#ifndef CORECHECKER_H
#define CORECHECKER_H

#include <QObject>
#include <QRunnable>
#include <QThread>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include <QProcess>
#include <QDir>
#include <QString>
#include "windows.h"

#include "autoclickerconfig.h"

enum CoreCheckerState
{
    NotStarted,
    SearchingForCore,
    StartingNewCore,
    WaitingForNewCore,
    Sucess,
    Failure,
};

class CoreChecker : public QObject
{
    Q_OBJECT
public:
    CoreChecker(QThread* targetThread);

    CoreCheckerState CurrentState(){ return this->currentState; };

public slots:
    void Run();
    void InitialHttpReply(QNetworkReply* reply);
    void ProcessWaitingReply(QNetworkReply* reply);

signals:
    void Reply();

private:
    CoreCheckerState currentState;

    QNetworkAccessManager* manager;
    QNetworkRequest request;

    void StartCorServerProcess();
    int attemptCount;
    const int maxAttemptCount = 100;
};

#endif // CORECHECKER_H
