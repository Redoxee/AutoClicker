#ifndef DOORSTYLEPROGRESSBAR_H
#define DOORSTYLEPROGRESSBAR_H

#include <QWidget>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "IProgressBar.h"

class DoorStyleProgressBar : public QWidget, public IProgressBar
{
    Q_OBJECT
public:
    explicit DoorStyleProgressBar(int nbBar, QWidget *parent = nullptr);

    int length;

    void SetProgress(float value) override;

    void SetBackward(bool back) { this->backward = back; };

private:
    QProgressBar** progressBars = nullptr;
    int prevValue = -1;
    bool backward = false;

signals:

};

#endif // DOORSTYLEPROGRESSBAR_H
