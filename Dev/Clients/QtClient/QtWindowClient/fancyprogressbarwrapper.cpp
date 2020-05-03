#include "fancyprogressbarwrapper.h"
#include "IProgressBar.h"


FancyProgressBarWrapper::FancyProgressBarWrapper(int duration, IProgressBar* targetBar)
{
    this->animationDuration = duration;
    this->targetBar = targetBar;
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

    this->targetBar->SetValue(progress);
}

void FancyProgressBarWrapper::updateDirection(QAbstractAnimation::Direction)
{

}

void FancyProgressBarWrapper::updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    if(oldState == newState)
    {
        return;
    }
}
