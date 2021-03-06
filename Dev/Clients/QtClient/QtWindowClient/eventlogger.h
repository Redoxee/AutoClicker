#ifndef EVENTLOGGER_H
#define EVENTLOGGER_H

#include <QVector>
#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QRandomGenerator>
#include <QStringList>
#include <QVBoxLayout>
#include <QPalette>
#include <QScrollArea>

class EventLogger : public QFrame
{
    Q_OBJECT
public:
    explicit EventLogger(QWidget* parent);

    void AppendRandomLog();
    void AppendString(QString log);
    void ClearLogs();
    void SkipNextRandomLogs(int numberOfSkip);

private:
    QVector<QString> verbList;
    QVector<QString> fileList;
    QRandomGenerator* randomGenerator;

    QStringList* stringList = nullptr;
    QScrollArea* scrollArea = nullptr;
    QLabel** labels = nullptr;

    int skipRandomLogCounter = 0;
    const int maxSize = 30;
};

#endif // EVENTLOGGER_H
