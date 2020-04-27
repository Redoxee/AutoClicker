#ifndef SERVERGAMEPLAYSTATE_H
#define SERVERGAMEPLAYSTATE_H

#include <QtNetwork/QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

struct ServerGameplayState
{
    struct Upgrade
    {
        QString Name = "";
        QString Description = "";
        QString Price = "";
        int InstanceBought = 0;
        int FailureFlags = 0;
    };

    int Score = 0;
    int FrameCount = 0;
    int TargetScore = 0;
    int ClickValue = 0;
    int PassiveSpeed = 0;
    int GlobalFactor = 0;

    Upgrade* Upgrades = nullptr;
    int NumberOfUpgrades = 0;

    void SetDataData(QJsonObject jsonData)
    {
        this->Score = jsonData["Score"].toInt();
        this->FrameCount = jsonData["FrameCount"].toInt();
        this->TargetScore = jsonData["TargetScore"].toInt();
        this->ClickValue = jsonData["ClickValue"].toInt();
        this->PassiveSpeed = jsonData["PassiveSpeed"].toInt();
        this->GlobalFactor = jsonData["GlobalFactor"].toInt();

        QJsonArray jsonUpgrades = jsonData["Upgrades"].toArray();
        int nbUpgrades = jsonUpgrades.size();

        if(this->Upgrades != nullptr && this->NumberOfUpgrades != nbUpgrades)
        {
            delete[] this->Upgrades;
            this->Upgrades = new Upgrade[nbUpgrades];
        }
        else if(this->Upgrades == nullptr)
        {
            this->Upgrades = new Upgrade[nbUpgrades];
        }

        this->NumberOfUpgrades = nbUpgrades;

        for(int index = 0; index < nbUpgrades; ++index)
        {
            QJsonObject jsonUpgrade = jsonUpgrades[index].toObject();
            Upgrade* upgrd = &this->Upgrades[index];

            upgrd->Name = jsonUpgrade["Name"].toString();
            upgrd->Description = jsonUpgrade["Description"].toString();
            upgrd->Price = jsonUpgrade["Price"].toInt();
            upgrd->InstanceBought = jsonUpgrade["NumberOfInstanceBought"].toInt();

            upgrd->FailureFlags = jsonUpgrades[index].toObject()["FailureFlags"].toInt();
        }
    }
};

#endif // SERVERGAMEPLAYSTATE_H
