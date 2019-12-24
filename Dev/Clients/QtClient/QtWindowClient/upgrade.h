#ifndef UPGRADE_H
#define UPGRADE_H

#include <QString>

class Upgrade
{
public:
    Upgrade(QString name, int price)
    {
        this->Name = name;
        this->Price = price;
        this->IsDirty = false;
    }

    QString Name;
    int Price;
    bool IsDirty;
};

#endif // UPGRADE_H
