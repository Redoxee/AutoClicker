#ifndef GRIDPROGRESSBAR_H
#define GRIDPROGRESSBAR_H

#include <QWidget>
#include <QGridLayout>
#include <QProgressBar>

class GridProgressBar : public QWidget
{
    Q_OBJECT
public:
    explicit GridProgressBar(QWidget *parent = nullptr, int row = 5, int column = 5);

    int row = 5;
    int column = 5;

signals:

private :
    QGridLayout* gridLayout = nullptr;
};

#endif // GRIDPROGRESSBAR_H
