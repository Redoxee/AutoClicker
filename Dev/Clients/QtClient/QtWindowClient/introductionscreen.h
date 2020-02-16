#ifndef INTRODUCTIONSCREEN_H
#define INTRODUCTIONSCREEN_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>

#include <QtDebug>

#include "introscreenwidget.h"

class IntroductionScreen : public QMainWindow
{
public slots:
    void StartPressed();

public:
    IntroductionScreen(QApplication* application, QWidget *parent = nullptr);
private:
    void SetupUi();
    QApplication* qApplication;

    IntroScreenWidget* introScreenWidget;
    QLayout* mainLayout;
    QLayout* mainHorizontalLayout;
};

#endif // INTRODUCTIONSCREEN_H
