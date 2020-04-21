#include "tiledprogressbar.h"

TiledProgressBar::TiledProgressBar(QWidget *parent) : QWidget(parent)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QLayout* mainLayout = new QHBoxLayout(this);
    int index = 0;
    int numberOfBars = this->row * this->column * this->barPerTile;
    this->progressBars = new QProgressBar*[numberOfBars];
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    for(int colIndex = 0; colIndex < this->column; ++colIndex)
    {
        QLayout* secondLayout = new QVBoxLayout();
        mainLayout->addItem(secondLayout);
        secondLayout->setMargin(0);
        secondLayout->setSpacing(0);

        for(int rowIndex = 0; rowIndex < this->row; ++rowIndex)
        {
            QLayout* tile;
            bool vert = false;
            if(((rowIndex % 2) + (colIndex % 2)) % 2 > 0)
            {
                vert = true;
                tile = new QHBoxLayout();
            }
            else
            {
                vert = false;
                tile = new QVBoxLayout();
            }
            secondLayout->addItem(tile);
            tile->setMargin(0);
            tile->setSpacing(0);

            for(int barIndex = 0; barIndex < this->barPerTile; ++barIndex)
            {
                QProgressBar* bar = new QProgressBar(this);
                this->progressBars[index++] = bar;
                tile->addWidget(bar);
                if(vert)
                {
                    bar->setOrientation(Qt::Orientation::Vertical);
                }

                if(colIndex % 2 > 0)
                {
                    bar->setInvertedAppearance(true);
                }

                bar->setTextVisible(false);
                QSizePolicy sizePolicy = bar->sizePolicy();
                sizePolicy.setRetainSizeWhenHidden(true);
                bar->setSizePolicy(sizePolicy);
            }
        }
    }
}

void TiledProgressBar::SetValue(float value)
{
    int numberOfBars = this->row * this->column * this->barPerTile;
    int targetFill = static_cast<int>(floor(value * 100 * numberOfBars));
    for(int i = 0 ; i < numberOfBars; ++i)
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
