#include "serverworker.h"

ServerWorker::ServerWorker(QApplication* application, QThread* thread)
{
    this->applicationPath = application->applicationDirPath();

    this->workerThread = thread;
    this->moveToThread(thread);
}

void ServerWorker::initialServerRequestResponse(QNetworkReply* httpResponse)
{
    qDebug() << "a response has arrived " << httpResponse->error() <<".";

    if(httpResponse->error() == 0)
    {
        qDebug() << "Core game server found.";
        this->currentState = State::WaitingForGame;
        emit this->InitialServerResponse();
        return;
    }
    else if(httpResponse->error() > 99)
    {
        qDebug() << "Unhandled error  : " + httpResponse->errorString();
        this->currentState = State::NoServerFound;
        emit this->InitialServerResponse();
        return;
    }

    if(this->attemptCount >= ServerWorker::maxAttemptCount)
    {
        qDebug() << "Start server failed, no respons after " << ServerWorker::maxAttemptCount << " tries.";
        this->currentState = State::NoServerFound;
        emit this->InitialServerResponse();
        return;
    }

    QThread::msleep(AutoClicker::RefreshRate());
    this->attemptCount++;
    this->manager->get(this->request);
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
        this->StartNewServer();
    }
}

void ServerWorker::StartNewServer()
{

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

    this->manager->get(this->request);
}
