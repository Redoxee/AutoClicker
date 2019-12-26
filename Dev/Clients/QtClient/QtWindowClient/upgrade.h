#ifndef UPGRADE_H
#define UPGRADE_H

#include <QString>

class Upgrade
{
public:
    Upgrade(int index, QString name, int price)
    {
        this->Index = index;
        this->Name = name;
        this->Price = price;
        this->IsDirty = false;
    }

    QString GetLabel()
    {
        QString label = QString(this->Name + ": " + QString::number(this->Price));

        return label;
    }

    int Index;
    QString Name;
    int Price;
    bool IsDirty;
};

#endif // UPGRADE_H
