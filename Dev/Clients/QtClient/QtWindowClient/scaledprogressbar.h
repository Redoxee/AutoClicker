#ifndef SMOOTHPROGRESSBAR_H
#define SMOOTHPROGRESSBAR_H

#include <QObject>
#include <QProgressBar>

class ScaledProgressBar : public QProgressBar
{
    Q_OBJECT
public:
    explicit ScaledProgressBar(int scale, QWidget* parent = nullptr);
    void SetScaledValue(int value);

private :
    int scale;
};

#endif // SMOOTHPROGRESSBAR_H
