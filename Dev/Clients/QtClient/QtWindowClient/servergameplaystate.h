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
    int Index = 0;

    QString GetPriceLabel()
    {
        QString label = QString("Aquire\n%1").arg(SWIUtils::FormatDownQuantity(this->Price));
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

    Upgrade* clickUpgrade = nullptr;
    Upgrade* Upgrades = nullptr;
    size_t NumberOfUpgrades = 0;

    const QString prestigeName = "Prestige_1";
    const QString prestigeImproveName = "PrestigeImprove";
    const QString clickUpgradeName = "Click_upgrade_1";
    const QString clickUpgradeImproveName = "Improve_click_upgrade_1";
    const QString clickFactorName = "ClickBonus";
    const QString firstGeneratorName = "Generator_1";
    const QString firstGeneratorImproveName = "Improve_generator_1";
    const QString secondGeneratorName = "Generator_1";
    const QString secondGeneratorImproveName = "Improve_generator_1";

    static const int ClickUpgradeIndex = 0;
    static const int ClickUpgradeImproveIndex = 1;
    static const int ClickBonusFactorIndex = 2;
    static const int Generator1Index = 3;
    static const int Generator1ImproveIndex = 4;
    static const int Generator2Index = 5;
    static const int Generator2ImproveIndex = 6;
    static const int PrestigeIndex = 7;
    static const int PrestigeImproveIndex = 8;

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
            Upgrade* upgrade = &this->Upgrades[index];

            int impactValue = jsonUpgrade["ImpactValue"].toInt(0);

            upgrade->Name = jsonUpgrade["Name"].toString();
            upgrade->Description = jsonUpgrade["Description"].toString().arg(impactValue);
            upgrade->Price = jsonUpgrade["Price"].toInt();
            upgrade->InstanceBought = jsonUpgrade["NumberOfInstanceBought"].toInt();
            upgrade->Index = index;

            upgrade->FailureFlags = jsonUpgrades[index].toObject()["FailureFlags"].toInt();
            if(upgrade->Name == ServerGameplayState::clickUpgradeName)
            {
                this->clickUpgrade = upgrade;
            }
        }
    }
};

#endif // SERVERGAMEPLAYSTATE_H
