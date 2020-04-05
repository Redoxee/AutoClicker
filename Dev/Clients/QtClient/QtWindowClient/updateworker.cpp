#include "updateworker.h"

UpdateWorker::UpdateWorker()
{
    this->thread = new QThread(this);
    this->moveToThread(this->thread);
    connect(this->thread, &QThread::started, this, &UpdateWorker::Run);
    connect(this->thread, &QThread::finished, this, &UpdateWorker::Finished);
    this->thread->start();
}

UpdateWorker::~UpdateWorker()
{
    this->thread->terminate();
    delete this->thread;
}

void UpdateWorker::Run()
{
    while(true)
    {
        QThread::msleep(UpdateWorker::sleepTime);
        emit this->Update(UpdateWorker::sleepTime);
        if(this->isFinished)
        {
            return;
        }
    }
}
void UpdateWorker::Finished()
{
    this->isFinished = true;
}
