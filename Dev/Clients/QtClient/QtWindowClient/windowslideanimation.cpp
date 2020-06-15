#include "windowslideanimation.h"

WindowSlideAnimation::WindowSlideAnimation(QObject* parent, QWidget* target, int duration, QEasingCurve curve, QPoint translation) : QAbstractAnimation(parent)
{
    this->target = target;
    this->animDuration = duration;
    this->curve = curve;
    this->Gap = translation;
}

int WindowSlideAnimation::duration() const
{
    return this->animDuration;
}

bool WindowSlideAnimation::event(QEvent*)
{
    return false;
}

void WindowSlideAnimation::updateCurrentTime(int currentTime)
{
    float duration = this->duration();
    float t = static_cast<float>(currentTime)/static_cast<float>(duration);

    t = this->curve.valueForProgress(t);

    QPoint nextPos = t * this->Gap + this->StartingPoint;

    this->target->move(nextPos);
}

void WindowSlideAnimation::updateDirection(QAbstractAnimation::Direction)
{

}

void WindowSlideAnimation::updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    if(oldState == newState)
    {
        return;
    }

    if(newState == QAbstractAnimation::State::Running)
    {
        this->StartingPoint = this->target->pos();
        this->EndingPoint = this->StartingPoint + this->Gap;
    }
    else
    {
        this->target->move(this->EndingPoint);
    }
}
