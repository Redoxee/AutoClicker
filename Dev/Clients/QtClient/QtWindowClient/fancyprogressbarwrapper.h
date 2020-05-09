#ifndef FANCYPROGRESSBARWRAPPER_H
#define FANCYPROGRESSBARWRAPPER_H

#include <QObject>
#include <QAbstractAnimation>
#include <QEasingCurve>

class IProgressBar;

class FancyProgressBarWrapper : public QAbstractAnimation
{
    Q_OBJECT
public:
    explicit FancyProgressBarWrapper(int duration, IProgressBar* targetBar, Direction direction = Direction::Forward);

    int duration() const override;
    bool event(QEvent *event) override;
    void updateCurrentTime(int currentTime) override;
    void updateDirection(QAbstractAnimation::Direction direction) override;
    void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState) override;

    void setAnimationDuration(int duration);
    void setEasingCurve(QEasingCurve curve);
private:
    int animationDuration = 0;
    IProgressBar* targetBar = nullptr;

    QEasingCurve easingCurve = QEasingCurve::Linear;

    Direction direction;
};

#endif // FANCYPROGRESSBARWRAPPER_H
