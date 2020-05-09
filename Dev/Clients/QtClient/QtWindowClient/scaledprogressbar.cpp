#include "scaledprogressbar.h"

ScaledProgressBar::ScaledProgressBar(int scale, QWidget* parent) : QProgressBar(parent)
{
    this->scale = scale;
}

void ScaledProgressBar::SetProgress(float value)
{
    this->SetScaledValue(static_cast<int>(value * 100));
}

void ScaledProgressBar::SetScaledValue(int value)
{
    int prevValue = this->value();
    if(prevValue == value)
    {
        return;
    }

    int score = (value % this->scale);
    score = (static_cast<double>(score) / static_cast<double>(this->scale)) * 100;
    this->setValue(score);

    if(prevValue == 0)
    {
        this->repaint();
    }
}
