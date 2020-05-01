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
        lBar->setValue(50 + i);
        lBar->setStyleSheet("border : 0px;margin : -1px;background:transparent;");
        lLayout->addWidget(lBar);

        QProgressBar* rBar = new QProgressBar();
        rBar->setTextVisible(false);
        rBar->setValue(50 + i);
        rBar->setInvertedAppearance(true);
        rBar->setStyleSheet("border : 0px;margin : -1px;background:transparent;");
        rLayout->addWidget(rBar);

        this->progressbars[i * 2] = lBar;
        this->progressbars[i * 2+ 1] = rBar;
    }

    this->centralLayout = cLayout;
}


void EndScoreWidget::Update(float time)
{
    const float TAU = 6.2831853f;

    float t = time * .001;

    for(int i = 0; i < this->nbProgressBarRows; ++i)
    {
        const float p = static_cast<float>(i) / static_cast<float>(this->nbProgressBarRows);
        float freq = .3 + sin(t * .125 + p) * 1.7 + .85;

        float value = sin(TAU * p * freq + t) * .3 + .5;
        int iValue = static_cast<int>(value * 100);
        this->progressbars[i * 2]->setValue(iValue);
        this->progressbars[i * 2 + 1]->setValue(iValue);
    }
}
