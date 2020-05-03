#include "upgradebutton.h"
#include "servergameplaystate.h"

UpgradeButton::UpgradeButton(Upgrade* upgrade, QWidget *parent) : QPushButton(parent)
{
    this->upgrade = upgrade;
    this->setText(this->upgrade->GetLabel());
}

void UpgradeButton::RefreshLabel()
{
    this->setText(this->upgrade->GetLabel());
}


void UpgradeButton::SetUpgrade(Upgrade *upgrade)
{
    this->upgrade = upgrade;
}
