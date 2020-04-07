#ifndef UPGRADE_H
#define UPGRADE_H

#include <QString>
#include "SWIUtils.h"

class Upgrade
{
public:
    Upgrade(int index, QString name, int price, int instanceBought)
    {
        this->Index = index;
        this->Name = name;
        this->Price = price;
        this->IsDirty = false;
        this->InstanceBought = instanceBought;
    }

    QString GetLabel()
    {

        QString label = QString("(%1) %2 :\n %3").arg(FormatDownQuantity(this->InstanceBought), this->Name, FormatDownQuantity(this->Price));

        return label;
    }

    int Index;
    QString Name;
    int Price;
    int InstanceBought;

    bool IsDirty;

};

#endif // UPGRADE_H
