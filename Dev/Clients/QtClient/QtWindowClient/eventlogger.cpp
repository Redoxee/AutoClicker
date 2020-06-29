#include "eventlogger.h"
#include <QRandomGenerator>
#include <QScrollBar>

#include "gamesettings.h"

EventLogger::EventLogger(QWidget* parent) : QFrame(parent)
{
    QJsonObject root = GameSettings::Instance()->LoadingQuips;
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

    this->setFrameStyle(QFrame::Box | QFrame::Sunken);
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::white);
    this->setPalette(pal);
    this->setAutoFillBackground(true);

    this->randomGenerator = new QRandomGenerator();
    this->stringList = new QStringList();

    QVBoxLayout* mLayout = new QVBoxLayout(this);
    mLayout->setMargin(0);
    this->scrollArea = new QScrollArea(this);
    mLayout->addWidget(this->scrollArea);

    QWidget* scrollableWidget = new QWidget();
    QVBoxLayout* vLayout = new QVBoxLayout(scrollableWidget);
    scrollableWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    vLayout->setMargin(0);

    vLayout->setSpacing(0);
    vLayout->setMargin(0);

    this->labels = new QLabel*[EventLogger::maxSize];
    for(int index = 0; index < EventLogger::maxSize; ++index)
    {
        this->labels[index] = new QLabel(this);
        this->labels[index]->setMinimumHeight(10);
        this->labels[index]->setMinimumWidth(271);
        this->labels[index]->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
        vLayout->addWidget(this->labels[index]);
    }

    this->scrollArea->setWidget(scrollableWidget);
}

void EventLogger::AppendRandomLog()
{
    if(this->skipRandomLogCounter > 0)
    {
        this->skipRandomLogCounter--;
        return;
    }

    int verbIndex = this->randomGenerator->bounded(this->verbList.count());
    int fileIndex = this->randomGenerator->bounded(this->fileList.count());
    this->AppendString(this->verbList[verbIndex].arg(this->fileList[fileIndex]));
}

void EventLogger::AppendString(QString log)
{
    this->stringList->append(log);
    if(this->stringList->count() >= this->maxSize)
    {
        this->stringList->removeAt(0);
    }

    int numberOfLogs = this->stringList->count();
    for(int index = 0; index < this->maxSize; ++index)
    {
        if(index < numberOfLogs)
        {
            this->labels[this->maxSize - 1 - index]->setText(this->stringList->at(numberOfLogs - 1 - index));
        }
    }

    this->scrollArea->verticalScrollBar()->setValue(this->scrollArea->verticalScrollBar()->maximum());
}

void EventLogger::ClearLogs()
{
    this->stringList->clear();
    for(int index = 0; index < this->maxSize; ++index)
    {
        this->labels[index]->setText("");
    }

    this->scrollArea->verticalScrollBar()->setValue(this->scrollArea->verticalScrollBar()->maximum());
}

void EventLogger::SkipNextRandomLogs(int numberOfSkip)
{
    this->skipRandomLogCounter = numberOfSkip;
}
