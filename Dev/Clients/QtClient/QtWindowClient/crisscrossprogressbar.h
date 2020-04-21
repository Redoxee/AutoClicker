#ifndef CRISSCROSSPROGRESSBAR_H
#define CRISSCROSSPROGRESSBAR_H

#include <QObject>
#include <QWidget>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QDebug>
#include <QStackedLayout>
#include <QGridLayout>

class CrissCrossProgressBar : public QWidget
{
    Q_OBJECT
public:
    explicit CrissCrossProgressBar(int row, int column, int spacing, QWidget *parent = nullptr);

    void SetValue(float value);

private:
    QProgressBar** progressBars = nullptr;

    QBoxLayout* firstLayout = nullptr;
    QBoxLayout* secondLayout = nullptr;

    int row = 30;
    int column = 15;
    int spacing = 8;

signals:

};

#endif // CRISSCROSSPROGRESSBAR_H
