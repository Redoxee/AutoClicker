#ifndef INTROANIMATIONWINDOW_H
#define INTROANIMATIONWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>

class IntroAnimationWindow : public QMainWindow
{
public:
    IntroAnimationWindow(QApplication* application, QWidget *parent = nullptr);
};

#endif // INTROANIMATIONWINDOW_H
