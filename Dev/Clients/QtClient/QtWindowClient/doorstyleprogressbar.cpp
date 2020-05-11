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
        mBar->setStyleSheet("border : 0px; margin : -1px; background-color : transparent;");

        mBar = new QProgressBar();
        hLayout->addWidget(mBar);
        this->progressBars[i * 2 + 1] = mBar;
        mBar->setTextVisible(false);
        mBar->setInvertedAppearance(true);
        mBar->setStyleSheet("border : 0px; margin : -1px; background-color : transparent;");

        layout->insertLayout(0, hLayout);
    }
}

void DoorStyleProgressBar::SetProgress(float value)
{
    int targetFill = static_cast<int>(floor(value * 100 * this->length));
    if(this->prevValue == targetFill)
    {
        return;
    }

    for(int i = 0 ; i < this->length; ++i)
    {
        int index = i;
        if(this->backward)
        {
            index = this->length - i - 1;
        }

        if(targetFill >= 99)
        {
            if(this->progressBars[index * 2]->value() != 99)
            {
                this->progressBars[index * 2]->setValue(99);
                this->progressBars[index * 2 + 1]->setValue(99);
                this->progressBars[index * 2]->setStyleSheet("border : 0px; margin : -1px; background-color : #06B025;");
                this->progressBars[index * 2 + 1]->setStyleSheet("border : 0px; margin : -1px; background-color : #06B025;");
            }
        }
        else if(targetFill > 0)
        {
            int prevFill = this->progressBars[index * 2]->value();
            this->progressBars[index * 2]->show();
            this->progressBars[index * 2]->setValue(targetFill);

            this->progressBars[index * 2 + 1]->show();
            this->progressBars[index * 2 + 1]->setValue(targetFill);

            if(prevFill >= 99)
            {
                this->progressBars[index * 2]->setStyleSheet("border : 0px; margin : -1px; background-color : transparent;");
                this->progressBars[index * 2 + 1]->setStyleSheet("border : 0px; margin : -1px; background-color : transparent;");
            }
        }
        else
        {
            this->progressBars[index * 2]->hide();
            this->progressBars[index * 2 + 1]->hide();
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

    this->prevValue = targetFill;
}
