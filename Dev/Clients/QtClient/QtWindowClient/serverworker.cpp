#include "serverworker.h"

#include <QStringList>

#include "autoclickerconfig.h"
#include "servergameplaystate.h"
#include "gamesettings.h"

using namespace ServerUtils;

ServerWorker::ServerWorker(QApplication* application)
{
    this->currentState = State::Initializing;
    this->application = application;
    this->applicationPath = application->applicationDirPath();

    this->data = new ServerGameplayState();
    this->isOwner = false;

    this->thread = new QThread();
    connect(this->thread, &QThread::started, this, &ServerWorker::ThreadStarted);
    connect(this->thread, &QThread::finished, this, &ServerWorker::ThreadFinished);
    this->thread->start();
    this->moveToThread(this->thread);
}

ServerWorker::~ServerWorker()
{
    this->thread->quit();
    this->thread->wait();

    delete this->data;

    delete this->serverProcess;
}

void ServerWorker::ThreadStarted()
{
    this->StartWorker();
}

void ServerWorker::ThreadFinished()
{
    if(this->isOwner)
    {
        if(!GameSettings::Instance()->DontKillProcessOnClose)
        {
            this->TerminateServer();
        }
    }
}

void ServerWorker::StartWorker()
{
    qDebug() << "ServerWorker start.";
    this->manager = new QNetworkAccessManager();
    connect(this, SIGNAL(PostOrderSignal(ServerWorker::Order)), this, SLOT(orderSlot(ServerWorker::Order)));

    connect(this->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(initialServerRequestResponse(QNetworkReply*)));
    QNetworkConfigurationManager configurationManager;
    QNetworkConfiguration configuration = configurationManager.defaultConfiguration();
    configuration.setConnectTimeout(100);
    this->manager->setConfiguration(configuration);
    QString uri = QString::fromStdString(AutoClicker::BaseURI() + "get_state = full");
    this->request.setUrl(QUrl(uri));
    this->currentState = State::SearchingPreexistingServerInstance;
    this->manager->get(this->request);
}

void ServerWorker::PauseServer()
{
    this->currentState = State::Pause;
    this->request.setUrl(QUrl(QString::fromStdString( AutoClicker::BaseURI() + "set_update_pause=true")));
    this->reply = manager->get(this->request);
}

void ServerWorker::TerminateServer()
{
    this->currentState = State::Pause;
    if(this->reply != nullptr)
    {
        this->reply->abort();
    }

    this->request.setUrl(QUrl(QString::fromStdString( AutoClicker::BaseURI() + "set_terminated=true")));
    this->reply = manager->get(this->request);
    QThread::msleep(200);
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
    this->serverProcess = new QProcess();

    // Note : windows only solution to display a console for the core game process
    this->serverProcess->setCreateProcessArgumentsModifier([] (QProcess::CreateProcessArguments *args)
    {
        args->flags &= ~CREATE_NO_WINDOW;
        args->startupInfo->dwFlags &= ~STARTF_USESTDHANDLES;
    });

    QStringList arguments;
    arguments << "--Config" << this->applicationPath + QString::fromStdString(AutoClicker::RelativeServerConfigPath());
    arguments << "--LogFile" << "GameLog.txt";
    this->serverProcess->setProgram(coreGameProcessPath);
    this->serverProcess->setArguments(arguments);
    this->serverProcess->startDetached();

    connect(this->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(startServerRequest(QNetworkReply*)));

    this->reply = this->manager->get(this->request);
    this->isOwner = true;
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
        this->data->SetDataData(jsonObject);
        emit RefreshGameData(this->data);
    }

    if(this->currentState == State::ActiveRefresh)
    {
        QThread::msleep(AutoClicker::RefreshRate());
        this->reply = this->manager->get(this->request);
    }
}
