#ifndef SPIRALPROGRESSBAR_H
#define SPIRALPROGRESSBAR_H

#include <QObject>
#include <QWidget>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "IProgressBar.h"

class SpiralProgressBar : public QWidget, public IProgressBar
{
    Q_OBJECT
public:
    explicit SpiralProgressBar(QWidget *parent = nullptr);

    void SetProgress(float value) override;

    QLayout* CentralLayout = nullptr;

private:
    int numberOfLoops = 7;
    QProgressBar** progressBars = nullptr;
};

#endif // SPIRALPROGRESSBAR_H
