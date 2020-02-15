#ifndef AUTOCLICKERCONFIG_H
#define AUTOCLICKERCONFIG_H

#include <string>

namespace AutoClicker {
    static const int WindowWidth = 500;
    static const int WindowHeight = 600;
    inline unsigned long FrameLength() { return 1000;}
    inline unsigned long RefreshRate() { return 250;}
    inline std::string BaseURI() { return "http://localhost:1414/AutoClicker?";}
}

#endif // AUTOCLICKERCONFIG_H
