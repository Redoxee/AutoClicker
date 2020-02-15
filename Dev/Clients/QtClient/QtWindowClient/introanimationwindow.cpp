#include "introanimationwindow.h"
#include "autoclickerconfig.h"

IntroAnimationWindow::IntroAnimationWindow(QApplication* application, QWidget *parent): QMainWindow(parent)
{
    this->setWindowTitle("Shitty Wizzard Installer");
    this->setFixedSize(AutoClicker::WindowWidth, AutoClicker::WindowHeight);
}
