#include "upgradebutton.h"

UpgradeButton::UpgradeButton(QString mainLabel, QWidget *parent) : QWidget(parent)
{
    QHBoxLayout* hLayout = new QHBoxLayout(this);
    hLayout->setMargin(0);
    hLayout->setSpacing(0);
    this->MainButton = new QPushButton(this);
    hLayout->addWidget(this->MainButton);
    this->SecondaryButton = new QPushButton(this);
    hLayout->addWidget(this->SecondaryButton);

    this->MainButton->setText(mainLabel);
    this->SecondaryButton->setText("+");

    this->MainButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    this->SecondaryButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    this->MainButton->setFixedWidth(55);
    this->SecondaryButton->setFixedWidth(25);
}
