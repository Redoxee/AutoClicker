#ifndef SERVERGAMEPLAYSTATE_H
#define SERVERGAMEPLAYSTATE_H

#include <QtNetwork/QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "SWIUtils.h"

enum class FailureFlags
{
    None = 0,
    NotEnoughMoney = 1,
    PurchaseLimitReached = 2,
    GameOver = 4,
    UnknownUpgrade = 8,
    LockedByAnOtherPurchase = 16,
};

namespace FailureFlagsUtils {
    inline FailureFlags operator~ (FailureFlags a) { return static_cast<FailureFlags>(~static_cast<int>(a)); }
    inline FailureFlags operator| (FailureFlags a, FailureFlags b) { return static_cast<FailureFlags>(static_cast<int>(a) | static_cast<int>(b)); }
    inline FailureFlags operator& (FailureFlags a, FailureFlags b) { return static_cast<FailureFlags>(static_cast<int>(a) & static_cast<int>(b)); }
    inline FailureFlags operator^ (FailureFlags a, FailureFlags b) { return static_cast<FailureFlags>(static_cast<int>(a) ^ static_cast<int>(b)); }
    inline FailureFlags& operator|= (FailureFlags& a, FailureFlags b) { a = static_cast<FailureFlags>(static_cast<int>(a) | static_cast<int>(b)); return a;}
    inline FailureFlags& operator&= (FailureFlags& a, FailureFlags b) { a = static_cast<FailureFlags>(static_cast<int>(a) & static_cast<int>(b)); return a;}
    inline FailureFlags& operator^= (FailureFlags& a, FailureFlags b) { a = static_cast<FailureFlags>(static_cast<int>(a) ^ static_cast<int>(b)); return a;}
}

struct Upgrade
{
    QString Name = "";
    QString Description = "";
    int Price = 0;
    int InstanceBought = 0;
    FailureFlags FailureFlags = FailureFlags::None;
    int Index = 0;
    int ImpactValue = 0;

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

    Upgrade* prestige = nullptr;
    Upgrade* prestigeImprove = nullptr;
    Upgrade* clickUpgrade = nullptr;
    Upgrade* clickUpgradeImprove = nullptr;
    Upgrade* clickFactor = nullptr;
    Upgrade* firstGenerator = nullptr;
    Upgrade* firstGeneratorImprove = nullptr;
    Upgrade* secondGenerator = nullptr;
    Upgrade* secondGeneratorImprove = nullptr;

    Upgrade* Upgrades = nullptr;
    size_t NumberOfUpgrades = 0;

    const QString prestigeName = "Prestige_1";
    const QString prestigeImproveName = "PrestigeImprove";
    const QString clickUpgradeName = "Click_upgrade_1";
    const QString clickUpgradeImproveName = "Improve_click_upgrade_1";
    const QString clickFactorName = "ClickBonus";
    const QString firstGeneratorName = "Generator_1";
    const QString firstGeneratorImproveName = "Improve_generator_1";
    const QString secondGeneratorName = "Generator_2";
    const QString secondGeneratorImproveName = "Improve_generator_2";

    static const int ClickUpgradeIndex = 0;
    static const int ClickUpgradeImproveIndex = 1;
    static const int ClickBonusFactorIndex = 2;
    static const int FirstGeneratorIndex = 3;
    static const int FirstGeneratorImproveIndex = 4;
    static const int SecondGeneratorIndex = 5;
    static const int SecondGeneratorImproveIndex = 6;
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
            upgrade->ImpactValue = jsonUpgrade["ImpactValue"].toInt();

            upgrade->Index = index;

            int iFailureFlags = jsonUpgrades[index].toObject()["FailureFlags"].toInt();
            upgrade->FailureFlags = static_cast<FailureFlags>(iFailureFlags);
            if(upgrade->Name == ServerGameplayState::clickUpgradeName)
            {
                this->clickUpgrade = upgrade;
            }
            else if(upgrade->Name == ServerGameplayState::clickUpgradeImproveName)
            {
                this->clickUpgradeImprove = upgrade;
            }
            else if(upgrade->Name == ServerGameplayState::clickFactorName)
            {
                this->clickFactor = upgrade;
            }
            else if(upgrade->Name == ServerGameplayState::firstGeneratorName)
            {
                this->firstGenerator = upgrade;
            }
            else if(upgrade->Name == ServerGameplayState::firstGeneratorImproveName)
            {
                this->firstGeneratorImprove = upgrade;
            }
            else if(upgrade->Name == ServerGameplayState::secondGeneratorName)
            {
                this->secondGenerator = upgrade;
            }
            else if(upgrade->Name == ServerGameplayState::secondGeneratorImproveName)
            {
                this->secondGeneratorImprove = upgrade;
            }
            else if(upgrade->Name == ServerGameplayState::prestigeName)
            {
                this->prestige = upgrade;
            }
            else if(upgrade->Name == ServerGameplayState::prestigeImproveName)
            {
                this->prestigeImprove = upgrade;
            }
        }
    }
};

#endif // SERVERGAMEPLAYSTATE_H
