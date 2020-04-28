#ifndef UPGRADEBUTTON_H
#define UPGRADEBUTTON_H

#include <QPushButton>
#include "servergameplaystate.h"

class UpgradeButton : public QPushButton
{
    Q_OBJECT
public:
    explicit UpgradeButton(ServerGameplayState::Upgrade* upgrade, QWidget *parent = nullptr);
    void RefreshLabel();

    ServerGameplayState::Upgrade* Upgrade;
signals:

public slots:
};

#endif // UPGRADEBUTTON_H
