#include "spiralprogressbar.h"

SpiralProgressBar::SpiralProgressBar(QWidget *parent) : QWidget(parent)
{

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->progressBars = new QProgressBar*[this->numberOfLoops * 4];

    QLayout* lastLayout = new QVBoxLayout(this);
    lastLayout->setMargin(0);
    lastLayout->setSpacing(0);

    for(int i = 0; i < this->numberOfLoops; ++i)
    {
        int index = i * 4;
        this->progressBars[index + 0] = new QProgressBar(this);
        this->progressBars[index + 1] = new QProgressBar(this);
        this->progressBars[index + 2] = new QProgressBar(this);
        this->progressBars[index + 3] = new QProgressBar(this);

        QLayout* nextLayout = new QHBoxLayout();
        QProgressBar* progressBar = this->progressBars[index + 0];
        lastLayout->addItem(nextLayout);
        lastLayout->addWidget(progressBar);
        progressBar->setTextVisible(false);

        lastLayout = nextLayout;
        lastLayout->setMargin(0);
        lastLayout->setSpacing(0);

        nextLayout = new QVBoxLayout();
        progressBar = this->progressBars[index + 1];
        lastLayout->addItem(nextLayout);
        lastLayout->addWidget(progressBar);
        progressBar->setTextVisible(false);
        progressBar->setOrientation(Qt::Orientation::Vertical);

        lastLayout = nextLayout;
        lastLayout->setMargin(0);
        lastLayout->setSpacing(0);

        nextLayout = new QHBoxLayout();
        progressBar = this->progressBars[index + 2];
        lastLayout->addWidget(progressBar);
        lastLayout->addItem(nextLayout);
        progressBar->setTextVisible(false);
        progressBar->setInvertedAppearance(true);

        lastLayout = nextLayout;
        lastLayout->setMargin(0);
        lastLayout->setSpacing(0);

        nextLayout = new QVBoxLayout();
        progressBar = this->progressBars[index + 3];
        lastLayout->addWidget(progressBar);
        lastLayout->addItem(nextLayout);
        progressBar->setTextVisible(false);
        progressBar->setOrientation(Qt::Orientation::Vertical);
        progressBar->setInvertedAppearance(true);

        lastLayout = nextLayout;
        lastLayout->setMargin(0);
        lastLayout->setSpacing(0);

        this->progressBars[index + 0]->setValue(0);
        this->progressBars[index + 1]->setValue(0);
        this->progressBars[index + 2]->setValue(0);
        this->progressBars[index + 3]->setValue(0);

        this->progressBars[index + 0]->hide();
        this->progressBars[index + 1]->hide();
        this->progressBars[index + 2]->hide();
        this->progressBars[index + 3]->hide();

        this->progressBars[index + 0]->setStyleSheet("border : 0px;margin : -1px;background:transparent;");
        this->progressBars[index + 1]->setStyleSheet("border : 0px;margin : -1px;background:transparent;");
        this->progressBars[index + 2]->setStyleSheet("border : 0px;margin : -1px;background:transparent;");
        this->progressBars[index + 3]->setStyleSheet("border : 0px;margin : -1px;background:transparent;");
    }

    this->CentralLayout = new QHBoxLayout();
    lastLayout->addItem(this->CentralLayout);

    this->progressBars[0]->setTextVisible(true);
}

void SpiralProgressBar::SetProgress(float value)
{
    int targetFill = static_cast<int>(floor(value * 100 * this->numberOfLoops * 4));
    int nbBars = this->numberOfLoops * 4;
    for(int i = 0 ; i < nbBars; ++i)
    {
        if(targetFill >= 100)
        {
            this->progressBars[i]->setValue(99);
            this->progressBars[i]->setTextVisible(false);
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
