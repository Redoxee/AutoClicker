#ifndef UPGRADEBUTTON_H
#define UPGRADEBUTTON_H

#include <QPushButton>

struct Upgrade;

class UpgradeButton : public QPushButton
{
    Q_OBJECT
public:
    explicit UpgradeButton(Upgrade* upgrade, QWidget *parent = nullptr);
    void RefreshLabel();

    void SetUpgrade(Upgrade* upgrade);
private:
    Upgrade* upgrade;
signals:

public slots:
};

#endif // UPGRADEBUTTON_H
