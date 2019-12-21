#ifndef REFRESHERWORKER_H
#define REFRESHERWORKER_H

#include <QObject>
#include <QRunnable>
#include <QThread>
#include <QJsonDocument>
#include <QJsonObject>


class RefresherWorker : public QObject
{
    Q_OBJECT

    int number = 0;

    bool running;

public:
    RefresherWorker(){
            this->running = false;
        }

    bool isRunning() const{
        return this->running;
    }
    void stop(){
        this->running = false;
    }

signals:
    void refreshData(int data);

public slots:

    void run(){
        this->running = true;
        while(this->running){
            this->number++;
            this->refreshData(this->number);

            QThread::msleep(10);
        }
    }
};

#endif // REFRESHERWORKER_H
