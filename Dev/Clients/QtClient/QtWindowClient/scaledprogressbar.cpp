#include "scaledprogressbar.h"

ScaledProgressBar::ScaledProgressBar(int scale, QWidget* parent) : QProgressBar(parent)
{
    this->scale = scale;
}

void ScaledProgressBar::SetScaledValue(int value)
{
    int score = (value % this->scale);
    score = (static_cast<double>(score) / static_cast<double>(this->scale)) * 100;
    this->setValue(score);
}
