#ifndef EVENTLOGGER_H
#define EVENTLOGGER_H

#include <QVector>
#include <QWidget>
#include <QTextBrowser>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QUrl>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QRandomGenerator>

class EventLogger : public QTextBrowser
{
    Q_OBJECT
public:
    explicit EventLogger(QWidget* parent);

    void AppendRandomLog();

private:
    QVector<QString> verbList;
    QVector<QString> fileList;
    QRandomGenerator randomGenerator;
};

#endif // EVENTLOGGER_H
