#include <qdebug.h>
#include "gamesettings.h"

GameSettings::GameSettings()
{
    if(GameSettings::instance != nullptr)
    {
        qDebug("Multiple settings instanciated!");
        delete this;
        return;
    }

    GameSettings::instance = this;
}

GameSettings::~GameSettings()
{
    if(GameSettings::instance == this)
    {
        GameSettings::instance = nullptr;
    }
}

GameSettings* GameSettings::Instance()
{
    return GameSettings::instance;
}

GameSettings* GameSettings::instance = nullptr;
