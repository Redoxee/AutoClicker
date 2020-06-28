#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H

#include <QJsonObject>

class GameSettings
{
public:
    GameSettings();
    virtual ~GameSettings();

    bool SkipIntroCinematic = false;
    bool SkipUpgradeReveal = false;
    bool DontKillProcessOnClose = false;

    QJsonObject LoadingQuips;

    static GameSettings* Instance();

private:
    static GameSettings* instance;
};

#endif // GAMESETTINGS_H
