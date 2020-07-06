#ifndef ENDSCOREWIDGET_H
#define ENDSCOREWIDGET_H

#include <QWidget>
#include <QProgressBar>
#include <QBoxLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

class EndScoreWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EndScoreWidget(QWidget *parent = nullptr);

    QBoxLayout* centralLayout = nullptr;

    void Update(float time);

private:
    const int nbProgressBarRows = 35;
    QProgressBar** progressbars = nullptr;
};

#endif // ENDSCOREWIDGET_H
