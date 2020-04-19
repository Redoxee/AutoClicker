#include "gridprogressbar.h"

GridProgressBar::GridProgressBar(QWidget *parent, int row, int column) : QWidget(parent)
{
    this->row = row;
    this->column = column;

    this->gridLayout = new QGridLayout(this);
    this->gridLayout->setSpacing(0);
    for(int x = 0; x < column; ++x)
    {
        for(int y = 0; y < row; ++y)
        {
            QWidget* dummyWidget = new QWidget(this);

            QProgressBar* hBar = new QProgressBar(this);

            hBar->setTextVisible(false);

            hBar->setStyleSheet("QProgressBar {\
                                       padding: -1px;\
            background: rgba(0,0,0,100%);\
                                       };\
               QProgressBar::chunk{border-top: 1px solid black;border-bottom: 1px solid black};\
            ");

            QProgressBar* vBar = new QProgressBar(this);

            vBar->setTextVisible(false);
   /*        vBar->setStyleSheet("QProgressBar {\
                                        border: 0px solid black;\
                                       padding: -1px;\
                                        background: transparent;\
                                       }");
//        QProgressBar::chunk{border-left: 1px solid black;border-right: 1px solid black};
*/
            vBar->setOrientation(Qt::Orientation::Vertical);

            if(x % 2 + y % 2 > 0)
            {
                hBar->setParent(dummyWidget);
                vBar->setParent(dummyWidget);
            }
            else
            {
                vBar->setParent(dummyWidget);
                hBar->setParent(dummyWidget);
            }

            vBar->setValue(0);
            hBar->setValue(0);

            this->gridLayout->addWidget(dummyWidget,x,y);

        }
    }

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->gridLayout->setSizeConstraint(QLayout::SizeConstraint::SetNoConstraint);
}
