#include "doorstyleprogressbar.h"

DoorStyleProgressBar::DoorStyleProgressBar(int nbBars, QWidget *parent) : QWidget(parent)
{
    this->length = nbBars;
    this->progressBars = new QProgressBar*[nbBars * 2];

    QBoxLayout* layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);

    for(int i = 0; i < this->length; ++i)
    {
        QHBoxLayout* hLayout = new QHBoxLayout();
        hLayout->setSpacing(0);
        hLayout->setMargin(0);
        QProgressBar* mBar = new QProgressBar();
        hLayout->addWidget(mBar);
        this->progressBars[i * 2] = mBar;
        mBar->setTextVisible(false);
        QSizePolicy sp = mBar->sizePolicy();
        sp.setRetainSizeWhenHidden(true);
        mBar->setSizePolicy(sp);
        mBar->setStyleSheet("border : 0px; margin : -1px;");

        mBar = new QProgressBar();
        hLayout->addWidget(mBar);
        this->progressBars[i * 2 + 1] = mBar;
        mBar->setTextVisible(false);
        mBar->setInvertedAppearance(true);
        mBar->setStyleSheet("border : 0px; margin : -1px;");

        layout->addLayout(hLayout);
    }
}

void DoorStyleProgressBar::SetValue(float value)
{
    int targetFill = static_cast<int>(floor(value * 100 * this->length));
    if(this->prevValue == targetFill)
    {
        return;
    }

    for(int i = 0 ; i < this->length; ++i)
    {
        if(targetFill >= 99)
        {
            if(this->progressBars[i * 2]->value() != 99)
            {
                this->progressBars[i * 2]->setValue(99);
                this->progressBars[i * 2 + 1]->setValue(99);
                this->progressBars[i * 2]->setStyleSheet("border : 0px; margin : -1px; background-color : #06B025;");
                this->progressBars[i * 2 + 1]->setStyleSheet("border : 0px; margin : -1px; background-color : #06B025;");
            }
        }
        else if(targetFill > 0)
        {
            this->progressBars[i * 2]->show();
            this->progressBars[i * 2]->setValue(targetFill);

            this->progressBars[i * 2 + 1]->show();
            this->progressBars[i * 2 + 1]->setValue(targetFill);
        }
        else
        {
            this->progressBars[i * 2]->hide();
            this->progressBars[i * 2 + 1]->hide();
        }

        if(targetFill > 0)
        {
            targetFill = targetFill - 100;
            if(targetFill < 0)
            {
                targetFill = 0;
            }
        }
    }
}
