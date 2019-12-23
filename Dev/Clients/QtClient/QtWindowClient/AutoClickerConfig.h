#ifndef AUTOCLICKERCONFIG_H
#define AUTOCLICKERCONFIG_H

#include <string>

namespace AutoClicker {
    inline unsigned long TickLength() { return 100;}
    inline unsigned long RefreshRate() { return 250;}
    inline std::string BaseURI() { return "http://localhost:1234/AutoClicker?";}
}

#endif // AUTOCLICKERCONFIG_H
