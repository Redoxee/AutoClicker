#include "upgradebutton.h"

UpgradeButton::UpgradeButton(class Upgrade* upgrade, QWidget *parent) : QPushButton(parent)
{
    this->Upgrade = upgrade;
    this->setText(this->Upgrade->GetLabel());
}
