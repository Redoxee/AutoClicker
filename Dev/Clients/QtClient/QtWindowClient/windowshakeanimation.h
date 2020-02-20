#ifndef WINDOWSHAKEANIMATION_H
#define WINDOWSHAKEANIMATION_H

#include <QMainWindow>
#include <QObject>
#include <QAbstractAnimation>
#include <QtMath>

class WindowShakeAnimation : public QAbstractAnimation
{
    Q_OBJECT
public:
    WindowShakeAnimation(QWidget* target, int duration, float amplitude, float frequence);

    int duration() const override;
    bool event(QEvent *event) override;
    void updateCurrentTime(int currentTime) override;
    void updateDirection(QAbstractAnimation::Direction direction) override;
    void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState) override;

private:
    QWidget* target;
    int targetDuration;
    float amplitude;
    float frequence;

    QPoint positionAtStart;
};

#endif // WINDOWSHAKEANIMATION_H
