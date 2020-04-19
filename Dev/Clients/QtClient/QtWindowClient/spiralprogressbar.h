#ifndef SPIRALPROGRESSBAR_H
#define SPIRALPROGRESSBAR_H

#include <QObject>
#include <QWidget>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QDebug>

class SpiralProgressBar : public QWidget
{
    Q_OBJECT
public:
    explicit SpiralProgressBar(QWidget *parent = nullptr);

    void SetValue(float value);

    QLayout* CentralLayout = nullptr;
signals:

private:
    int numberOfLoops = 7;
    QProgressBar** progressBars = nullptr;
};

#endif // SPIRALPROGRESSBAR_H
