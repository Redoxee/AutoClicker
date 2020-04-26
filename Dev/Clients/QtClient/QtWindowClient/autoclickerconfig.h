#ifndef AUTOCLICKERCONFIG_H
#define AUTOCLICKERCONFIG_H

#include <string>

namespace AutoClicker {
    static const int WindowWidth = 500;
    static const int WindowHeight = 600;
    inline unsigned long FrameLength() { return 800;}
    inline unsigned long RefreshRate() { return 200;}
    inline std::string BaseURI() { return "http://localhost:1414/AutoClicker?"; }
    inline std::string RelativeCoreServerPath() { return "/../Server/ClickerServer.exe"; }
    inline std::string RelativeConfigPath() { return "/../Content/WizardClicker.txt"; }
}

#endif // AUTOCLICKERCONFIG_H
