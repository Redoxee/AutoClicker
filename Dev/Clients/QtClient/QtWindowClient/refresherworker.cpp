#include "refresherworker.h"

void RefresherWorker::run()
{
    this->running = true;

    this->manager = new QNetworkAccessManager();
    QObject::connect(this->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleHttpRequest(QNetworkReply*)));


    QString uri = QString::fromStdString(AutoClicker::BaseURI() + "get_state = full&set_ticklength = " + std::to_string(AutoClicker::TickLength()) + "& set_update_pause = false");
    this->request.setUrl(QUrl(uri));
    this->waitingForQuery = true;
    this->manager->get(this->request);
}

void RefresherWorker::TryRequestRefresh()
{
    if(!this->running)
    {
        return;
    }

    if(this->waitingForQuery)
    {
        return;
    }

    this->request.setUrl(QUrl("http://localhost:1234/AutoClicker?get_state=full"));
    this->waitingForQuery = true;
    this->manager->get(this->request);
}

void RefresherWorker::ProcessReply(QNetworkReply *reply)
{
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }

    QString answer = reply->readAll();

    //qDebug() << answer;

    QJsonDocument jsonDocumnet = QJsonDocument::fromJson(answer.toUtf8());
    QJsonObject jsonObject = jsonDocumnet.object();

    this->refreshData(jsonObject);
}

void RefresherWorker::handleHttpRequest(QNetworkReply* reply)
{
    this->waitingForQuery = false;
    this->ProcessReply(reply);
    QThread::msleep(AutoClicker::RefreshRate());
    this->TryRequestRefresh();
}
