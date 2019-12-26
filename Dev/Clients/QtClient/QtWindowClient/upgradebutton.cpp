#include "upgradebutton.h"

UpgradeButton::UpgradeButton(class Upgrade* upgrade, QWidget *parent) : QPushButton(parent)
{
    this->Upgrade = upgrade;
    this->setText(this->Upgrade->GetLabel());
}

void UpgradeButton::RefreshLabel()
{
    this->setText(this->Upgrade->GetLabel());
}
