#ifndef SMOOTHPROGRESSBAR_H
#define SMOOTHPROGRESSBAR_H

#include <QObject>
#include <QProgressBar>

#include "IProgressBar.h"

class ScaledProgressBar : public QProgressBar, public IProgressBar
{
    Q_OBJECT
public:
    explicit ScaledProgressBar(int scale, QWidget* parent = nullptr);
    void SetScaledValue(int value);
    void SetProgress(float value) override;

private :
    int scale;
};

#endif // SMOOTHPROGRESSBAR_H
