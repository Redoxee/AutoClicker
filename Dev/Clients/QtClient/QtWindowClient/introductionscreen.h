#ifndef INTRODUCTIONSCREEN_H
#define INTRODUCTIONSCREEN_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>

#include <QtDebug>

#include "introscreenwidget.h"
#include "maingamewidget.h"
class IntroductionScreen : public QMainWindow
{
public slots:
    void StartPressed();

public:
    IntroductionScreen(QApplication* application, QWidget *parent = nullptr);
private:
    void SetupUi();
    QApplication* qApplication;

    QLayout* mainLayout;
    QLayout* mainHorizontalLayout;
    QWidget* mainWidget;


    IntroScreenWidget* introScreenWidget;
    MainGameWidget* mainGameWidget;
};

#endif // INTRODUCTIONSCREEN_H
