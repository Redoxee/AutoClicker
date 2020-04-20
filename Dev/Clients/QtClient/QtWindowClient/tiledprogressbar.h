#ifndef TILEDPROGRESSBAR_H
#define TILEDPROGRESSBAR_H

#include <QObject>
#include <QWidget>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QDebug>

class TiledProgressBar : public QWidget
{
    Q_OBJECT
public:
    explicit TiledProgressBar(QWidget *parent = nullptr);

    void SetValue(float value);

    int row = 6;
    int column = 3;
    int barPerTile = 5;

private:
    QProgressBar** progressBars = nullptr;

signals:
};

#endif // TILEDPROGRESSBAR_H
