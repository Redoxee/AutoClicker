#include "gamecinematic.h"

GameCinematic::GameCinematic(QWidget *parent, QMainWindow* mainWindow) : QWidget(parent)
{
    this->windowShakeAnimation = new WindowShakeAnimation(mainWindow, 1000, 15, .05);
    this->windowShakeAnimation->start();
}
