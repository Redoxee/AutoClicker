#include "eventlogger.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QUrl>

EventLogger::EventLogger(QWidget* parent) : QTextBrowser(parent)
{
}
