#ifndef INTRODUCTIONSCREEN_H
#define INTRODUCTIONSCREEN_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>

class IntroductionScreen : public QMainWindow
{
public slots:
    void StartPressed();

public:
    IntroductionScreen(QApplication* application, QWidget *parent = nullptr);
private:
    void SetupUi();
    QApplication* qApplication;
};

#endif // INTRODUCTIONSCREEN_H
