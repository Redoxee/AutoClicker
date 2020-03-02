#include "corechecker.h"

CoreChecker::CoreChecker(QThread* targetThread)
{
    this->currentState = CoreCheckerState::NotStarted;
    connect(targetThread, SIGNAL(started()), this, SLOT(Run()));
}

void CoreChecker::Run()
{
    this->currentState = CoreCheckerState::SearchingForCore;
    this->attemptCount = 0;

    this->manager = new QNetworkAccessManager();
    QObject::connect(this->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(InitialHttpReply(QNetworkReply*)));

    QString uri = QString::fromStdString(AutoClicker::BaseURI() + "get_state = full");
    this->request.setUrl(QUrl(uri));
    this->manager->get(this->request);
}

void CoreChecker::InitialHttpReply(QNetworkReply* reply)
{
    if(reply->error() == 0)
    {
        qDebug() << "Core game server found.";
        this->currentState = CoreCheckerState::Sucess;
        emit this->Reply();
        return;
    }
    else if(reply->error() > 99)
    {
        qDebug() << "Unhandled error  : " + reply->errorString();
        this->currentState = CoreCheckerState::Failure;
        emit this->Reply();
        return;
    }

    qDebug() << "Server not found, starting a new one : " + reply->errorString();
    this->currentState = CoreCheckerState::StartingNewCore;
    this->StartCorServerProcess();
    this->currentState = CoreCheckerState::WaitingForNewCore;

    disconnect(this->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(InitialHttpReply(QNetworkReply*)));
    connect(this->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(ProcessWaitingReply(QNetworkReply*)));


    QThread::msleep(AutoClicker::RefreshRate());
    this->attemptCount++;
    this->manager->get(this->request);
}

void CoreChecker::ProcessWaitingReply(QNetworkReply* reply)
{
    if(reply->error() == 0)
    {
        qDebug() << "Core game server found.";
        this->currentState = CoreCheckerState::Sucess;
        emit this->Reply();
        return;
    }
    else if(reply->error() > 99)
    {
        qDebug() << "Unhandled error  : " + reply->errorString();
        this->currentState = CoreCheckerState::Failure;
        emit this->Reply();
        return;
    }

    if(this->attemptCount >= CoreChecker::maxAttemptCount)
    {
        qDebug() << "Start server failed, no respons after " << CoreChecker::maxAttemptCount << " tries.";
        this->currentState = CoreCheckerState::Failure;
        emit this->Reply();
        return;
    }

    QThread::msleep(AutoClicker::RefreshRate());
    this->attemptCount++;
    this->manager->get(this->request);
}

void CoreChecker::StartCorServerProcess()
{
    QString coreGameProcessPath = QDir::currentPath() + QString::fromStdString(AutoClicker::RelativeCoreServerPath());
    qDebug() << coreGameProcessPath;
    QProcess qProcess;

    // Note : windows only solution to display a console for the core game process
    qProcess.setCreateProcessArgumentsModifier([] (QProcess::CreateProcessArguments *args)
    {
        args->flags &= ~CREATE_NO_WINDOW;
        args->startupInfo->dwFlags &= ~STARTF_USESTDHANDLES;
    });

    qProcess.setProgram(coreGameProcessPath);
    qProcess.setArguments({QDir::currentPath() + QString::fromStdString(AutoClicker::RelativeConfigPath())});

    qProcess.startDetached();
}
