#ifndef EVENTLOGGER_H
#define EVENTLOGGER_H

#include <QWidget>
#include <QTextBrowser>

class EventLogger : public QTextBrowser
{
    Q_OBJECT
public:
    explicit EventLogger(QWidget* parent);

private:

};

#endif // EVENTLOGGER_H
