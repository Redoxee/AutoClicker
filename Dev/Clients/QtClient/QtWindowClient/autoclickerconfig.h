#ifndef AUTOCLICKERCONFIG_H
#define AUTOCLICKERCONFIG_H

#include <string>

namespace AutoClicker {
    static const int WindowWidth = 500;
    static const int WindowHeight = 600;
    inline unsigned long FrameLength() { return 500;}
    inline unsigned long RefreshRate() { return 250;}
    inline std::string BaseURI() { return "http://localhost:1414/AutoClicker?"; }
    inline std::string RelativeCoreServerPath() { return "/Server/ClickerServer.exe"; }
    inline std::string RelativeConfigPath() { return "/Content/WizardClicker.txt"; }
    inline int ScoreFormula(int frame)
    {
        int score = 300000 - frame * 3;
        return score > 0 ? score : 0;
    }
}

#endif // AUTOCLICKERCONFIG_H
