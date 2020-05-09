#include "fancyprogressbarwrapper.h"
#include "IProgressBar.h"


FancyProgressBarWrapper::FancyProgressBarWrapper(int duration, IProgressBar* targetBar, Direction direction)
{
    this->animationDuration = duration;
    this->targetBar = targetBar;
    this->direction = direction;
}

void FancyProgressBarWrapper::setEasingCurve(QEasingCurve curve)
{
    this->easingCurve = curve;
}

int FancyProgressBarWrapper::duration() const
{
    return this->animationDuration;
}

bool FancyProgressBarWrapper::event(QEvent*)
{
    return false;
}

void FancyProgressBarWrapper::updateCurrentTime(int currentTime)
{
    float duration = this->duration();
    float fTime = static_cast<float>(currentTime);
    if(duration == 0)
    {
        return;
    }

    if(fTime > duration)
    {
        fTime = duration;
    }

    qreal progress = this->easingCurve.valueForProgress(fTime / duration);

    if(this->direction == Direction::Backward)
    {
        progress = 1 - progress;
    }

    this->targetBar->SetProgress(progress);
}

void FancyProgressBarWrapper::updateDirection(QAbstractAnimation::Direction direction)
{
    this->direction = direction;
}

void FancyProgressBarWrapper::updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    if(oldState == newState)
    {
        return;
    }
}
