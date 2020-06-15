#ifndef WINDOWSLIDANIMATION_H
#define WINDOWSLIDANIMATION_H

#include <QObject>
#include <QWidget>
#include <QAbstractAnimation>
#include <QEasingCurve>
#include <QPoint>

class WindowSlideAnimation : public QAbstractAnimation
{
    Q_OBJECT
public:
    WindowSlideAnimation(QObject* parent, QWidget* target, int duration, QEasingCurve curve, QPoint translation);

    int duration() const override;
    bool event(QEvent *event) override;
    void updateCurrentTime(int currentTime) override;
    void updateDirection(QAbstractAnimation::Direction direction) override;
    void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState) override;


private:
    QWidget* target;
    int animDuration;
    QEasingCurve curve;
    QPoint StartingPoint;
    QPoint EndingPoint;
    QPoint Gap;
};

#endif // WINDOWSLIDANIMATION_H
