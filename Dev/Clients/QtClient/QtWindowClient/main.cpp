#include "gamewindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    GameWindow mainWindow(&a);
    mainWindow.show();

    return a.exec();
}
