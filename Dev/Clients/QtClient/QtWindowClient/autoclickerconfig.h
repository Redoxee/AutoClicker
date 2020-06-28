#ifndef AUTOCLICKERCONFIG_H
#define AUTOCLICKERCONFIG_H

#include <string>

namespace AutoClicker {
    inline unsigned long FrameLength() { return 500;}
    inline unsigned long RefreshRate() { return 250;}
    inline std::string InitFilePath() {return "Ressources/client_init.json";};
    inline std::string BaseURI() { return "http://localhost:1414/AutoClicker?"; }
    inline std::string RelativeCoreServerPath() { return "/Server/ClickerServer.exe"; }
    inline std::string RelativeServerConfigPath() { return "/Content/WizardClicker.json"; }
    inline int ScoreFormula(int frame)
    {
        int score = 300000 - frame * 3;
        return score > 0 ? score : 0;
    }
}

#endif // AUTOCLICKERCONFIG_H
