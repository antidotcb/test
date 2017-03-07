#pragma once

#include <string>


namespace testapp {

class Controls
{
public:
    static void ClearScreen();
    static void Progressbar(const char* title);
    static void Username(const std::string& username);
};

}
