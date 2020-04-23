#ifndef DOORSTYLEPROGRESSBAR_H
#define DOORSTYLEPROGRESSBAR_H

#include <QWidget>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QHBoxLayout>

class DoorStyleProgressBar : public QWidget
{
    Q_OBJECT
public:
    explicit DoorStyleProgressBar(int nbBar, QWidget *parent = nullptr);

    int length;

    void SetValue(float value);

private:
    QProgressBar** progressBars = nullptr;
    int prevValue = -1;

signals:

};

#endif // DOORSTYLEPROGRESSBAR_H
