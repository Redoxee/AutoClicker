#include "eventlogger.h"
#include <QRandomGenerator>

#include <QDebug>

EventLogger::EventLogger(QWidget* parent) : QTextBrowser(parent)
{
    QString val;
    QFile file;
    file.setFileName("Ressources/LoadingQuips.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();
    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject root = d.object();
    QJsonArray formats = root["Formats"].toArray();
    for(int index = 0; index < formats.count(); ++index)
    {
        QString value = formats[index].toString();
        this->verbList.append(value);
    }

    QJsonArray fileNames = root["Targets"].toArray();
    for(int index = 0; index < fileNames.count(); ++index)
    {
        QString value = fileNames[index].toString();
        this->fileList.append(value);
    }
}

void EventLogger::AppendRandomLog()
{
    int verbIndex = this->randomGenerator.bounded(this->verbList.count());
    int fileIndex = this->randomGenerator.bounded(this->fileList.count());
    this->append(this->verbList[verbIndex].arg(this->fileList[fileIndex]));
}
