#include "windowshakeanimation.h"

WindowShakeAnimation::WindowShakeAnimation(QWidget* target, int targetDuration, float amplitude, float frequence)
{
    this->target = target;
    this->targetDuration = targetDuration;
    this->amplitude = amplitude;
    this->frequence = frequence;
}

int WindowShakeAnimation::duration() const
{
    return this->targetDuration;
}

bool WindowShakeAnimation::event(QEvent*)
{
    return false;
}

void WindowShakeAnimation::updateCurrentTime(int currentTime)
{
    float duration = this->duration();
    float damp = (duration - (float)currentTime)/duration;
    damp = pow(damp, 1.5f);


    int x = sin(currentTime * this->frequence) * this->amplitude * damp;
    int y = cos(currentTime* this->frequence * 2.0f) * this->amplitude * damp;
    QPoint nextPos = this->positionAtStart + QPoint(x, y);
    this->target->move(nextPos);
}

void WindowShakeAnimation::updateDirection(QAbstractAnimation::Direction)
{

}

void WindowShakeAnimation::updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    if(oldState == newState)
    {
        return;
    }

    if(newState == QAbstractAnimation::State::Running)
    {
        this->positionAtStart = this->target->pos();
    }
    else
    {
        this->target->move(this->positionAtStart);
    }
}
