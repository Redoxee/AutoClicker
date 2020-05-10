#include "crisscrossprogressbar.h"

CrissCrossProgressBar::CrissCrossProgressBar(int row, int column, int spacing, QWidget *parent) : QWidget(parent)
{
    this->row = row;
    this->column = column;
    this->spacing = spacing;

    int numberOfBars = this->row + this->column;
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    this->firstLayout = new QVBoxLayout();

    this->progressBars = new QProgressBar*[numberOfBars];
    int barIndex = 0;

    for(int index = 0; index < this->row; ++index)
    {
        QProgressBar* bar = new QProgressBar(this);
        this->progressBars[barIndex++] = bar;

        bar->setParent(this);
        this->firstLayout->insertWidget(0, bar);
        bar->setTextVisible(false);
        if((index % 2) > 0)
        {
            bar->setInvertedAppearance(true);
        }

        QSizePolicy sp = bar->sizePolicy();
        sp.setRetainSizeWhenHidden(true);
        bar->setSizePolicy(sp);
        bar->setStyleSheet("border : 0px;margin : -1px;background:transparent;");
    }

    this->secondLayout = new QHBoxLayout();

    for(int index = 0; index < this->column; ++index)
    {
        QProgressBar* bar = new QProgressBar(this);
        this->progressBars[barIndex++] = bar;
        bar->setOrientation(Qt::Orientation::Vertical);
        this->secondLayout->addWidget(bar);
        bar->setTextVisible(false);
        if((index % 2) > 0)
        {
            bar->setInvertedAppearance(true);
        }

        QSizePolicy sp = bar->sizePolicy();
        sp.setRetainSizeWhenHidden(true);
        bar->setSizePolicy(sp);
        bar->setStyleSheet("border : 0px;margin : -1px;background:transparent;");
    }

    this->firstLayout->setSpacing(this->spacing);
    this->secondLayout->setSpacing(this->spacing);

    QGridLayout* stacked = new QGridLayout(this);
    stacked->addItem(this->firstLayout, 0, 0);
    stacked->addItem(this->secondLayout, 0, 0);
    stacked->setMargin(0);
}

void CrissCrossProgressBar::SetProgress(float value)
{
    int numberOfBars = this->row + this->column;
    int targetFill = static_cast<int>(floor(value * 100 * numberOfBars));
    for(int i = 0 ; i < numberOfBars; ++i)
    {
        if(targetFill >= 100)
        {
            this->progressBars[i]->setValue(99);
        }
        else if(targetFill > 0)
        {
            this->progressBars[i]->show();
            this->progressBars[i]->setValue(targetFill);
        }
        else
        {
            this->progressBars[i]->hide();
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
