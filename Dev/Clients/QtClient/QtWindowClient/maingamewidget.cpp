#include "maingamewidget.h"

MainGameWidget::MainGameWidget(QWidget* parent) : QWidget(parent)
{

}

MainGameWidget::~MainGameWidget()
{
    this->refreshWorker->stop();
    for(auto current = this->UpgradeButtons.begin(); current != this->UpgradeButtons.end(); current++)
    {
        delete (*current)->Upgrade;
    }
    this->UpgradeButtons.clear();

    QLayoutItem *child;
    while ((child = this->UpgradeLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }
}
