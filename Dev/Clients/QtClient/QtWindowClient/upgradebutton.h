#ifndef UPGRADEBUTTON_H
#define UPGRADEBUTTON_H

#include <QPushButton>
#include "upgrade.h"

class UpgradeButton : public QPushButton
{
    Q_OBJECT
public:
    explicit UpgradeButton(Upgrade* upgrade, QWidget *parent = nullptr);
    void RefreshLabel();

    Upgrade* Upgrade;
signals:

public slots:
};

#endif // UPGRADEBUTTON_H
