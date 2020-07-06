#include "endscorewidget.h"

EndScoreWidget::EndScoreWidget(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout* hLayout = new QHBoxLayout(this);
    QVBoxLayout* lLayout = new QVBoxLayout();
    QVBoxLayout* rLayout = new QVBoxLayout();
    QVBoxLayout* cLayout = new QVBoxLayout();
    hLayout->addLayout(lLayout);
    hLayout->addLayout(cLayout);
    hLayout->addLayout(rLayout);

    hLayout->setMargin(0);
    hLayout->setSpacing(0);
    lLayout->setSpacing(0);
    cLayout->setSpacing(0);
    rLayout->setSpacing(0);

    this->progressbars = new QProgressBar*[this->nbProgressBarRows * 2];

    for(int i = 0; i < 35; ++i)
    {
        QProgressBar* lBar = new QProgressBar();
        lBar->setTextVisible(false);
        lBar->setValue(100);
        lBar->setStyleSheet("border : 0px;margin : -1px;background:transparent;");
        lLayout->addWidget(lBar);

        QProgressBar* rBar = new QProgressBar();
        rBar->setTextVisible(false);
        rBar->setValue(100);
        rBar->setInvertedAppearance(true);
        rBar->setStyleSheet("border : 0px;margin : -1px;background:transparent;");
        rLayout->addWidget(rBar);
        this->progressbars[i * 2] = lBar;
        this->progressbars[i * 2+ 1] = rBar;

        lBar->setMaximumWidth(50);
        rBar->setMaximumWidth(50);
    }

    this->centralLayout = cLayout;
}


void EndScoreWidget::Update(float time)
{
    const float TAU = 6.2831853f;

    float t = time * .001;

    float f = t * .025;
    f = f < 1 ? f : 1;

    for(int i = 0; i < this->nbProgressBarRows; ++i)
    {
        const float p = static_cast<float>(i) / static_cast<float>(this->nbProgressBarRows);
        float freq = 1;

        float value = (sin(TAU * p * freq - t) * .3 + .5) * f;
        int iValue = static_cast<int>(value * 100);
        int prevValue = this->progressbars[i * 2]->value();
        this->progressbars[i * 2]->setValue(iValue);
        this->progressbars[i * 2 + 1]->setValue(iValue);

        if(prevValue == 0 && iValue != 0)
        {
            this->progressbars[i * 2]->repaint();
            this->progressbars[i * 2 + 1]->repaint();
        }
    }
}
