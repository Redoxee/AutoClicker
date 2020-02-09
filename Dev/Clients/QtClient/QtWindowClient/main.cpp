#include "mainwindow.h"
#include "introductionscreen.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*
    MainWindow mainWindow(&a);
    mainWindow.show();
    */

    IntroductionScreen introScreen(&a);
    introScreen.show();

    return a.exec();
}
