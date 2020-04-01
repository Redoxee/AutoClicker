#include "gamewindow.h"
#include "serverworker.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    qRegisterMetaType<ServerWorker::Order>("ServerWorker::Order");

    QApplication a(argc, argv);

    GameWindow mainWindow(&a);
    mainWindow.show();

    return a.exec();
}
