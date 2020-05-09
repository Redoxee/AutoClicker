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

private:
    QProgressBar** progressBars = nullptr;
    int prevValue = -1;

signals:

};

#endif // DOORSTYLEPROGRESSBAR_H
