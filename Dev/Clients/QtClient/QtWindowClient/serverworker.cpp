#include "serverworker.h"

ServerWorker::ServerWorker(QApplication* application)
{
    this->currentState = State::Initializing;
    this->application = application;
    this->applicationPath = application->applicationDirPath();

    this->workerThread = new QThread();
    connect(this->workerThread, SIGNAL(started()), this, SLOT(onThreadStart()));
    this->workerThread->start();
    this->moveToThread(this->workerThread);
}

void ServerWorker::onThreadStart()
{
    qDebug() << "ServerWorker thread start.";

    connect(this, SIGNAL(PostOrderSignal(ServerWorker::Order)), this, SLOT(orderSlot(ServerWorker::Order)));

    this->manager = new QNetworkAccessManager();
    connect(this->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(initialServerRequestResponse(QNetworkReply*)));
    QNetworkConfigurationManager configurationManager;
    QNetworkConfiguration configuration = configurationManager.defaultConfiguration();
    configuration.setConnectTimeout(100);
    this->manager->setConfiguration(configuration);
    QString uri = QString::fromStdString(AutoClicker::BaseURI() + "get_state = full");
    this->request.setUrl(QUrl(uri));
    this->currentState = State::SearchingPreexistingServerInstance;
    this->manager->get(this->request);

    connect(this->application, SIGNAL(lastWindowClosed()), this, SLOT(aboutToQuit()));
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

void ServerWorker::RequestOrder(Order order)
{
    emit PostOrderSignal(order);
}

void ServerWorker::orderSlot(ServerWorker::Order order)
{
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

    if(order == Order::OrderStartGameplayRefresh)
    {
        if(this->currentState != State::ActiveRefresh)
        {
            this->currentState = State::ActiveRefresh;
            this->request.setUrl(QUrl(QString::fromStdString(AutoClicker::BaseURI() + "get_state=full&set_frame_length = " + std::to_string(AutoClicker::FrameLength()) + "& set_update_pause = false")));
            connect(this->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(gameplayServerResponse(QNetworkReply*)));
            this->manager->get(this->request);
            this->request.setUrl(QUrl(QString::fromStdString(AutoClicker::BaseURI() + "get_state=full")));
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

    this->reply = this->manager->get(this->request);
}

void ServerWorker::startServerRequest(QNetworkReply* httpResponse)
{
    qDebug() << "a response has arrived" << httpResponse->error() <<"(" << static_cast<int>(httpResponse->error()) << ").";

    if(httpResponse->error() == 0)
    {
        qDebug() << "Core game server found.";
        this->currentState = State::WaitingForGame;
        disconnect(this->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(startServerRequest(QNetworkReply*)));
        emit this->ServerStarted();
        return;
    }

    if(httpResponse->error() >= 99)
    {
        qDebug() << "Unhandled error  : " + httpResponse->errorString();
        this->currentState = State::NoServerFound;
    }

    if(this->attemptCount < ServerWorker::maxAttemptCount)
    {
        QThread::msleep(AutoClicker::RefreshRate());
        this->attemptCount++;
        this->reply = this->manager->get(this->request);
    }
}


void ServerWorker::gameplayServerResponse(QNetworkReply* reply)
{
    if (reply->error()) {
        qDebug() << reply->errorString();
    }
    else
    {
        QString answer = reply->readAll();

        QJsonDocument jsonDocumnet = QJsonDocument::fromJson(answer.toUtf8());
        QJsonObject jsonObject = jsonDocumnet.object();

        emit RefreshGameData(jsonObject);
    }

    if(this->currentState == State::ActiveRefresh)
    {
        QThread::msleep(AutoClicker::RefreshRate());
        this->reply = this->manager->get(this->request);
    }
}


void ServerWorker::aboutToQuit()
{
    qDebug() << "About to quit";
    if(this->currentState == State::ActiveRefresh)
    {
        disconnect(this->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(gameplayServerResponse(QNetworkReply*)));
    }

    this->currentState = State::Stopping;
    this->request.setUrl(QUrl(QString::fromStdString( AutoClicker::BaseURI() + "set_update_pause=true")));
    this->reply = manager->get(this->request);
    qDebug() << "About to quit called.";
    // TODO : find a way to enable one last request before quitting without waiting an arbitrary length of time.
    QThread::sleep(1);
}
