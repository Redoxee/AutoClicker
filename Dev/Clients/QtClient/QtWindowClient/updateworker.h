#ifndef UPDATEWORKER_H
#define UPDATEWORKER_H

#include <QObject>
#include <QThread>
#include <QApplication>

class UpdateWorker : public QObject
{
    Q_OBJECT
public:
    UpdateWorker();
    virtual ~UpdateWorker();
signals:
    void Update(float dt);
private slots:
    void Run();
    void Finished();

private:
    static constexpr float sleepTime = (1000.0f / 60.0f);
    QThread* thread = nullptr;
    bool isFinished = false;
};

#endif // UPDATEWORKER_H
