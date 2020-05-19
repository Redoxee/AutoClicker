#ifndef SERVERGAMEPLAYSTATE_H
#define SERVERGAMEPLAYSTATE_H

#include <QtNetwork/QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "SWIUtils.h"

struct Upgrade
{
    QString Name = "";
    QString Description = "";
    int Price = 0;
    int InstanceBought = 0;
    int FailureFlags = 0;

    QString GetLabel()
    {
        QString label = QString("(%1)\n %2 :\n %3").arg(SWIUtils::FormatDownQuantity(this->InstanceBought), this->Name, SWIUtils::FormatDownQuantity(this->Price));

        return label;
    }
};

struct ServerGameplayState
{
    int Score = 0;
    int FrameCount = 0;
    int TargetScore = 0;
    int ClickValue = 0;
    int PassiveSpeed = 0;
    int GlobalFactor = 0;

    int TempBonusFactor = 0;
    int TempBonusDuration = 0;

    int WakeTimer = 0;
    int WakeDuration = 0;

    Upgrade* Upgrades = nullptr;
    size_t NumberOfUpgrades = 0;

    bool IsFinished()
    {
        return  this->Score >= this->TargetScore;
    }

    bool IsSleeping()
    {
        return this->WakeTimer>0 && this->WakeDuration > 0;
    }

    void SetDataData(QJsonObject jsonData)
    {
        this->Score = jsonData["Score"].toInt();
        this->FrameCount = jsonData["FrameCount"].toInt();
        this->TargetScore = jsonData["TargetScore"].toInt();
        this->ClickValue = jsonData["ClickValue"].toInt();
        this->PassiveSpeed = jsonData["PassiveSpeed"].toInt();
        this->GlobalFactor = jsonData["GlobalFactor"].toInt();

        this->TempBonusDuration = jsonData["TemporaryBonusTimer"].toInt();
        this->TempBonusFactor = jsonData["TemporaryBonusFactor"].toInt();

        this->WakeTimer = jsonData["WakeTimer"].toInt();
        this->WakeDuration = jsonData["WakeDuration"].toInt();

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

            int impactValue = jsonUpgrade["ImpactValue"].toInt(0);

            upgrd->Name = jsonUpgrade["Name"].toString();
            upgrd->Description = jsonUpgrade["Description"].toString().arg(impactValue);
            upgrd->Price = jsonUpgrade["Price"].toInt();
            upgrd->InstanceBought = jsonUpgrade["NumberOfInstanceBought"].toInt();

            upgrd->FailureFlags = jsonUpgrades[index].toObject()["FailureFlags"].toInt();
        }
    }
};

#endif // SERVERGAMEPLAYSTATE_H
