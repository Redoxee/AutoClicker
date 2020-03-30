#include "serverworker.h"

ServerWorker::ServerWorker(QApplication* application, QThread* thread)
{
    this->applicationPath = application->applicationDirPath();

    this->workerThread = thread;
    this->moveToThread(thread);
}

void ServerWorker::initialServerRequestResponse(QNetworkReply* httpResponse)
{
    qDebug() << "a response has arrived" << httpResponse->error() <<"(" << static_cast<int>(httpResponse->error()) << ").";
    disconnect(this->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(initialServerRequestResponse(QNetworkReply*)));

    if(httpResponse->error() == 0)
    {
        qDebug() << "Core game server found.";
        this->currentState = State::WaitingForGame;
        emit this->InitialServerResponse();
        return;
    }

    qDebug() << "No server error  : " + httpResponse->errorString();
    this->currentState = State::NoServerFound;
    emit this->InitialServerResponse();
}

void ServerWorker::startServerRequest(QNetworkReply* httpResponse)
{
    qDebug() << "a response has arrived" << httpResponse->error() <<"(" << static_cast<int>(httpResponse->error()) << ").";

    if(httpResponse->error() == 0)
    {
        qDebug() << "Core game server found.";
        this->currentState = State::WaitingForGame;
        emit this->ServerStarted();
        return;
    }

    if(httpResponse->error() >= 99)
    {
        qDebug() << "Unhandled error  : " + httpResponse->errorString();
        this->currentState = State::NoServerFound;
        emit this->ServerStarted();
    }

    if(this->attemptCount < ServerWorker::maxAttemptCount)
    {
        QThread::msleep(AutoClicker::RefreshRate());
        this->attemptCount++;
        this->manager->get(this->request);
    }
}

void ServerWorker::onThreadStart()
{
    qDebug() << "ServerWorker thread start.";
    this->manager = new QNetworkAccessManager();
    connect(this->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(initialServerRequestResponse(QNetworkReply*)));

    QString uri = QString::fromStdString(AutoClicker::BaseURI() + "get_state = full");
    this->request.setUrl(QUrl(uri));
    this->manager->get(this->request);
}

void ServerWorker::RequestOrder(ServerWorker::Order order)
{
    qDebug() << "New order";
    if(order == Order::OrderStartNewServer)
    {
        if(this->currentState == State::NoServerFound)
        {
            this->StartNewServer();
        }
        else if(this->currentState == State::WaitingForGame)
        {
            emit this->ServerStarted();
        }
    }
}

void ServerWorker::StartNewServer()
{
    this->currentState = State::StartingNewServerInstance;

    QString coreGameProcessPath = this->applicationPath + QString::fromStdString(AutoClicker::RelativeCoreServerPath());
    qDebug() << coreGameProcessPath;
    QProcess qProcess;

    // Note : windows only solution to display a console for the core game process
    qProcess.setCreateProcessArgumentsModifier([] (QProcess::CreateProcessArguments *args)
    {
        args->flags &= ~CREATE_NO_WINDOW;
        args->startupInfo->dwFlags &= ~STARTF_USESTDHANDLES;
    });

    qProcess.setProgram(coreGameProcessPath);
    qProcess.setArguments({this->applicationPath + QString::fromStdString(AutoClicker::RelativeConfigPath())});
    qProcess.startDetached();

    connect(this->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(startServerRequest(QNetworkReply*)));

    this->manager->get(this->request);
}
